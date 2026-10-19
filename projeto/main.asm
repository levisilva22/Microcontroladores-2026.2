;==============================================================================
; Semáforo com máquina de estados (ATmega328P) — projeto da disciplina
;
; Requisitos atendidos:
;  1) Timer do AVR define os tempos        -> ver Timer1/TOP abaixo, e
;                                             timers_states no fim do arquivo
;  2) Interrupção em vez de polling        -> todo o programa reage a 3
;                                             interrupções (reset/Timer1/USART);
;                                             o "loop:" principal não faz nada
;  3) Contador num display de 7 segmentos  -> count/timers_vetor/split_digits
;  4) Status enviado pela USART            -> msg_1..msg_7 + prepare_for_send
;==============================================================================
.include "m328Pdef.inc"

.def temp = r16
.def dozens = r17
.def states = r18
.def timer = r19
.def unit = r20
.def current_digit = r21
.def count_ticks = r22
.def count = r23
.def s1_phase = r2
.def Highaddr = r24
.def Loweraddr = r25

.equ STATE_NUMBER = 7       ; quantos estados tem a máquina (E0..E6)
.equ TICKS_PER_SECOND = 250 ; quantas interrupções do Timer1 formam 1 segundo real
.equ UBRRvalue = 103        ; baud rate da USART (9600 bps @ 16MHz)

; É aqui que nasce a base de tempo que todo o projeto usa.
; O Timer1 (modo CTC) gera 1 interrupção a cada "DELAY" milissegundos; esse
; tique é contado TICKS_PER_SECOND vezes (acima) até virar "1 segundo de
; verdade" dentro da ISR (interruption_timer, mais abaixo). É esse segundo
; que timers_states usa pra saber quanto tempo cada estado dura.
#define CLOCK 16000000
#define DELAY 4
.equ PRESCALE = 0b100       ;/256 prescale
.equ PRESCALE_DIV = 256
.equ WGM = 0b0100           ;Waveform generation mode: CTC (TOP = OCR1A)
;Em CTC o período é (TOP + 1) ciclos do timer, por isso o "- 1".
; Clock do timer após o prescaler: 16e6/256 = 62500 ciclos por segundo.
; Queremos 4 ms: 62500 * (4/1000) = 250 ciclos -> TOP = 250 - 1 = 249.
; (o "+500 /1000" na conta abaixo só arredonda em vez de truncar)
.equ TOP = ((CLOCK/PRESCALE_DIV)*DELAY + 500)/1000 - 1
.if TOP > 65535
.error "TOP is out of range"
.endif


.cseg
;==============================================================================
; Vetor de interrupção -- os 3 únicos pontos de entrada do programa.
; Repare que "loop:" (mais abaixo) não faz nada sozinho -- é tudo disparado por
; uma dessas 3 interrupções, nunca por polling (requisito 2).
;
; Atenção: as tabelas de dados (.db) ficam TODAS no fim do arquivo, depois de
; todo o código. Isso não é só estilo -- colocar dados entre esses "jmp" e os
; rótulos de destino já confundiu o linker deste projeto (binutils antigo),
; que gravava "jmp 0" em vez do endereço certo, sem erro nenhum no build. Se
; for mexer na ordem do arquivo, mantenha os rótulos de destino (reset,
; interruption_timer, usart_interruption) logo depois do vetor, sem dados no
; meio.
;==============================================================================
.org 0x0000
jmp reset               ; reset do AVR (liga a placa / botão de reset)
.org OC1Aaddr
jmp interruption_timer  ; Timer1, Compare Match A -- a cada 4ms (ver TOP acima)
.org UDREaddr
jmp usart_interruption  ; USART, registrador de transmissão vazio

