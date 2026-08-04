/*
 * avr-ciclos — cronômetro de ciclos de clock para programas AVR.
 *
 * Executa o firmware no simulador (libsimavr, ciclo-preciso) e informa
 * quantos ciclos de clock e quanto tempo se passam entre dois pontos do
 * programa. Serve para responder "quanto tempo essa rotina leva?" sem
 * precisar de osciloscópio — o que o Stopwatch do MPLAB X/Atmel Studio faz.
 *
 * Uso:
 *   avr-ciclos <firmware.elf> [--de PONTO] [--ate PONTO] [--freq Hz]
 *              [--mcu nome] [--max ciclos]
 *
 *   PONTO pode ser um rótulo do seu assembly (ex.: loopit) ou um endereço
 *   (ex.: 0x0006). Sem --de, conta desde o reset. Sem --ate, conta até o
 *   limite de --max.
 *
 * Compilação: gcc -O2 avr-ciclos.c -o avr-ciclos -lsimavr
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <simavr/sim_avr.h>
#include <simavr/sim_elf.h>

static const char *NOME_PROG = "avr-ciclos";

static void uso(void)
{
	fprintf(stderr,
		"Uso: %s <firmware.elf> [opções]\n"
		"  --de PONTO     rótulo ou endereço onde a contagem começa (padrão: reset)\n"
		"  --ate PONTO    rótulo ou endereço onde a contagem termina\n"
		"  --freq Hz      clock da simulação (padrão: 16000000)\n"
		"  --mcu nome     dispositivo (padrão: atmega328p)\n"
		"  --max N        limite de ciclos para não rodar para sempre (padrão: 200000000)\n"
		"  --rotulos      lista os rótulos do programa e sai\n"
		"\nExemplo:\n"
		"  %s build/main.elf --de loopit --ate loopit    # uma volta do loop\n",
		NOME_PROG, NOME_PROG, NOME_PROG);
}

/*
 * Percorre a tabela de símbolos com avr-nm. O simavr expõe apenas símbolos
 * globais, e rótulos de assembly (loopit, delay, ...) são locais — por isso
 * lemos a tabela completa aqui. Se 'procurado' for NULL, apenas lista.
 */
static int percorre_rotulos(const char *elf, const char *procurado, uint32_t *saida)
{
	char cmd[2048], linha[512];
	int achou = 0;

	snprintf(cmd, sizeof(cmd), "avr-nm --numeric-sort \"%s\" 2>/dev/null", elf);
	FILE *p = popen(cmd, "r");
	if (!p)
		return 0;

	while (fgets(linha, sizeof(linha), p)) {
		char endereco[64], tipo[8], simbolo[256];

		if (sscanf(linha, "%63s %7s %255s", endereco, tipo, simbolo) != 3)
			continue;
		/* t/T = código (.text); w/W = símbolo fraco */
		if (tipo[0] != 't' && tipo[0] != 'T' && tipo[0] != 'w' && tipo[0] != 'W')
			continue;

		if (!procurado) {
			/* esconde os símbolos internos do linker (__ctors_end, ...) */
			if (strncmp(simbolo, "__", 2) == 0 || strcmp(simbolo, "_etext") == 0)
				continue;
			printf("  0x%04lx  %s\n", strtoul(endereco, NULL, 16), simbolo);
		} else if (strcmp(simbolo, procurado) == 0) {
			*saida = (uint32_t)strtoul(endereco, NULL, 16);
			achou = 1;
			break;
		}
	}
	pclose(p);
	return achou;
}

/* Resolve "0x1234" ou um rótulo do programa para endereço de flash (bytes). */
static int resolve_ponto(const char *elf, elf_firmware_t *f, const char *texto, uint32_t *saida)
{
	if (!texto)
		return 0;

	if (texto[0] == '0' && (texto[1] == 'x' || texto[1] == 'X')) {
		*saida = (uint32_t)strtoul(texto, NULL, 16);
		return 1;
	}

	if (percorre_rotulos(elf, texto, saida))
		return 1;

#if ELF_SYMBOLS
	for (uint32_t i = 0; i < f->symbolcount; i++) {
		avr_symbol_t *s = f->symbol[i];
		if (s->addr >= 0x800000)   /* símbolos de RAM vêm com esse offset */
			continue;
		if (strcmp(s->symbol, texto) == 0) {
			*saida = s->addr;
			return 1;
		}
	}
#endif
	fprintf(stderr, "%s: rótulo '%s' não encontrado em %s.\n", NOME_PROG, texto, elf);
	fprintf(stderr, "  Rótulos disponíveis (use --rotulos para ver de novo):\n");
	percorre_rotulos(elf, NULL, NULL);
	return -1;
}

