;
; SerialCommunication_UpperCaseConvert.asm
;
; Created: 14/03/2017 21:20:26
; Author : Erick
;Program 7.1 - Serial Echo
;Illustrate serial communications
;Platform: Arduino-UNO
;Device: ATMega328P
;Serial communications between the STK and PC
;Echo received characters (converting to uppercase)
;Receive error displayed on LED�s

;16 mhz clock speed, 9600 baud UBRR = 103
.equ UBRRvalue = 103
.def temp = r16
.cseg

;Stack initialization
ldi temp, low(RAMEND)
out SPL, temp
ldi temp, high(RAMEND)
out SPH, temp

;leds display RXD counter or receive error flags
ldi temp, $FF
out DDRB, temp
out PORTB, temp ;initially clear

;initialize USART
ldi temp, high (UBRRvalue) ;baud rate
sts UBRR0H, temp
ldi temp, low (UBRRvalue)
sts UBRR0L, temp

;URSEL 0 = UBRRH, 1 = UCSRC (shared port address)
;UMSEL 0 = Asynchronous, 1 = Synchronous
;USBS 0 = One stop bit, 1 = Two stop bits
;UCSZ0:1 Character Size: 0 = 5, 1 = 6, 2 = 7, 3 = 8
;UPM0:1 0 = none, 1 = reserved, 2 = Even, 3 = Odd

;8 bits, 1 stop, no parity
ldi temp, (3<<UCSZ00)
sts UCSR0C, temp
ldi temp, (1<<RXEN0)|(1<<TXEN0)
sts UCSR0B, temp; enable receive and transmit

;USART initialization complete
.def rxCount = r0 ;holds the complemented receive count
;this is stored in 1�s complement for easy display
;on LED�s
clr rxCount
com rxCount ;initial value is ~0


.org URXCAddr


;loop until a byte is received
;lp:
;rcall receive	;poll receiver for byte
;brcc lp			;carry set indicates byte received
	
	;count received byte and display receive count
;dec rxCount ;subtract 1 since this is the complemented count
;out PORTB, rxCount

	;r25 holds error flags - Check if an error occurred
	;Mask out non-error flags
;andi r25, (1<<FE0) | (1<<DOR0) | (1<<UPE0)
;brne receiveError ;halt program on error
;rcall toUpperCase ;convert letters to uppercase
;rcall transmit ;echo converted character
;rjmp lp
	
;Receive error - display error message on LEDs
;r25 has error codes
; bit 2 = PE
; 3 = DOR
; 4 = FE
receiveError:
	com r25; display error code
	out PORTB, r25
	rjmp pc ;halt execution
	
;Function definitions��
;Translate lowercase characters to uppercase
;Character is in r24, return value is r24
.def theByte = r24
toUpperCase:
	cpi theByte, 'a'	;screen out non-lowercase
	brlo toUpperCase_ret
	cpi theByte, 'z' + 1
	brsh toUpperCase_ret
	andi theByte, ~('A'^'a') ;only alter lowercase letters

toUpperCase_ret:
	ret
.undef theByte
	
;Receive byte - non-blocking implementation
;return the byte received (if any) in r24
;return error flags in r25
;return with carry clear if no byte was ready
.def rec_byte = r24
.def rec_err = r25

interruption_usart:
	;count received byte and display receive count
	push temp
	ldi temp, SREG
	push temp

	rcall received
	dec rxCount ;subtract 1 since this is the complemented count
	out PORTB, rxCount

	; r25 holds error flags - Check if an error occurred
	; Mask out non-error flags
	andi r25, (1<<FE0) | (1<<DOR0) | (1<<UPE0)
	brne receiveError ;halt program on error
	rcall toUpperCase ;convert letters to uppercase
	rcall transmit ;echo converted character

received:
	push temp
	ldi temp, SREG
	push temp

	lds r17, UCSR0A
	sbrs r17, RXC0		;is byte in Rx buffer?
	ret					;not yet
	lds rec_err, UCSR0A	;error flags
	lds rec_byte, UDR0	;received byte
	sec					;to indicate a byte was received
	ret
.undef rec_byte
.undef rec_err

;Transmit byte - blocks until transmit buffer can accept a byte
;The param, byte to transmit, is in r24
.def byte_tx = r24
transmit:
	lds r17, ucsr0a
	sbrs r17, udre0		;wait for tx buffer to be emptyrjmp transmit ;not ready yet
	rjmp transmit
	sts udr0, byte_tx	;transmit character
	ret
.undef byte_tx