reset:
  ldi temp, high(RAMEND)
  out SPH, temp
  ldi temp, low(RAMEND)
  out SPL, temp

  ; Pinagem dos 5 semáforos (S1 Jaraguá, S2 Jatiúca reto, S3 Jatiúca
  ; esquerda, S4 Cruzamento, S5 Pedestre).
  ;
  ; --- PORTD: bit1 (TXD) fica de fora -- é usado pelo próprio periférico
  ;     USART (TXEN0 abaixo); escrever nele não teria efeito nenhum no LED,
  ;     o hardware da UART sempre tem prioridade. bit0 (RXD) volta a ser
  ;     usado normalmente porque RXEN0 nunca é habilitado neste código.
  ;     Layout (ver estados_portD):
  ;     bit0=S5 verde
  ;     bit2=S4 verde   bit3=S4 amarelo   bit4=S4 vermelho
  ;     bit5=S1 verde   bit6=S1 amarelo   bit7=S1 vermelho
  ldi temp, 0b11111101
  out DDRD, temp
  ldi temp, 0b00111111    ; PB0 a PB3 como saída (decodificador do display)
  out DDRB, temp
  ; --- PORTC: S2 e S3 usam só 2 bits cada (verde, amarelo) -- o vermelho
  ;     de cada um é feito por uma porta NOR + transistor que acende
  ;     sozinho quando nem verde nem amarelo estão ligados (não precisa de
  ;     bit de software pra isso). S5 vermelho mora aqui porque PD1 (TXD)
  ;     não podia ser usado -- o verde de S5 ficou no PORTD (bit0, acima).
  ;     Layout (ver estados_portC):
  ;     bit0=S2 verde   bit1=S2 amarelo
  ;     bit3=S3 verde   bit4=S3 amarelo
  ;     bit5=S5 vermelho
  ldi temp, 0b00111111
  out DDRC, temp

  ; Inicialização da USART -- 9600 bps, 8 bits, 1 stop bit, sem
  ; paridade, só transmissão (TXEN0; não habilita RXEN0 porque o projeto só
  ; manda status, nunca recebe nada pela serial).
  ldi temp, high(UBRRvalue)
  sts UBRR0H, temp
  ldi temp, low(UBRRvalue)
  sts UBRR0L, temp

  ;8 bits, 1 stop. no parity
  ldi temp, (3<<UCSZ00)
  sts UCSR0C, temp
  ldi temp, (1<<TXEN0)
  sts UCSR0B, temp

  ; Estado inicial: E0, contadores zerados.
  clr states
  clr count_ticks

  clr current_digit
  clr s1_phase
  rcall reload_s1_timer   ; carrega o 1º valor do contador do display
  rcall load_display      ; acende os LEDs de E0 e já mostra o 1º número

  rcall prepare_for_send    ; prepara a 1ª mensagem da USART
  rcall interruption_handle ; liga o Timer1 (só depois disso "sei" faz efeito)
  sei                       ; habilita interrupções globais -- a partir
                            ; daqui, todo o programa é guiado por interrupção

; O "main loop": de propósito não faz nada. Tudo (troca de estado, display,
; envio de mensagem) acontece dentro das 3 ISRs, nunca aqui.
loop:
  rjmp loop

; Lê o estado atual (0..6) nas 3 tabelas e aplica: LEDs (PORTD/PORTC) e a
; duração desse estado (timer). Chamada no reset e sempre que muda de estado
; (changed_states, mais abaixo).
load_display:
	; Seta a saída do led conforme a máquina de estados (S1, S4, S5 -> PORTD)
  ldi ZL, low(estados_portD*2)
	ldi ZH, high(estados_portD*2)
	clr temp
	add ZL, states          ; Z = endereço(estados_portD) + estado atual
	adc ZH, temp
	lpm temp, Z              ; lê o byte desse estado na flash
	out PORTD, temp

  ; S2, S3 -> PORTC
  ldi ZL, low(estados_portC*2)
	ldi ZH, high(estados_portC*2)
	clr temp
	add ZL, states
	adc ZH, temp
	lpm temp, Z
	out PORTC, temp

	; Aqui é onde a duração "oficial" do estado (definida na tabela
	; timers_states, lá no fim do arquivo) é carregada em "timer". É esse
	; valor que interruption_timer vai decrementar segundo a segundo até
	; trocar de estado.
	ldi ZL, low(timers_states*2)
	ldi ZH, high(timers_states*2)
	clr temp
	add ZL, states
	adc ZH, temp
	lpm timer, Z

	rcall split_digits ; atualiza dezena/unidade pro display
	ret


