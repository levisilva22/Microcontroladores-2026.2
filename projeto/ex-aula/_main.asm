	rjmp RESET ;Redirecionando sinal de reset
RESET:
	ldi r16, 0b11111111 ;r16 = 11111111
	out DDRB, r16 ;configura PORTB como saída
loopit:
	out PORTB, r16
	rjmp loopit
