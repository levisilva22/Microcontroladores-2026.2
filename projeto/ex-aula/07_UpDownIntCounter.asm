;
; UpDownInterruptCounter.asm
;
; Created: 20/02/2017 16:23:03
; Author : Erick
;


;Counter value is displayed on LEDS
;External interrupts are used to change the counter
;value
;LED's on Port B
;Switches on Port D (only switches 2 and 3 are used)
;Up/down counter, interrupt controlled
.def count = r16 ;count is changed via interrupt handlers

;Interrupt Vector Table (IVT)
jmp reset
jmp countup ;int0
jmp countdown ;int1

;Program code overlaps the rest of the IVT as it is not
;used
.def temp = r17
countup: ;int0 handler
	inc count
	reti

countdown: ;int1 handler
	dec count
	reti

reset:
	ldi temp, high(RAMEND) ;stack initialization required for interrupts
	out SPH, temp
	ldi temp, low(RAMEND)
	out SPL, temp
	
	;configure INT0 and INT1 sense
	ldi temp, (0b11 << ISC10) | (0b11 << ISC00) ;positive edge triggers
	sts EICRA, temp
	;enable int0, int1
	ldi temp, (1 << INT0) | (1 << INT1)
	out EIMSK, temp
	ldi temp, $FF ;set PORTB for output
	out DDRB, temp
	clr count ;initialize count
	sei ;enabled interrupts can occur now
	lp:
		cli ; disable interrupts for this critical section
		out PORTB, count
		sei ;enable interrupts after next instruction
		rjmp lp