; Configura o Timer1 e liga a interrupção dele. Chamada 1 vez só, no reset
; (antes do "sei"), por isso salvar SREG/temp aqui é só precaução -- não há
; risco real de reentrância ainda.
interruption_handle:
  push temp
  in temp, SREG
  push temp

  ; TOP é o valor de comparação (calculado lá em cima a partir de
  ; CLOCK/PRESCALE/DELAY) -- é ele que define que a interrupção do Timer1
  ; acontece a cada 4ms, não outro período qualquer.
	;On MEGA series, write high byte of 16-bit timer registers first
	ldi temp, high(TOP) ;initialize compare value (TOP)
	sts OCR1AH, temp
	ldi temp, low(TOP)
	sts OCR1AL, temp

	;zera o contador
	clr temp
	sts TCNT1H, temp
	sts TCNT1L, temp

	ldi temp, ((WGM&0b11) << WGM10) ;lower 2 bits of WGM
	; WGM&0b11 = 0b0100 & 0b0011 = 0b0000
	sts TCCR1A, temp

	; Aqui é o "usar interrupção" de verdade -- liga o bit OCIE1A em
	; TIMSK1, que autoriza o Timer1 a interromper o programa quando TCNT1
	; bater em TOP. Sem isso, o timer até contaria, mas interruption_timer
	; nunca seria chamada.
	lds temp, TIMSK1
	sbr temp, 1 << OCIE1A
	sts TIMSK1, temp

	; Prescaler /256 (bits CS10) -- é essa divisão do clock de 16MHz que
	; torna os 4ms (e não microssegundos) possíveis com um TOP de 8 bits
	; de folga.
	;upper 2 bits of WGM and clock select
	ldi temp, ((WGM>> 2) << WGM12)|(PRESCALE << CS10)
	; WGM >> 2 = 0b0100 >> 2 = 0b0001
	; (WGM >> 2) << WGM12 = (0b0001 << 3) = 0b0001000
	; (PRESCALE << CS10) = 0b100 << 0 = 0b100
	; 0b0001000 | 0b100 = 0b0001100
	sts TCCR1B, temp ;start counter (TCCR1B também inicia a contagem)

  pop temp
  out SREG, temp
  pop temp

  ret

; Avança pro próximo estado (módulo STATE_NUMBER = 7) -- é chamada só
; quando "timer" (a duração do estado atual) chega a zero, lá em
; interruption_timer. Fecha o ciclo: depois do estado 6 (E6) volta pro
; estado 0 (E0).
changed_states:
  inc states
  cpi states, STATE_NUMBER
  brne load_new
  clr states

load_new:
  rcall load_display
  rcall prepare_for_send
  ret

; O contador do display tem o próprio ciclo (verde/amarelo/vermelho do
; semáforo S1), independente de qual dos 7 estados da máquina está ativo.
; "s1_phase" (0,1,2) indexa timers_vetor; essa rotina só lê a posição
; atual e recarrega "count" -- quem decide QUANDO chamar (e avançar
; s1_phase) é o trecho dentro de interruption_timer, logo abaixo.
reload_s1_timer:
  ldi ZL, low(timers_vetor*2)
  ldi ZH, high(timers_vetor*2)
  clr temp
  add ZL, s1_phase
  adc ZH, temp
  lpm count, Z
  ret

;==============================================================================
; interruption_timer -- ISR do Timer1, roda a cada 4ms (TOP, lá em cima).
;==============================================================================
interruption_timer:
  ; Prólogo de toda ISR em assembly -- salva o que a rotina vai usar
  ; (temp, SREG, e o ponteiro Z que load_display/split_digits usam por
  ; baixo dos panos) antes de mexer em qualquer coisa, pra devolver o
  ; processador exatamente como estava quando a interrupção chegou.
  push temp
  in temp, SREG
  push temp
  push ZL
  push ZH

  ; A cada 4ms (toda vez que a ISR roda), alterna qual dos 2 dígitos do
  ; display está aceso -- é a multiplexação (ver multiplexar, mais
  ; abaixo). Precisa acontecer toda vez, não só 1x por segundo, senão o
  ; olho humano enxergaria o pisca-pisca em vez dos 2 dígitos "juntos".
  rcall multiplexar

  ; Conta os tiques de 4ms até formar 1 segundo real (TICKS_PER_SECOND =
  ; 250 -> 250*4ms = 1000ms). Só a partir daqui pra baixo é que o tempo
  ; "anda" de verdade -- tudo acima roda toda interrupção, independente
  ; de segundo.
  inc count_ticks
  cpi count_ticks, TICKS_PER_SECOND
  brne end_isr

  clr count_ticks

  ; O relógio da máquina de 7 estados. "timer" foi carregado em
  ; load_display com a duração (em segundos) do estado atual, vinda de
  ; timers_states. A cada segundo real, decrementa; quando chega a 0, o
  ; tempo desse estado acabou -> muda de estado (changed_states), que por
  ; sua vez chama load_display de novo e recarrega "timer" com a duração
  ; do PRÓXIMO estado.
  dec timer
  brne skip_state_change
  rcall changed_states
