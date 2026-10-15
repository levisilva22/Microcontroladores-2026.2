.include "m328Pdef.inc" 

.def temp = r16
.def dozens = r17
.def states = r18
.def timer = r19
.def unit = r20
.def current_digit = r21
.def count_ticks = r22
.def Highaddr = r24
.def Loweraddr = r25

.equ STATE_NUMBER = 3
.equ TICKS_PER_SECOND = 250
.equ UBRRvalue = 103


#define CLOCK 16000000 
#define DELAY 4
.equ PRESCALE = 0b100       ;/256 prescale
.equ PRESCALE_DIV = 256
.equ WGM = 0b0100           ;Waveform generation mode: CTC (TOP = OCR1A)
;Em CTC o período é (TOP + 1) ciclos do timer, por isso o "- 1"
;16e6/256 * 1 = 31250  -->  TOP = 31249
.equ TOP = ((CLOCK/PRESCALE_DIV)*DELAY + 500)/1000 - 1
.if TOP > 65535
.error "TOP is out of range"
.endif


.cseg
;=================================;
; vetor de interrupção por timers
;=================================;
.org 0x0000
jmp reset
.org OC1Aaddr
jmp interruption_timer

;================================;
; vetor de interrupção por USART
;================================;
.org UDREaddr
jmp usart_interruption

;cometar
states_vetor:
.db 0b00010000, 0b00100000, 0b01000000   ; PD4 verde, PD5 amarelo, PD6 vermelho

timers_vetor:
.db 0x32, 0x5, 0x3, 0x14, 0x5A, 0x5, 0xF

msg_1:
.asciz "Semaforo 1 - Verde\r\n"
msg_2:
.asciz "Semaforo 1 - Amarelo\r\n"
msg_3:
.asciz "Semaforo 1 - Vermelho\r\n"

msg_vetor:
.word msg_1, msg_2, msg_3

reset:
  ldi temp, high(RAMEND)
  out SPH, temp
  ldi temp, low(RAMEND)
  out SPL, temp
  
  ; defini a saída da porta D
  ; Seta a saída do led 
  ; conforme a máquina de estados
  ldi temp, 0b01110000    ; PD4, PD5 e PD6 como saída (LEDs)
  out DDRD, temp
  ldi temp, 0b00111111    ; PB0 a PB3 como saída (decodificador)
  out DDRB, temp

  ; Inicialize o USART
  ldi temp, high(UBRRvalue)
  sts UBRR0H, temp
  ldi temp, low(UBRRvalue)
  sts UBRR0L, temp

  ;8 bits, 1 stop. no parity
  ldi temp, (3<<UCSZ00)
  sts UCSR0C, temp
  ldi temp, (1<<TXEN0)
  sts UCSR0B, temp

  clr states 
  clr count_ticks
  
  clr current_digit
  rcall load_display
  
  rcall prepare_for_send
  rcall interruption_handle
  sei


loop:
  rjmp loop

load_display:
	; Seta a saída do led conforme a máquina de estados
  ldi ZL, low(states_vetor*2)
	ldi ZH, high(states_vetor*2)
	clr temp
	add ZL, states
	adc ZH, temp
	lpm temp, Z
	out PORTD, temp

	; Seta o tempo para o estado atual
	ldi ZL, low(timers_vetor*2)
	ldi ZH, high(timers_vetor*2)
	clr temp
	add ZL, states
	adc ZH, temp
	lpm timer, Z

	rcall split_digits       
	ret


interruption_handle:
  push temp
  in temp, SREG
  push temp

  
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

	;habilita a interrupção de Compare Match A
	lds temp, TIMSK1
	sbr temp, 1 << OCIE1A
	sts TIMSK1, temp

	;upper 2 bits of WGM and clock select
	ldi temp, ((WGM>> 2) << WGM12)|(PRESCALE << CS10)
	; WGM >> 2 = 0b0100 >> 2 = 0b0001
	; (WGM >> 2) << WGM12 = (0b0001 << 3) = 0b0001000
	; (PRESCALE << CS10) = 0b100 << 0 = 0b100
	; 0b0001000 | 0b100 = 0b0001100
	sts TCCR1B, temp ;start counter
	
  pop temp
  out SREG, temp
  pop temp

  ret

changed_states:
  inc states
  cpi states, STATE_NUMBER
  brne load_new
  clr states

load_new:
  rcall load_display
  rcall prepare_for_send
  ret


interruption_timer:
  ; salvar registrador temp
  ; salvar SREG
  ; Desencrementar Timer
  ; Verificar se chegou em zero
  ; branch se chegou
  ; ler os dois vetores e escrever o
  ; padrao na PORTD
  ; carregar a nova duração do timer
  ; atualizar o display com o valor atual
  ; restaurar pilha 
  ; reti

  push temp
  in temp, SREG
  push temp
  push ZL
  push ZH

  ; mostra um dos displays ligado
  rcall multiplexar

  ; Contar os ticks a cada 1s
  inc count_ticks
  cpi count_ticks, TICKS_PER_SECOND
  brne end_isr
  
  clr count_ticks
  dec timer
  brne update_digits
  

  rcall changed_states
  rjmp end_isr

update_digits:
  rcall split_digits

end_isr:
  pop ZH
  pop ZL
  pop temp
  out SREG, temp
  pop temp
  
  reti

multiplexar:
  cpi current_digit, 0
  breq bytes_dozens

  mov temp, unit
  ori temp, (1<<PB5)
  rjmp write_display
  
bytes_dozens:
  mov temp, dozens
  ori temp, (1<<PB4)
  
write_display:
  out PORTB, temp
  ldi temp, 1
  eor current_digit, temp
  ret

split_digits:
  mov temp, timer
  clr dozens

dozens_loop:
  cpi temp, 10
  brlo show
  subi temp, 10
  inc dozens
  rjmp dozens_loop

show:
  mov unit, temp
  ret

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

transmit:
  mov ZH, Highaddr
  mov ZL, Loweraddr
  lpm temp, Z+              ; lê o caractere e avança
  cpi temp, 0
  breq shotdown_udrie0      ; fim da string

  sts UDR0, temp            ; envia
  mov Highaddr, ZH          ; guarda a posição seguinte
  mov Loweraddr, ZL
  ret

shotdown_udrie0:
  lds temp, UCSR0B
  andi temp, 0b11011111     ; desliga só o UDRIE0 (bit 5)
  sts UCSR0B, temp
  ret

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