static void mostra_tempo(uint64_t ciclos, uint32_t freq)
{
	double segundos = (double)ciclos / (double)freq;

	if (segundos >= 1.0)
		printf("Tempo    : %.6f s\n", segundos);
	else if (segundos >= 1e-3)
		printf("Tempo    : %.3f ms\n", segundos * 1e3);
	else if (segundos >= 1e-6)
		printf("Tempo    : %.3f us\n", segundos * 1e6);
	else
		printf("Tempo    : %.1f ns\n", segundos * 1e9);
}

int main(int argc, char *argv[])
{
	const char *arquivo = NULL, *txt_de = NULL, *txt_ate = NULL;
	const char *mcu = "atmega328p";
	uint32_t freq = 16000000;
	uint64_t max_ciclos = 200000000ULL;
	int listar_rotulos = 0;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--de") && i + 1 < argc)        txt_de  = argv[++i];
		else if (!strcmp(argv[i], "--ate") && i + 1 < argc)  txt_ate = argv[++i];
		else if (!strcmp(argv[i], "--freq") && i + 1 < argc) freq = (uint32_t)strtoul(argv[++i], NULL, 0);
		else if (!strcmp(argv[i], "--mcu") && i + 1 < argc)  mcu = argv[++i];
		else if (!strcmp(argv[i], "--max") && i + 1 < argc)  max_ciclos = strtoull(argv[++i], NULL, 0);
		else if (!strcmp(argv[i], "--rotulos"))              listar_rotulos = 1;
		else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) { uso(); return 0; }
		else if (argv[i][0] != '-' && !arquivo)              arquivo = argv[i];
		else { fprintf(stderr, "%s: argumento inesperado '%s'\n", NOME_PROG, argv[i]); uso(); return 2; }
	}

	if (!arquivo) { uso(); return 2; }

	if (listar_rotulos) {
		printf("Rótulos de %s:\n", arquivo);
		percorre_rotulos(arquivo, NULL, NULL);
		return 0;
	}
	if (freq == 0) { fprintf(stderr, "%s: --freq precisa ser maior que zero.\n", NOME_PROG); return 2; }

	elf_firmware_t f = {{0}};
	if (elf_read_firmware(arquivo, &f) != 0) {
		fprintf(stderr, "%s: não consegui ler '%s'.\n", NOME_PROG, arquivo);
		return 1;
	}
	f.frequency = freq;

	avr_t *avr = avr_make_mcu_by_name(mcu);
	if (!avr) {
		fprintf(stderr, "%s: dispositivo '%s' não suportado pelo simulador.\n", NOME_PROG, mcu);
		return 1;
	}
	avr_init(avr);
	avr_load_firmware(avr, &f);
	avr->frequency = freq;

	uint32_t addr_de = 0, addr_ate = 0;
	int tem_de = 0, tem_ate = 0;

	if (txt_de) {
		int r = resolve_ponto(arquivo, &f, txt_de, &addr_de);
		if (r < 0) return 1;
		tem_de = r;
	}
	if (txt_ate) {
		int r = resolve_ponto(arquivo, &f, txt_ate, &addr_ate);
		if (r < 0) return 1;
		tem_ate = r;
	}

	printf("Firmware : %s (%s @ %.3f MHz)\n", arquivo, mcu, freq / 1e6);
	if (tem_de)  printf("De       : %s (0x%04x)\n", txt_de, addr_de);
	else         printf("De       : reset (0x0000)\n");
	if (tem_ate) printf("Até      : %s (0x%04x)\n", txt_ate, addr_ate);
	else         printf("Até      : limite de %" PRIu64 " ciclos\n", max_ciclos);
	printf("---------------------------------------------\n");

	int contando = !tem_de;
	uint64_t ciclo_inicial = 0;
	int chegou_ao_fim = 0;

	while (avr->state != cpu_Done && avr->state != cpu_Crashed && avr->cycle < max_ciclos) {
		if (!contando && avr->pc == addr_de) {
			contando = 1;
			ciclo_inicial = avr->cycle;
		} else if (contando && tem_ate && avr->pc == addr_ate && avr->cycle > ciclo_inicial) {
			chegou_ao_fim = 1;
			break;
		}
		avr_run(avr);
	}

	if (tem_de && !contando) {
		fprintf(stderr, "%s: o programa nunca passou por '%s' em %" PRIu64 " ciclos.\n",
			NOME_PROG, txt_de, max_ciclos);
		return 1;
	}
	if (tem_ate && !chegou_ao_fim) {
		fprintf(stderr, "%s: o programa nunca chegou a '%s' em %" PRIu64 " ciclos "
			"(aumente com --max).\n", NOME_PROG, txt_ate, max_ciclos);
		return 1;
	}

	uint64_t ciclos = avr->cycle - ciclo_inicial;
	printf("Ciclos   : %" PRIu64 "\n", ciclos);
	mostra_tempo(ciclos, freq);

	if (avr->state == cpu_Crashed)
		printf("Aviso    : a CPU travou durante a simulação.\n");

	return 0;
}