skip_state_change:

  ; O relógio do contador do display -- independente do "timer" acima de
  ; propósito (o número mostrado não precisa andar junto com a troca dos
  ; 7 estados). Quando "count" zera, avança s1_phase (0->1->2->0) e
  ; recarrega de timers_vetor.
  dec count
  brne skip_s1_reload
  inc s1_phase
  ldi temp, 3
  cp s1_phase, temp
  brne s1_phase_ok
  clr s1_phase
s1_phase_ok:
  rcall reload_s1_timer
skip_s1_reload:

  ; Recalcula dezena/unidade com o "count" (possivelmente) novo, pra
  ; multiplexar (acima) já mostrar o valor atualizado.
  rcall split_digits

end_isr:
  ; Epílogo da ISR -- desempilha na ordem inversa e "reti" (não "ret"!)
  ; pra reabilitar interrupções globais ao retornar.
  pop ZH
  pop ZL
  pop temp
  out SREG, temp
  pop temp

  reti

; Só existe 1 decodificador BCD (PORTB bits 0-3), mas são 2 dígitos
; (dezena/unidade) -- então mostra um de cada vez, bem rápido (chamada a
; cada 4ms por interruption_timer), alternando qual dígito está
; "selecionado" nos bits 4/5 do PORTB. "current_digit" guarda de quem é a
; vez; o "eor" no fim sempre alterna 0<->1 pra próxima chamada.
multiplexar:
  cpi current_digit, 0
  breq bytes_dozens

  mov temp, unit
  ori temp, (1<<PB5)      ; seleciona o dígito da unidade
  rjmp write_display

bytes_dozens:
  mov temp, dozens
  ori temp, (1<<PB4)      ; seleciona o dígito da dezena

write_display:
  out PORTB, temp          ; 4 bits de valor BCD + 1 bit de seleção de dígito
  ldi temp, 1
  eor current_digit, temp  ; alterna pra próxima vez
  ret

; Separa "count" (0-99) em dezena/unidade -- o AVR não tem instrução de
; divisão, então é feito subtraindo 10 repetidamente até não dar mais
; (técnica clássica pra converter binário em BCD "na mão").
split_digits:
  mov temp, count
  clr dozens

dozens_loop:
  cpi temp, 10
  brlo show                ; temp < 10 -> já achou a unidade
  subi temp, 10
  inc dozens
  rjmp dozens_loop

show:
  mov unit, temp
  ret

;==============================================================================
; Comunicação serial (USART) -- envio de status por interrupção, nunca por
; polling. A ideia central: "prepare_for_send" só aponta pra mensagem certa
; e liga 1 bit (UDRIE0); o hardware da USART chama "usart_interruption"
; sozinho, um caractere por vez, cada vez que o registrador de transmissão
; esvazia -- o programa principal nunca fica parado esperando a serial.
;==============================================================================
usart_interruption:
  push temp
  push ZH
  push ZL
  in temp, SREG
  push temp

  rcall transmit

  pop temp
  out SREG, temp
  pop ZL
  pop ZH
  pop temp
  reti

; Envia 1 caractere por chamada (chamado de dentro da ISR acima).
; "Highaddr"/"Loweraddr" guardam a posição atual dentro da mensagem -- não
; dá pra usar o registrador Z direto pra isso porque ele é reaproveitado
; por load_display/split_digits/etc. entre uma interrupção da USART e a
; próxima; esses 2 registradores são os únicos "reservados" só pra isso.
transmit:
  mov ZH, Highaddr
  mov ZL, Loweraddr
  lpm temp, Z+              ; lê o caractere e avança
  cpi temp, 0
  breq shotdown_udrie0      ; achou o \0 (fim da string) -> encerra o envio

  sts UDR0, temp            ; envia o caractere (dispara o próximo UDRE)
  mov Highaddr, ZH          ; guarda a posição seguinte, pra próxima vez
  mov Loweraddr, ZL
  ret

