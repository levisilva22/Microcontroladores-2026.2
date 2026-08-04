/*
 * avr-serial — terminal serial para o simulador.
 *
 * Roda o firmware no simulador (libsimavr) com a USART0 ligada ao seu
 * terminal: o que o programa transmitir aparece na tela, e o que você
 * digitar é entregue ao programa como se viesse do PC pela serial.
 * Serve para testar código de USART sem precisar da placa.
 *
 * Uso: avr-serial <firmware.elf> [--freq Hz] [--mcu nome]
 * Sair: Ctrl+C
 *
 * Compilação: gcc -O2 avr-serial.c -o avr-serial -lsimavr -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <simavr/sim_avr.h>
#include <simavr/sim_elf.h>
#include <simavr/avr_uart.h>
#include <simavr/sim_irq.h>

static volatile int rodando = 1;
static struct termios termo_original;
static int termo_salvo = 0;

static void restaura_terminal(void)
{
	if (termo_salvo)
		tcsetattr(STDIN_FILENO, TCSANOW, &termo_original);
}

static void ao_interromper(int sinal)
{
	(void)sinal;
	rodando = 0;
}

/* Chamado pelo simulador a cada byte que o programa transmite. */
static void byte_transmitido(struct avr_irq_t *irq, uint32_t valor, void *param)
{
	(void)irq; (void)param;
	fputc(valor & 0xff, stdout);
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	const char *arquivo = NULL, *mcu = "atmega328p";
	uint32_t freq = 16000000;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--freq") && i + 1 < argc)     freq = (uint32_t)strtoul(argv[++i], NULL, 0);
		else if (!strcmp(argv[i], "--mcu") && i + 1 < argc) mcu = argv[++i];
		else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			fprintf(stderr, "Uso: avr-serial <firmware.elf> [--freq Hz] [--mcu nome]\n"
			                "Digite para enviar pela serial; Ctrl+C para sair.\n");
			return 0;
		}
		else if (argv[i][0] != '-' && !arquivo)             arquivo = argv[i];
		else { fprintf(stderr, "avr-serial: argumento inesperado '%s'\n", argv[i]); return 2; }
	}
	if (!arquivo) {
		fprintf(stderr, "Uso: avr-serial <firmware.elf> [--freq Hz] [--mcu nome]\n");
		return 2;
	}

	elf_firmware_t f = {{0}};
	if (elf_read_firmware(arquivo, &f) != 0) {
		fprintf(stderr, "avr-serial: não consegui ler '%s'.\n", arquivo);
		return 1;
	}
	f.frequency = freq;

	avr_t *avr = avr_make_mcu_by_name(mcu);
	if (!avr) {
		fprintf(stderr, "avr-serial: dispositivo '%s' não suportado.\n", mcu);
		return 1;
	}
	avr_init(avr);
	avr_load_firmware(avr, &f);
	avr->frequency = freq;

	/* Desliga o eco automático do simavr: nós mesmos imprimimos os bytes. */
	uint32_t flags = 0;
	avr_ioctl(avr, AVR_IOCTL_UART_GET_FLAGS('0'), &flags);
	flags &= ~AVR_UART_FLAG_STDIO;
	avr_ioctl(avr, AVR_IOCTL_UART_SET_FLAGS('0'), &flags);

	avr_irq_t *irq_saida  = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);
	avr_irq_t *irq_entrada = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT);
	if (!irq_saida || !irq_entrada) {
		fprintf(stderr, "avr-serial: este dispositivo não expõe USART0 no simulador.\n");
		return 1;
	}
	avr_irq_register_notify(irq_saida, byte_transmitido, NULL);

	/* Terminal em modo cru: cada tecla vai direto para o AVR, sem eco local. */
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &termo_original) == 0) {
		struct termios cru = termo_original;
		termo_salvo = 1;
		atexit(restaura_terminal);
		cfmakeraw(&cru);
		cru.c_lflag |= ISIG;            /* mantém Ctrl+C funcionando */
		cru.c_oflag |= OPOST | ONLCR;   /* \n continua quebrando linha na tela */
		tcsetattr(STDIN_FILENO, TCSANOW, &cru);
	}
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

	signal(SIGINT, ao_interromper);
	signal(SIGTERM, ao_interromper);

	fprintf(stderr, "== avr-serial: %s em %s @ %.3f MHz. Digite para enviar; Ctrl+C sai. ==\r\n",
		arquivo, mcu, freq / 1e6);

	/*
	 * Executa o firmware e, a cada fatia de simulação, entrega ao AVR o que
	 * foi digitado. A checagem periódica (em vez de a cada instrução) evita
	 * gastar tempo lendo o teclado dezenas de milhões de vezes por segundo.
	 */
	const uint32_t instrucoes_por_leitura = 10000;
	uint32_t contador = 0;

	while (rodando) {
		int estado = avr_run(avr);
		if (estado == cpu_Done || estado == cpu_Crashed)
			break;

		if (++contador >= instrucoes_por_leitura) {
			unsigned char buf[64];
			ssize_t lidos = read(STDIN_FILENO, buf, sizeof(buf));
			for (ssize_t i = 0; i < lidos; i++)
				avr_raise_irq(irq_entrada, buf[i]);
			contador = 0;
		}
	}

	restaura_terminal();
	fprintf(stderr, "\r\n== avr-serial: encerrado (%s) ==\n",
		avr->state == cpu_Crashed ? "CPU travou" : "fim");
	return 0;
}
