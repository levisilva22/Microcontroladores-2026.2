.include "m328Pdef.inc"

.def count = r18
.def temp0 = r16
.def temp1 = r17

; vetor de interrupção
.org 0x0000
jmp reset
jmp press_button ; INT0

; Resto do vetor não é usado -> o código pode ocupar esse espaço
; Tabela de conversão 0x0-0xF -> padrão do display de 7 segmentos
; (cátodo comum, segmento aceso = 1; bit0=a, bit1=b, ..., bit6=g, bit7=dp)
sevenseg_table:
.db 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71

reset:
  ; iniciar a pilha
  ldi temp0, high(RAMEND)
  out SPH, temp0
  ldi temp0, low(RAMEND)
  out SPL, temp0

  ; Configurando interrupção externa pino INT0
  ldi temp0, (0b11 << ISC00) ;positive edge triggers
  sts EICRA, temp0
  ;enable int0
  ldi temp0, (1 << INT0)
  out EIMSK, temp0

  ldi temp0, 0b11111011 ;PORTD como saída pro display, exceto PD2 (botão/INT0) como entrada
  out DDRD, temp0

  clr count ;zera o contador
  rcall mostra_count ;mostra 0 no display antes do primeiro clique

  sei ;enabled interrupts can occur now

loop:
  rjmp loop ;espera as interrupções do botão

press_button: ;rotina de interrupção do INT0
  push temp0
  in temp0, SREG ;salvar SREG na pilha
  push temp0

  inc count
  cpi count, 0x10 ;passou de 0xF?
  brne mostra
  clr count ;volta pra 0x0

mostra:
  rcall mostra_count

  pop temp0
  out SREG, temp0 ;restaura SREG
  pop temp0
  reti

; Le sevenseg_table[count] e escreve no PORTD
mostra_count:
  push ZL
  push ZH
  push temp1

  ldi ZL, low(sevenseg_table*2)
  ldi ZH, high(sevenseg_table*2)
  clr temp1
  add ZL, count
  adc ZH, temp1
  lpm temp1, Z
  out PORTD, temp1

  pop temp1
  pop ZH
  pop ZL
  ret