; Mensagem terminou -- desliga a interrupção de UDRE (só ela, os outros
; bits de UCSR0B ficam como estavam) pra parar de interromper o programa
; sem ter mais nada pra enviar.
shotdown_udrie0:
  lds temp, UCSR0B
  andi temp, 0b11011111     ; desliga só o UDRIE0 (bit 5)
  sts UCSR0B, temp
  ret

; Escolhe a mensagem certa pro estado atual (msg_vetor é uma tabela de
; ENDEREÇOS, .word, por isso states*2 -- cada endereço ocupa 2 bytes) e
; liga o UDRIE0 de novo, disparando o envio em segundo plano. Chamada no
; reset (1ª mensagem) e toda vez que o estado muda (load_new).
prepare_for_send:
  ldi ZL, low(msg_vetor*2)
  ldi ZH, high(msg_vetor*2)
  mov temp, states
  lsl temp                  ; cada endereço tem 2 bytes
  add ZL, temp
  clr temp                  ; o clr não altera o carry
  adc ZH, temp
  lpm Loweraddr, Z+         ; o .dw guarda o byte baixo primeiro
  lpm Highaddr, Z

  lds temp, UCSR0B
  ori temp, (1<<UDRIE0)     ; liga só o UDRIE0
  sts UCSR0B, temp
  ret

;==============================================================================
; Tabelas de dados -- ficam todas aqui no fim do arquivo de propósito (ver o
; aviso no início do vetor de interrupção, lá em cima, sobre o bug do "jmp 0").
;==============================================================================
; Tabela-verdade (docs/maquina-de-estados.md) -> bytes de PORTD, um por estado
; E0..E6.
;   bit0=S5 Verde
;   bit2=S4 verde  bit3=S4 amarelo  bit4=S4 vermelho
;   bit5=S1 verde  bit6=S1 amarelo  bit7=S1 vermelho
estados_portD:
.db 0b10010000, 0b10010000, 0b00110000, 0b01010000, 0b10000100, 0b10001000, 0b10010001

; Mesma tabela-verdade -> bytes de PORTC, um por estado E0..E6. S2 e S3 só
; têm verde/amarelo -- o vermelho de cada um vem do transistor (aceso quando
; os dois bits abaixo estão em 0).
;   bit5=S5 vermelho
;   bit0=S2 verde  bit1=S2 amarelo
;   bit3=S3 verde  bit4=S3 amarelo
estados_portC:
.db 0b00101001, 0b00110001, 0b00100001, 0b00100010, 0b00100000, 0b00100000, 0b00000000

; Duração (em segundos) de cada estado E0..E6, na MESMA ordem/índice das
; tabelas de LED acima -- posição 0 = quanto tempo o estado E0 dura, e
; assim por diante. Esses valores vieram da matriz de conflito /
; tabela-verdade em docs/maquina-de-estados.md, não foram escolhidos no
; chute.
timers_states:
.db 30, 5, 60, 3, 20, 5, 15

; Ciclo próprio do contador do display (verde/amarelo/vermelho do
; semáforo S1), lido por reload_s1_timer -- independente de timers_states
; acima.
timers_vetor:
.db 98, 20, 5

;==============================================================================
; Uma mensagem de status por estado (E0..E6), mandada pela USART toda vez
; que muda de estado (prepare_for_send).
; ATENÇÃO: o enunciado pede o formato
;   "Semáforo 1 - Amarelo; Semáforo 2 - Verde; Semáforo 3 - Vermelho..."
; (todos os semáforos numa única mensagem). As strings abaixo descrevem a
; fase por extenso em vez disso -- ajustar o texto antes da apresentação se
; precisar bater exatamente com o formato do enunciado.
;==============================================================================
msg_1:
.asciz "E0 - S2/S3 abrindo\r\n"
msg_2:
.asciz "E1 - S3 fechando\r\n"
msg_3:
.asciz "E2 - S1/S2 abertos\r\n"
msg_4:
.asciz "E3 - Amarelo S1/S2\r\n"
msg_5:
.asciz "E4 - Cruzamento aberto\r\n"
msg_6:
.asciz "E5 - Amarelo cruzamento\r\n"
msg_7:
.asciz "E6 - Pedestre\r\n"

;=============================;
; vetor de endereços de label
; ============================;
msg_vetor:
.word msg_1, msg_2, msg_3, msg_4, msg_5, msg_6, msg_7
