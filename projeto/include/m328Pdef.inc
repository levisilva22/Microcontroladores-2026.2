; m328Pdef.inc convertido do pack oficial Microchip ATmega_DFP 3.6.299
; (sintaxe adaptada para o GNU assembler; nomes de registradores/bits identicos)
;***** THIS IS A MACHINE GENERATED FILE - DO NOT EDIT ********************
;***** Created: 2011-02-09 12:03 ******* Source: ATmega328P.xml **********
;*************************************************************************
;* A P P L I C A T I O N   N O T E   F O R   T H E   A V R   F A M I L Y
;* 
;* Number            : AVR000
;* File Name         : "m328Pdef.inc"
;* Title             : Register/Bit Definitions for the ATmega328P
;* Date              : 2011-02-09
;* Version           : 2.35
;* Support E-mail    : avr@atmel.com
;* Target MCU        : ATmega328P
;* 
;* DESCRIPTION
;* When including this file in the assembly program file, all I/O register 
;* names and I/O register bit names appearing in the data book can be used.
;* In addition, the six registers forming the three data pointers X, Y and 
;* Z have been assigned names XL - ZH. Highest RAM address for Internal 
;* SRAM is also defined 
;* 
;* The Register names are represented by their hexadecimal address.
;* 
;* The Register Bit names are represented by their bit number (0-7).
;* 
;* Please observe the difference in using the bit names with instructions
;* such as "sbr"/"cbr" (set/clear bit in register) and "sbrs"/"sbrc"
;* (skip if bit in register set/cleared). The following example illustrates
;* this:
;* 
;* in    r16,PORTB             ;read PORTB latch
;* sbr   r16,(1<<PB6)+(1<<PB5) ;set PB6 and PB5 (use masks, not bit#)
;* out   PORTB,r16             ;output to PORTB
;* 
;* in    r16,TIFR              ;read the Timer Interrupt Flag Register
;* sbrc  r16,TOV0              ;test the overflow flag (use bit#)
;* rjmp  TOV0_is_set           ;jump if set
;* ...                         ;otherwise do something else
;*************************************************************************

#ifndef _M328PDEF_INC_
#define _M328PDEF_INC_


#pragma partinc 0

; ***** SPECIFY DEVICE ***************************************************
; .device ATmega328P
#pragma AVRPART ADMIN PART_NAME ATmega328P
.equ SIGNATURE_000, 0x1e
.equ SIGNATURE_001, 0x95
.equ SIGNATURE_002, 0x0f

#pragma AVRPART CORE CORE_VERSION V2E


; ***** I/O REGISTER DEFINITIONS *****************************************
; NOTE:
; Definitions marked "MEMORY MAPPED"are extended I/O ports
; and cannot be used with IN/OUT instructions
.equ UDR0, 0xc6	; MEMORY MAPPED
.equ UBRR0L, 0xc4	; MEMORY MAPPED
.equ UBRR0H, 0xc5	; MEMORY MAPPED
.equ UCSR0C, 0xc2	; MEMORY MAPPED
.equ UCSR0B, 0xc1	; MEMORY MAPPED
.equ UCSR0A, 0xc0	; MEMORY MAPPED
.equ TWAMR, 0xbd	; MEMORY MAPPED
.equ TWCR, 0xbc	; MEMORY MAPPED
.equ TWDR, 0xbb	; MEMORY MAPPED
.equ TWAR, 0xba	; MEMORY MAPPED
.equ TWSR, 0xb9	; MEMORY MAPPED
.equ TWBR, 0xb8	; MEMORY MAPPED
.equ ASSR, 0xb6	; MEMORY MAPPED
.equ OCR2B, 0xb4	; MEMORY MAPPED
.equ OCR2A, 0xb3	; MEMORY MAPPED
.equ TCNT2, 0xb2	; MEMORY MAPPED
.equ TCCR2B, 0xb1	; MEMORY MAPPED
.equ TCCR2A, 0xb0	; MEMORY MAPPED
.equ OCR1BL, 0x8a	; MEMORY MAPPED
.equ OCR1BH, 0x8b	; MEMORY MAPPED
.equ OCR1AL, 0x88	; MEMORY MAPPED
.equ OCR1AH, 0x89	; MEMORY MAPPED
.equ ICR1L, 0x86	; MEMORY MAPPED
.equ ICR1H, 0x87	; MEMORY MAPPED
.equ TCNT1L, 0x84	; MEMORY MAPPED
.equ TCNT1H, 0x85	; MEMORY MAPPED
.equ TCCR1C, 0x82	; MEMORY MAPPED
.equ TCCR1B, 0x81	; MEMORY MAPPED
.equ TCCR1A, 0x80	; MEMORY MAPPED
.equ DIDR1, 0x7f	; MEMORY MAPPED
.equ DIDR0, 0x7e	; MEMORY MAPPED
.equ ADMUX, 0x7c	; MEMORY MAPPED
.equ ADCSRB, 0x7b	; MEMORY MAPPED
.equ ADCSRA, 0x7a	; MEMORY MAPPED
.equ ADCH, 0x79	; MEMORY MAPPED
.equ ADCL, 0x78	; MEMORY MAPPED
.equ TIMSK2, 0x70	; MEMORY MAPPED
.equ TIMSK1, 0x6f	; MEMORY MAPPED
.equ TIMSK0, 0x6e	; MEMORY MAPPED
.equ PCMSK1, 0x6c	; MEMORY MAPPED
.equ PCMSK2, 0x6d	; MEMORY MAPPED
.equ PCMSK0, 0x6b	; MEMORY MAPPED
.equ EICRA, 0x69	; MEMORY MAPPED
.equ PCICR, 0x68	; MEMORY MAPPED
.equ OSCCAL, 0x66	; MEMORY MAPPED
.equ PRR, 0x64	; MEMORY MAPPED
.equ CLKPR, 0x61	; MEMORY MAPPED
.equ WDTCSR, 0x60	; MEMORY MAPPED
.equ SREG, 0x3f
.equ SPL, 0x3d
.equ SPH, 0x3e
.equ SPMCSR, 0x37
.equ MCUCR, 0x35
.equ MCUSR, 0x34
.equ SMCR, 0x33
.equ ACSR, 0x30
.equ SPDR, 0x2e
.equ SPSR, 0x2d
.equ SPCR, 0x2c
.equ GPIOR2, 0x2b
.equ GPIOR1, 0x2a
.equ OCR0B, 0x28
.equ OCR0A, 0x27
.equ TCNT0, 0x26
.equ TCCR0B, 0x25
.equ TCCR0A, 0x24
.equ GTCCR, 0x23
.equ EEARH, 0x22
.equ EEARL, 0x21
.equ EEDR, 0x20
.equ EECR, 0x1f
.equ GPIOR0, 0x1e
.equ EIMSK, 0x1d
.equ EIFR, 0x1c
.equ PCIFR, 0x1b
.equ TIFR2, 0x17
.equ TIFR1, 0x16
.equ TIFR0, 0x15
.equ PORTD, 0x0b
.equ DDRD, 0x0a
.equ PIND, 0x09
.equ PORTC, 0x08
.equ DDRC, 0x07
.equ PINC, 0x06
.equ PORTB, 0x05
.equ DDRB, 0x04
.equ PINB, 0x03


; ***** BIT DEFINITIONS **************************************************

; ***** USART0 ***********************
; UDR0 - USART I/O Data Register
.equ UDR0_0, 0	; USART I/O Data Register bit 0
.equ UDR0_1, 1	; USART I/O Data Register bit 1
.equ UDR0_2, 2	; USART I/O Data Register bit 2
.equ UDR0_3, 3	; USART I/O Data Register bit 3
.equ UDR0_4, 4	; USART I/O Data Register bit 4
.equ UDR0_5, 5	; USART I/O Data Register bit 5
.equ UDR0_6, 6	; USART I/O Data Register bit 6
.equ UDR0_7, 7	; USART I/O Data Register bit 7

; UCSR0A - USART Control and Status Register A
.equ MPCM0, 0	; Multi-processor Communication Mode
.equ U2X0, 1	; Double the USART transmission speed
.equ UPE0, 2	; Parity Error
.equ DOR0, 3	; Data overRun
.equ FE0, 4	; Framing Error
.equ UDRE0, 5	; USART Data Register Empty
.equ TXC0, 6	; USART Transmitt Complete
.equ RXC0, 7	; USART Receive Complete

; UCSR0B - USART Control and Status Register B
.equ TXB80, 0	; Transmit Data Bit 8
.equ RXB80, 1	; Receive Data Bit 8
.equ UCSZ02, 2	; Character Size
.equ TXEN0, 3	; Transmitter Enable
.equ RXEN0, 4	; Receiver Enable
.equ UDRIE0, 5	; USART Data register Empty Interrupt Enable
.equ TXCIE0, 6	; TX Complete Interrupt Enable
.equ RXCIE0, 7	; RX Complete Interrupt Enable

; UCSR0C - USART Control and Status Register C
.equ UCPOL0, 0	; Clock Polarity
.equ UCSZ00, 1	; Character Size
.equ UCPHA0, UCSZ00	; For compatibility
.equ UCSZ01, 2	; Character Size
.equ UDORD0, UCSZ01	; For compatibility
.equ USBS0, 3	; Stop Bit Select
.equ UPM00, 4	; Parity Mode Bit 0
.equ UPM01, 5	; Parity Mode Bit 1
.equ UMSEL00, 6	; USART Mode Select
.equ UMSEL0, UMSEL00	; For compatibility
.equ UMSEL01, 7	; USART Mode Select
.equ UMSEL1, UMSEL01	; For compatibility

; UBRR0H - USART Baud Rate Register High Byte
.equ UBRR8, 0	; USART Baud Rate Register bit 8
.equ UBRR9, 1	; USART Baud Rate Register bit 9
.equ UBRR10, 2	; USART Baud Rate Register bit 10
.equ UBRR11, 3	; USART Baud Rate Register bit 11

; UBRR0L - USART Baud Rate Register Low Byte
.equ _UBRR0, 0	; USART Baud Rate Register bit 0
.equ _UBRR1, 1	; USART Baud Rate Register bit 1
.equ UBRR2, 2	; USART Baud Rate Register bit 2
.equ UBRR3, 3	; USART Baud Rate Register bit 3
.equ UBRR4, 4	; USART Baud Rate Register bit 4
.equ UBRR5, 5	; USART Baud Rate Register bit 5
.equ UBRR6, 6	; USART Baud Rate Register bit 6
.equ UBRR7, 7	; USART Baud Rate Register bit 7


; ***** TWI **************************
; TWAMR - TWI (Slave) Address Mask Register
.equ TWAM0, 1	; 
.equ TWAMR0, TWAM0	; For compatibility
.equ TWAM1, 2	; 
.equ TWAMR1, TWAM1	; For compatibility
.equ TWAM2, 3	; 
.equ TWAMR2, TWAM2	; For compatibility
.equ TWAM3, 4	; 
.equ TWAMR3, TWAM3	; For compatibility
.equ TWAM4, 5	; 
.equ TWAMR4, TWAM4	; For compatibility
.equ TWAM5, 6	; 
.equ TWAMR5, TWAM5	; For compatibility
.equ TWAM6, 7	; 
.equ TWAMR6, TWAM6	; For compatibility

; TWBR - TWI Bit Rate register
.equ TWBR0, 0	; 
.equ TWBR1, 1	; 
.equ TWBR2, 2	; 
.equ TWBR3, 3	; 
.equ TWBR4, 4	; 
.equ TWBR5, 5	; 
.equ TWBR6, 6	; 
.equ TWBR7, 7	; 

; TWCR - TWI Control Register
.equ TWIE, 0	; TWI Interrupt Enable
.equ TWEN, 2	; TWI Enable Bit
.equ TWWC, 3	; TWI Write Collition Flag
.equ TWSTO, 4	; TWI Stop Condition Bit
.equ TWSTA, 5	; TWI Start Condition Bit
.equ TWEA, 6	; TWI Enable Acknowledge Bit
.equ TWINT, 7	; TWI Interrupt Flag

; TWSR - TWI Status Register
.equ TWPS0, 0	; TWI Prescaler
.equ TWPS1, 1	; TWI Prescaler
.equ TWS3, 3	; TWI Status
.equ TWS4, 4	; TWI Status
.equ TWS5, 5	; TWI Status
.equ TWS6, 6	; TWI Status
.equ TWS7, 7	; TWI Status

; TWDR - TWI Data register
.equ TWD0, 0	; TWI Data Register Bit 0
.equ TWD1, 1	; TWI Data Register Bit 1
.equ TWD2, 2	; TWI Data Register Bit 2
.equ TWD3, 3	; TWI Data Register Bit 3
.equ TWD4, 4	; TWI Data Register Bit 4
.equ TWD5, 5	; TWI Data Register Bit 5
.equ TWD6, 6	; TWI Data Register Bit 6
.equ TWD7, 7	; TWI Data Register Bit 7

; TWAR - TWI (Slave) Address register
.equ TWGCE, 0	; TWI General Call Recognition Enable Bit
.equ TWA0, 1	; TWI (Slave) Address register Bit 0
.equ TWA1, 2	; TWI (Slave) Address register Bit 1
.equ TWA2, 3	; TWI (Slave) Address register Bit 2
.equ TWA3, 4	; TWI (Slave) Address register Bit 3
.equ TWA4, 5	; TWI (Slave) Address register Bit 4
.equ TWA5, 6	; TWI (Slave) Address register Bit 5
.equ TWA6, 7	; TWI (Slave) Address register Bit 6


; ***** TIMER_COUNTER_1 **************
; TIMSK1 - Timer/Counter Interrupt Mask Register
.equ TOIE1, 0	; Timer/Counter1 Overflow Interrupt Enable
.equ OCIE1A, 1	; Timer/Counter1 Output CompareA Match Interrupt Enable
.equ OCIE1B, 2	; Timer/Counter1 Output CompareB Match Interrupt Enable
.equ ICIE1, 5	; Timer/Counter1 Input Capture Interrupt Enable

; TIFR1 - Timer/Counter Interrupt Flag register
.equ TOV1, 0	; Timer/Counter1 Overflow Flag
.equ OCF1A, 1	; Output Compare Flag 1A
.equ OCF1B, 2	; Output Compare Flag 1B
.equ ICF1, 5	; Input Capture Flag 1

; TCCR1A - Timer/Counter1 Control Register A
.equ WGM10, 0	; Waveform Generation Mode
.equ WGM11, 1	; Waveform Generation Mode
.equ COM1B0, 4	; Compare Output Mode 1B, bit 0
.equ COM1B1, 5	; Compare Output Mode 1B, bit 1
.equ COM1A0, 6	; Comparet Ouput Mode 1A, bit 0
.equ COM1A1, 7	; Compare Output Mode 1A, bit 1

; TCCR1B - Timer/Counter1 Control Register B
.equ CS10, 0	; Prescaler source of Timer/Counter 1
.equ CS11, 1	; Prescaler source of Timer/Counter 1
.equ CS12, 2	; Prescaler source of Timer/Counter 1
.equ WGM12, 3	; Waveform Generation Mode
.equ WGM13, 4	; Waveform Generation Mode
.equ ICES1, 6	; Input Capture 1 Edge Select
.equ ICNC1, 7	; Input Capture 1 Noise Canceler

; TCCR1C - Timer/Counter1 Control Register C
.equ FOC1B, 6	; 
.equ FOC1A, 7	; 

; GTCCR - General Timer/Counter Control Register
.equ PSRSYNC, 0	; Prescaler Reset Timer/Counter1 and Timer/Counter0
.equ TSM, 7	; Timer/Counter Synchronization Mode


; ***** TIMER_COUNTER_2 **************
; TIMSK2 - Timer/Counter Interrupt Mask register
.equ TOIE2, 0	; Timer/Counter2 Overflow Interrupt Enable
.equ TOIE2A, TOIE2	; For compatibility
.equ OCIE2A, 1	; Timer/Counter2 Output Compare Match A Interrupt Enable
.equ OCIE2B, 2	; Timer/Counter2 Output Compare Match B Interrupt Enable

; TIFR2 - Timer/Counter Interrupt Flag Register
.equ TOV2, 0	; Timer/Counter2 Overflow Flag
.equ OCF2A, 1	; Output Compare Flag 2A
.equ OCF2B, 2	; Output Compare Flag 2B

; TCCR2A - Timer/Counter2 Control Register A
.equ WGM20, 0	; Waveform Genration Mode
.equ WGM21, 1	; Waveform Genration Mode
.equ COM2B0, 4	; Compare Output Mode bit 0
.equ COM2B1, 5	; Compare Output Mode bit 1
.equ COM2A0, 6	; Compare Output Mode bit 1
.equ COM2A1, 7	; Compare Output Mode bit 1

; TCCR2B - Timer/Counter2 Control Register B
.equ CS20, 0	; Clock Select bit 0
.equ CS21, 1	; Clock Select bit 1
.equ CS22, 2	; Clock Select bit 2
.equ WGM22, 3	; Waveform Generation Mode
.equ FOC2B, 6	; Force Output Compare B
.equ FOC2A, 7	; Force Output Compare A

; TCNT2 - Timer/Counter2
.equ TCNT2_0, 0	; Timer/Counter 2 bit 0
.equ TCNT2_1, 1	; Timer/Counter 2 bit 1
.equ TCNT2_2, 2	; Timer/Counter 2 bit 2
.equ TCNT2_3, 3	; Timer/Counter 2 bit 3
.equ TCNT2_4, 4	; Timer/Counter 2 bit 4
.equ TCNT2_5, 5	; Timer/Counter 2 bit 5
.equ TCNT2_6, 6	; Timer/Counter 2 bit 6
.equ TCNT2_7, 7	; Timer/Counter 2 bit 7

; OCR2A - Timer/Counter2 Output Compare Register A
.equ OCR2A_0, 0	; Timer/Counter2 Output Compare Register Bit 0
.equ OCR2A_1, 1	; Timer/Counter2 Output Compare Register Bit 1
.equ OCR2A_2, 2	; Timer/Counter2 Output Compare Register Bit 2
.equ OCR2A_3, 3	; Timer/Counter2 Output Compare Register Bit 3
.equ OCR2A_4, 4	; Timer/Counter2 Output Compare Register Bit 4
.equ OCR2A_5, 5	; Timer/Counter2 Output Compare Register Bit 5
.equ OCR2A_6, 6	; Timer/Counter2 Output Compare Register Bit 6
.equ OCR2A_7, 7	; Timer/Counter2 Output Compare Register Bit 7

; OCR2B - Timer/Counter2 Output Compare Register B
.equ OCR2B_0, 0	; Timer/Counter2 Output Compare Register Bit 0
.equ OCR2B_1, 1	; Timer/Counter2 Output Compare Register Bit 1
.equ OCR2B_2, 2	; Timer/Counter2 Output Compare Register Bit 2
.equ OCR2B_3, 3	; Timer/Counter2 Output Compare Register Bit 3
.equ OCR2B_4, 4	; Timer/Counter2 Output Compare Register Bit 4
.equ OCR2B_5, 5	; Timer/Counter2 Output Compare Register Bit 5
.equ OCR2B_6, 6	; Timer/Counter2 Output Compare Register Bit 6
.equ OCR2B_7, 7	; Timer/Counter2 Output Compare Register Bit 7

; ASSR - Asynchronous Status Register
.equ TCR2BUB, 0	; Timer/Counter Control Register2 Update Busy
.equ TCR2AUB, 1	; Timer/Counter Control Register2 Update Busy
.equ OCR2BUB, 2	; Output Compare Register 2 Update Busy
.equ OCR2AUB, 3	; Output Compare Register2 Update Busy
.equ TCN2UB, 4	; Timer/Counter2 Update Busy
.equ AS2, 5	; Asynchronous Timer/Counter2
.equ EXCLK, 6	; Enable External Clock Input

; GTCCR - General Timer Counter Control register
.equ PSRASY, 1	; Prescaler Reset Timer/Counter2
.equ PSR2, PSRASY	; For compatibility
;.equ	TSM	= 7	; Timer/Counter Synchronization Mode


; ***** AD_CONVERTER *****************
; ADMUX - The ADC multiplexer Selection Register
.equ MUX0, 0	; Analog Channel and Gain Selection Bits
.equ MUX1, 1	; Analog Channel and Gain Selection Bits
.equ MUX2, 2	; Analog Channel and Gain Selection Bits
.equ MUX3, 3	; Analog Channel and Gain Selection Bits
.equ ADLAR, 5	; Left Adjust Result
.equ REFS0, 6	; Reference Selection Bit 0
.equ REFS1, 7	; Reference Selection Bit 1

; ADCSRA - The ADC Control and Status register A
.equ ADPS0, 0	; ADC  Prescaler Select Bits
.equ ADPS1, 1	; ADC  Prescaler Select Bits
.equ ADPS2, 2	; ADC  Prescaler Select Bits
.equ ADIE, 3	; ADC Interrupt Enable
.equ ADIF, 4	; ADC Interrupt Flag
.equ ADATE, 5	; ADC  Auto Trigger Enable
.equ ADSC, 6	; ADC Start Conversion
.equ ADEN, 7	; ADC Enable

; ADCSRB - The ADC Control and Status register B
.equ ADTS0, 0	; ADC Auto Trigger Source bit 0
.equ ADTS1, 1	; ADC Auto Trigger Source bit 1
.equ ADTS2, 2	; ADC Auto Trigger Source bit 2
.equ ACME, 6	; 

; ADCH - ADC Data Register High Byte
.equ ADCH0, 0	; ADC Data Register High Byte Bit 0
.equ ADCH1, 1	; ADC Data Register High Byte Bit 1
.equ ADCH2, 2	; ADC Data Register High Byte Bit 2
.equ ADCH3, 3	; ADC Data Register High Byte Bit 3
.equ ADCH4, 4	; ADC Data Register High Byte Bit 4
.equ ADCH5, 5	; ADC Data Register High Byte Bit 5
.equ ADCH6, 6	; ADC Data Register High Byte Bit 6
.equ ADCH7, 7	; ADC Data Register High Byte Bit 7

; ADCL - ADC Data Register Low Byte
.equ ADCL0, 0	; ADC Data Register Low Byte Bit 0
.equ ADCL1, 1	; ADC Data Register Low Byte Bit 1
.equ ADCL2, 2	; ADC Data Register Low Byte Bit 2
.equ ADCL3, 3	; ADC Data Register Low Byte Bit 3
.equ ADCL4, 4	; ADC Data Register Low Byte Bit 4
.equ ADCL5, 5	; ADC Data Register Low Byte Bit 5
.equ ADCL6, 6	; ADC Data Register Low Byte Bit 6
.equ ADCL7, 7	; ADC Data Register Low Byte Bit 7

; DIDR0 - Digital Input Disable Register
.equ ADC0D, 0	; 
.equ ADC1D, 1	; 
.equ ADC2D, 2	; 
.equ ADC3D, 3	; 
.equ ADC4D, 4	; 
.equ ADC5D, 5	; 


; ***** ANALOG_COMPARATOR ************
; ACSR - Analog Comparator Control And Status Register
.equ ACIS0, 0	; Analog Comparator Interrupt Mode Select bit 0
.equ ACIS1, 1	; Analog Comparator Interrupt Mode Select bit 1
.equ ACIC, 2	; Analog Comparator Input Capture Enable
.equ ACIE, 3	; Analog Comparator Interrupt Enable
.equ ACI, 4	; Analog Comparator Interrupt Flag
.equ ACO, 5	; Analog Compare Output
.equ ACBG, 6	; Analog Comparator Bandgap Select
.equ ACD, 7	; Analog Comparator Disable

; DIDR1 - Digital Input Disable Register 1
.equ AIN0D, 0	; AIN0 Digital Input Disable
.equ AIN1D, 1	; AIN1 Digital Input Disable


; ***** PORTB ************************
; PORTB - Port B Data Register
.equ PORTB0, 0	; Port B Data Register bit 0
.equ PB0, 0	; For compatibility
.equ PORTB1, 1	; Port B Data Register bit 1
.equ PB1, 1	; For compatibility
.equ PORTB2, 2	; Port B Data Register bit 2
.equ PB2, 2	; For compatibility
.equ PORTB3, 3	; Port B Data Register bit 3
.equ PB3, 3	; For compatibility
.equ PORTB4, 4	; Port B Data Register bit 4
.equ PB4, 4	; For compatibility
.equ PORTB5, 5	; Port B Data Register bit 5
.equ PB5, 5	; For compatibility
.equ PORTB6, 6	; Port B Data Register bit 6
.equ PB6, 6	; For compatibility
.equ PORTB7, 7	; Port B Data Register bit 7
.equ PB7, 7	; For compatibility

; DDRB - Port B Data Direction Register
.equ DDB0, 0	; Port B Data Direction Register bit 0
.equ DDB1, 1	; Port B Data Direction Register bit 1
.equ DDB2, 2	; Port B Data Direction Register bit 2
.equ DDB3, 3	; Port B Data Direction Register bit 3
.equ DDB4, 4	; Port B Data Direction Register bit 4
.equ DDB5, 5	; Port B Data Direction Register bit 5
.equ DDB6, 6	; Port B Data Direction Register bit 6
.equ DDB7, 7	; Port B Data Direction Register bit 7

; PINB - Port B Input Pins
.equ PINB0, 0	; Port B Input Pins bit 0
.equ PINB1, 1	; Port B Input Pins bit 1
.equ PINB2, 2	; Port B Input Pins bit 2
.equ PINB3, 3	; Port B Input Pins bit 3
.equ PINB4, 4	; Port B Input Pins bit 4
.equ PINB5, 5	; Port B Input Pins bit 5
.equ PINB6, 6	; Port B Input Pins bit 6
.equ PINB7, 7	; Port B Input Pins bit 7


; ***** PORTC ************************
; PORTC - Port C Data Register
.equ PORTC0, 0	; Port C Data Register bit 0
.equ PC0, 0	; For compatibility
.equ PORTC1, 1	; Port C Data Register bit 1
.equ PC1, 1	; For compatibility
.equ PORTC2, 2	; Port C Data Register bit 2
.equ PC2, 2	; For compatibility
.equ PORTC3, 3	; Port C Data Register bit 3
.equ PC3, 3	; For compatibility
.equ PORTC4, 4	; Port C Data Register bit 4
.equ PC4, 4	; For compatibility
.equ PORTC5, 5	; Port C Data Register bit 5
.equ PC5, 5	; For compatibility
.equ PORTC6, 6	; Port C Data Register bit 6
.equ PC6, 6	; For compatibility

; DDRC - Port C Data Direction Register
.equ DDC0, 0	; Port C Data Direction Register bit 0
.equ DDC1, 1	; Port C Data Direction Register bit 1
.equ DDC2, 2	; Port C Data Direction Register bit 2
.equ DDC3, 3	; Port C Data Direction Register bit 3
.equ DDC4, 4	; Port C Data Direction Register bit 4
.equ DDC5, 5	; Port C Data Direction Register bit 5
.equ DDC6, 6	; Port C Data Direction Register bit 6

; PINC - Port C Input Pins
.equ PINC0, 0	; Port C Input Pins bit 0
.equ PINC1, 1	; Port C Input Pins bit 1
.equ PINC2, 2	; Port C Input Pins bit 2
.equ PINC3, 3	; Port C Input Pins bit 3
.equ PINC4, 4	; Port C Input Pins bit 4
.equ PINC5, 5	; Port C Input Pins bit 5
.equ PINC6, 6	; Port C Input Pins bit 6


; ***** PORTD ************************
; PORTD - Port D Data Register
.equ PORTD0, 0	; Port D Data Register bit 0
.equ PD0, 0	; For compatibility
.equ PORTD1, 1	; Port D Data Register bit 1
.equ PD1, 1	; For compatibility
.equ PORTD2, 2	; Port D Data Register bit 2
.equ PD2, 2	; For compatibility
.equ PORTD3, 3	; Port D Data Register bit 3
.equ PD3, 3	; For compatibility
.equ PORTD4, 4	; Port D Data Register bit 4
.equ PD4, 4	; For compatibility
.equ PORTD5, 5	; Port D Data Register bit 5
.equ PD5, 5	; For compatibility
.equ PORTD6, 6	; Port D Data Register bit 6
.equ PD6, 6	; For compatibility
.equ PORTD7, 7	; Port D Data Register bit 7
.equ PD7, 7	; For compatibility

; DDRD - Port D Data Direction Register
.equ DDD0, 0	; Port D Data Direction Register bit 0
.equ DDD1, 1	; Port D Data Direction Register bit 1
.equ DDD2, 2	; Port D Data Direction Register bit 2
.equ DDD3, 3	; Port D Data Direction Register bit 3
.equ DDD4, 4	; Port D Data Direction Register bit 4
.equ DDD5, 5	; Port D Data Direction Register bit 5
.equ DDD6, 6	; Port D Data Direction Register bit 6
.equ DDD7, 7	; Port D Data Direction Register bit 7

; PIND - Port D Input Pins
.equ PIND0, 0	; Port D Input Pins bit 0
.equ PIND1, 1	; Port D Input Pins bit 1
.equ PIND2, 2	; Port D Input Pins bit 2
.equ PIND3, 3	; Port D Input Pins bit 3
.equ PIND4, 4	; Port D Input Pins bit 4
.equ PIND5, 5	; Port D Input Pins bit 5
.equ PIND6, 6	; Port D Input Pins bit 6
.equ PIND7, 7	; Port D Input Pins bit 7


; ***** TIMER_COUNTER_0 **************
; TIMSK0 - Timer/Counter0 Interrupt Mask Register
.equ TOIE0, 0	; Timer/Counter0 Overflow Interrupt Enable
.equ OCIE0A, 1	; Timer/Counter0 Output Compare Match A Interrupt Enable
.equ OCIE0B, 2	; Timer/Counter0 Output Compare Match B Interrupt Enable

; TIFR0 - Timer/Counter0 Interrupt Flag register
.equ TOV0, 0	; Timer/Counter0 Overflow Flag
.equ OCF0A, 1	; Timer/Counter0 Output Compare Flag 0A
.equ OCF0B, 2	; Timer/Counter0 Output Compare Flag 0B

; TCCR0A - Timer/Counter  Control Register A
.equ WGM00, 0	; Waveform Generation Mode
.equ WGM01, 1	; Waveform Generation Mode
.equ COM0B0, 4	; Compare Output Mode, Fast PWm
.equ COM0B1, 5	; Compare Output Mode, Fast PWm
.equ COM0A0, 6	; Compare Output Mode, Phase Correct PWM Mode
.equ COM0A1, 7	; Compare Output Mode, Phase Correct PWM Mode

; TCCR0B - Timer/Counter Control Register B
.equ CS00, 0	; Clock Select
.equ CS01, 1	; Clock Select
.equ CS02, 2	; Clock Select
.equ WGM02, 3	; 
.equ FOC0B, 6	; Force Output Compare B
.equ FOC0A, 7	; Force Output Compare A

; TCNT0 - Timer/Counter0
.equ TCNT0_0, 0	; 
.equ TCNT0_1, 1	; 
.equ TCNT0_2, 2	; 
.equ TCNT0_3, 3	; 
.equ TCNT0_4, 4	; 
.equ TCNT0_5, 5	; 
.equ TCNT0_6, 6	; 
.equ TCNT0_7, 7	; 

; OCR0A - Timer/Counter0 Output Compare Register
.equ OCR0A_0, 0	; 
.equ OCR0A_1, 1	; 
.equ OCR0A_2, 2	; 
.equ OCR0A_3, 3	; 
.equ OCR0A_4, 4	; 
.equ OCR0A_5, 5	; 
.equ OCR0A_6, 6	; 
.equ OCR0A_7, 7	; 

; OCR0B - Timer/Counter0 Output Compare Register
.equ OCR0B_0, 0	; 
.equ OCR0B_1, 1	; 
.equ OCR0B_2, 2	; 
.equ OCR0B_3, 3	; 
.equ OCR0B_4, 4	; 
.equ OCR0B_5, 5	; 
.equ OCR0B_6, 6	; 
.equ OCR0B_7, 7	; 

; GTCCR - General Timer/Counter Control Register
;.equ	PSRSYNC	= 0	; Prescaler Reset Timer/Counter1 and Timer/Counter0
.equ PSR10, PSRSYNC	; For compatibility
;.equ	TSM	= 7	; Timer/Counter Synchronization Mode


; ***** EXTERNAL_INTERRUPT ***********
; EICRA - External Interrupt Control Register
.equ ISC00, 0	; External Interrupt Sense Control 0 Bit 0
.equ ISC01, 1	; External Interrupt Sense Control 0 Bit 1
.equ ISC10, 2	; External Interrupt Sense Control 1 Bit 0
.equ ISC11, 3	; External Interrupt Sense Control 1 Bit 1

; EIMSK - External Interrupt Mask Register
.equ INT0, 0	; External Interrupt Request 0 Enable
.equ INT1, 1	; External Interrupt Request 1 Enable

; EIFR - External Interrupt Flag Register
.equ INTF0, 0	; External Interrupt Flag 0
.equ INTF1, 1	; External Interrupt Flag 1

; PCICR - Pin Change Interrupt Control Register
.equ PCIE0, 0	; Pin Change Interrupt Enable 0
.equ PCIE1, 1	; Pin Change Interrupt Enable 1
.equ PCIE2, 2	; Pin Change Interrupt Enable 2

; PCMSK2 - Pin Change Mask Register 2
.equ PCINT16, 0	; Pin Change Enable Mask 16
.equ PCINT17, 1	; Pin Change Enable Mask 17
.equ PCINT18, 2	; Pin Change Enable Mask 18
.equ PCINT19, 3	; Pin Change Enable Mask 19
.equ PCINT20, 4	; Pin Change Enable Mask 20
.equ PCINT21, 5	; Pin Change Enable Mask 21
.equ PCINT22, 6	; Pin Change Enable Mask 22
.equ PCINT23, 7	; Pin Change Enable Mask 23

; PCMSK1 - Pin Change Mask Register 1
.equ PCINT8, 0	; Pin Change Enable Mask 8
.equ PCINT9, 1	; Pin Change Enable Mask 9
.equ PCINT10, 2	; Pin Change Enable Mask 10
.equ PCINT11, 3	; Pin Change Enable Mask 11
.equ PCINT12, 4	; Pin Change Enable Mask 12
.equ PCINT13, 5	; Pin Change Enable Mask 13
.equ PCINT14, 6	; Pin Change Enable Mask 14

; PCMSK0 - Pin Change Mask Register 0
.equ PCINT0, 0	; Pin Change Enable Mask 0
.equ PCINT1, 1	; Pin Change Enable Mask 1
.equ PCINT2, 2	; Pin Change Enable Mask 2
.equ PCINT3, 3	; Pin Change Enable Mask 3
.equ PCINT4, 4	; Pin Change Enable Mask 4
.equ PCINT5, 5	; Pin Change Enable Mask 5
.equ PCINT6, 6	; Pin Change Enable Mask 6
.equ PCINT7, 7	; Pin Change Enable Mask 7

; PCIFR - Pin Change Interrupt Flag Register
.equ PCIF0, 0	; Pin Change Interrupt Flag 0
.equ PCIF1, 1	; Pin Change Interrupt Flag 1
.equ PCIF2, 2	; Pin Change Interrupt Flag 2


; ***** SPI **************************
; SPDR - SPI Data Register
.equ SPDR0, 0	; SPI Data Register bit 0
.equ SPDR1, 1	; SPI Data Register bit 1
.equ SPDR2, 2	; SPI Data Register bit 2
.equ SPDR3, 3	; SPI Data Register bit 3
.equ SPDR4, 4	; SPI Data Register bit 4
.equ SPDR5, 5	; SPI Data Register bit 5
.equ SPDR6, 6	; SPI Data Register bit 6
.equ SPDR7, 7	; SPI Data Register bit 7

; SPSR - SPI Status Register
.equ SPI2X, 0	; Double SPI Speed Bit
.equ WCOL, 6	; Write Collision Flag
.equ SPIF, 7	; SPI Interrupt Flag

; SPCR - SPI Control Register
.equ SPR0, 0	; SPI Clock Rate Select 0
.equ SPR1, 1	; SPI Clock Rate Select 1
.equ CPHA, 2	; Clock Phase
.equ CPOL, 3	; Clock polarity
.equ MSTR, 4	; Master/Slave Select
.equ DORD, 5	; Data Order
.equ SPE, 6	; SPI Enable
.equ SPIE, 7	; SPI Interrupt Enable


; ***** WATCHDOG *********************
; WDTCSR - Watchdog Timer Control Register
.equ WDP0, 0	; Watch Dog Timer Prescaler bit 0
.equ WDP1, 1	; Watch Dog Timer Prescaler bit 1
.equ WDP2, 2	; Watch Dog Timer Prescaler bit 2
.equ WDE, 3	; Watch Dog Enable
.equ WDCE, 4	; Watchdog Change Enable
.equ WDP3, 5	; Watchdog Timer Prescaler Bit 3
.equ WDIE, 6	; Watchdog Timeout Interrupt Enable
.equ WDIF, 7	; Watchdog Timeout Interrupt Flag


; ***** CPU **************************
; SREG - Status Register
.equ SREG_C, 0	; Carry Flag
.equ SREG_Z, 1	; Zero Flag
.equ SREG_N, 2	; Negative Flag
.equ SREG_V, 3	; Two's Complement Overflow Flag
.equ SREG_S, 4	; Sign Bit
.equ SREG_H, 5	; Half Carry Flag
.equ SREG_T, 6	; Bit Copy Storage
.equ SREG_I, 7	; Global Interrupt Enable

; OSCCAL - Oscillator Calibration Value
.equ CAL0, 0	; Oscillator Calibration Value Bit0
.equ CAL1, 1	; Oscillator Calibration Value Bit1
.equ CAL2, 2	; Oscillator Calibration Value Bit2
.equ CAL3, 3	; Oscillator Calibration Value Bit3
.equ CAL4, 4	; Oscillator Calibration Value Bit4
.equ CAL5, 5	; Oscillator Calibration Value Bit5
.equ CAL6, 6	; Oscillator Calibration Value Bit6
.equ CAL7, 7	; Oscillator Calibration Value Bit7

; CLKPR - Clock Prescale Register
.equ CLKPS0, 0	; Clock Prescaler Select Bit 0
.equ CLKPS1, 1	; Clock Prescaler Select Bit 1
.equ CLKPS2, 2	; Clock Prescaler Select Bit 2
.equ CLKPS3, 3	; Clock Prescaler Select Bit 3
.equ CLKPCE, 7	; Clock Prescaler Change Enable

; SPMCSR - Store Program Memory Control and Status Register
.equ SELFPRGEN, 0; Added for backwards compatibility
.equ SPMEN, 0	; Store Program Memory
.equ PGERS, 1	; Page Erase
.equ PGWRT, 2	; Page Write
.equ BLBSET, 3	; Boot Lock Bit Set
.equ RWWSRE, 4	; Read-While-Write section read enable
.equ SIGRD, 5 ; Signature Row Read
.equ RWWSB, 6	; Read-While-Write Section Busy
.equ SPMIE, 7	; SPM Interrupt Enable

; MCUCR - MCU Control Register
.equ IVCE, 0	; 
.equ IVSEL, 1	; 
.equ PUD, 4	; 
.equ BODSE, 5	; BOD Sleep Enable
.equ BODS, 6	; BOD Sleep

; MCUSR - MCU Status Register
.equ PORF, 0	; Power-on reset flag
.equ EXTRF, 1	; External Reset Flag
.equ EXTREF, EXTRF	; For compatibility
.equ BORF, 2	; Brown-out Reset Flag
.equ WDRF, 3	; Watchdog Reset Flag

; SMCR - Sleep Mode Control Register
.equ SE, 0	; Sleep Enable
.equ SM0, 1	; Sleep Mode Select Bit 0
.equ SM1, 2	; Sleep Mode Select Bit 1
.equ SM2, 3	; Sleep Mode Select Bit 2

; GPIOR2 - General Purpose I/O Register 2
.equ GPIOR20, 0	; 
.equ GPIOR21, 1	; 
.equ GPIOR22, 2	; 
.equ GPIOR23, 3	; 
.equ GPIOR24, 4	; 
.equ GPIOR25, 5	; 
.equ GPIOR26, 6	; 
.equ GPIOR27, 7	; 

; GPIOR1 - General Purpose I/O Register 1
.equ GPIOR10, 0	; 
.equ GPIOR11, 1	; 
.equ GPIOR12, 2	; 
.equ GPIOR13, 3	; 
.equ GPIOR14, 4	; 
.equ GPIOR15, 5	; 
.equ GPIOR16, 6	; 
.equ GPIOR17, 7	; 

; GPIOR0 - General Purpose I/O Register 0
.equ GPIOR00, 0	; 
.equ GPIOR01, 1	; 
.equ GPIOR02, 2	; 
.equ GPIOR03, 3	; 
.equ GPIOR04, 4	; 
.equ GPIOR05, 5	; 
.equ GPIOR06, 6	; 
.equ GPIOR07, 7	; 

; PRR - Power Reduction Register
.equ PRADC, 0	; Power Reduction ADC
.equ PRUSART0, 1	; Power Reduction USART
.equ PRSPI, 2	; Power Reduction Serial Peripheral Interface
.equ PRTIM1, 3	; Power Reduction Timer/Counter1
.equ PRTIM0, 5	; Power Reduction Timer/Counter0
.equ PRTIM2, 6	; Power Reduction Timer/Counter2
.equ PRTWI, 7	; Power Reduction TWI


; ***** EEPROM ***********************
; EEARL - EEPROM Address Register Low Byte
.equ EEAR0, 0	; EEPROM Read/Write Access Bit 0
.equ EEAR1, 1	; EEPROM Read/Write Access Bit 1
.equ EEAR2, 2	; EEPROM Read/Write Access Bit 2
.equ EEAR3, 3	; EEPROM Read/Write Access Bit 3
.equ EEAR4, 4	; EEPROM Read/Write Access Bit 4
.equ EEAR5, 5	; EEPROM Read/Write Access Bit 5
.equ EEAR6, 6	; EEPROM Read/Write Access Bit 6
.equ EEAR7, 7	; EEPROM Read/Write Access Bit 7

; EEARH - EEPROM Address Register High Byte
.equ EEAR8, 0	; EEPROM Read/Write Access Bit 8
.equ EEAR9, 1	; EEPROM Read/Write Access Bit 9

; EEDR - EEPROM Data Register
.equ EEDR0, 0	; EEPROM Data Register bit 0
.equ EEDR1, 1	; EEPROM Data Register bit 1
.equ EEDR2, 2	; EEPROM Data Register bit 2
.equ EEDR3, 3	; EEPROM Data Register bit 3
.equ EEDR4, 4	; EEPROM Data Register bit 4
.equ EEDR5, 5	; EEPROM Data Register bit 5
.equ EEDR6, 6	; EEPROM Data Register bit 6
.equ EEDR7, 7	; EEPROM Data Register bit 7

; EECR - EEPROM Control Register
.equ EERE, 0	; EEPROM Read Enable
.equ EEPE, 1	; EEPROM Write Enable
.equ EEMPE, 2	; EEPROM Master Write Enable
.equ EERIE, 3	; EEPROM Ready Interrupt Enable
.equ EEPM0, 4	; EEPROM Programming Mode Bit 0
.equ EEPM1, 5	; EEPROM Programming Mode Bit 1



; ***** LOCKSBITS ********************************************************
.equ LB1, 0	; Lock bit
.equ LB2, 1	; Lock bit
.equ BLB01, 2	; Boot Lock bit
.equ BLB02, 3	; Boot Lock bit
.equ BLB11, 4	; Boot lock bit
.equ BLB12, 5	; Boot lock bit


; ***** FUSES ************************************************************
; LOW fuse bits
.equ CKSEL0, 0	; Select Clock Source
.equ CKSEL1, 1	; Select Clock Source
.equ CKSEL2, 2	; Select Clock Source
.equ CKSEL3, 3	; Select Clock Source
.equ SUT0, 4	; Select start-up time
.equ SUT1, 5	; Select start-up time
.equ CKOUT, 6	; Clock output
.equ CKDIV8, 7	; Divide clock by 8

; HIGH fuse bits
.equ BOOTRST, 0	; Select reset vector
.equ BOOTSZ0, 1	; Select boot size
.equ BOOTSZ1, 2	; Select boot size
.equ EESAVE, 3	; EEPROM memory is preserved through chip erase
.equ WDTON, 4	; Watchdog Timer Always On
.equ SPIEN, 5	; Enable Serial programming and Data Downloading
.equ DWEN, 6	; debugWIRE Enable
.equ RSTDISBL, 7	; External reset disable

; EXTENDED fuse bits
.equ BODLEVEL0, 0	; Brown-out Detector trigger level
.equ BODLEVEL1, 1	; Brown-out Detector trigger level
.equ BODLEVEL2, 2	; Brown-out Detector trigger level



; ***** CPU REGISTER DEFINITIONS *****************************************
#define XH r27
#define XL r26
#define YH r29
#define YL r28
#define ZH r31
#define ZL r30



; ***** DATA MEMORY DECLARATIONS *****************************************
.equ FLASHEND, 0x3fff	; Note: Word address
.equ IOEND, 0x00ff
.equ SRAM_START, 0x0100
.equ SRAM_SIZE, 2048
.equ RAMEND, 0x08ff
.equ XRAMEND, 0x0000
.equ E2END, 0x03ff
.equ EEPROMEND, 0x03ff
.equ EEADRBITS, 10
#pragma AVRPART MEMORY PROG_FLASH 32768
#pragma AVRPART MEMORY EEPROM 1024
#pragma AVRPART MEMORY INT_SRAM SIZE 2048
#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100



; ***** BOOTLOADER DECLARATIONS ******************************************
.equ NRWW_START_ADDR, 0x3800
.equ NRWW_STOP_ADDR, 0x3fff
.equ RWW_START_ADDR, 0x0
.equ RWW_STOP_ADDR, 0x37ff
.equ PAGESIZE, 64
.equ FIRSTBOOTSTART, 0x3f00
.equ SECONDBOOTSTART, 0x3e00
.equ THIRDBOOTSTART, 0x3c00
.equ FOURTHBOOTSTART, 0x3800
.equ SMALLBOOTSTART, FIRSTBOOTSTART
.equ LARGEBOOTSTART, FOURTHBOOTSTART



; ***** INTERRUPT VECTORS ************************************************
.equ INT0addr, 0x0002	; External Interrupt Request 0
.equ INT1addr, 0x0004	; External Interrupt Request 1
.equ PCI0addr, 0x0006	; Pin Change Interrupt Request 0
.equ PCI1addr, 0x0008	; Pin Change Interrupt Request 0
.equ PCI2addr, 0x000a	; Pin Change Interrupt Request 1
.equ WDTaddr, 0x000c	; Watchdog Time-out Interrupt
.equ OC2Aaddr, 0x000e	; Timer/Counter2 Compare Match A
.equ OC2Baddr, 0x0010	; Timer/Counter2 Compare Match A
.equ OVF2addr, 0x0012	; Timer/Counter2 Overflow
.equ ICP1addr, 0x0014	; Timer/Counter1 Capture Event
.equ OC1Aaddr, 0x0016	; Timer/Counter1 Compare Match A
.equ OC1Baddr, 0x0018	; Timer/Counter1 Compare Match B
.equ OVF1addr, 0x001a	; Timer/Counter1 Overflow
.equ OC0Aaddr, 0x001c	; TimerCounter0 Compare Match A
.equ OC0Baddr, 0x001e	; TimerCounter0 Compare Match B
.equ OVF0addr, 0x0020	; Timer/Couner0 Overflow
.equ SPIaddr, 0x0022	; SPI Serial Transfer Complete
.equ URXCaddr, 0x0024	; USART Rx Complete
.equ UDREaddr, 0x0026	; USART, Data Register Empty
.equ UTXCaddr, 0x0028	; USART Tx Complete
.equ ADCCaddr, 0x002a	; ADC Conversion Complete
.equ ERDYaddr, 0x002c	; EEPROM Ready
.equ ACIaddr, 0x002e	; Analog Comparator
.equ TWIaddr, 0x0030	; Two-wire Serial Interface
.equ SPMRaddr, 0x0032	; Store Program Memory Read

.equ INT_VECTORS_SIZE, 52	; size in words

#endif  /* _M328PDEF_INC_ */

; ***** END OF FILE ******************************************************

; --- Aliases em minusculas (o AVRASM2 e case-insensitive; o GNU as nao) ---
.equ signature_000, SIGNATURE_000
.equ signature_001, SIGNATURE_001
.equ signature_002, SIGNATURE_002
.equ udr0, UDR0
.equ ubrr0l, UBRR0L
.equ ubrr0h, UBRR0H
.equ ucsr0c, UCSR0C
.equ ucsr0b, UCSR0B
.equ ucsr0a, UCSR0A
.equ twamr, TWAMR
.equ twcr, TWCR
.equ twdr, TWDR
.equ twar, TWAR
.equ twsr, TWSR
.equ twbr, TWBR
.equ assr, ASSR
.equ ocr2b, OCR2B
.equ ocr2a, OCR2A
.equ tcnt2, TCNT2
.equ tccr2b, TCCR2B
.equ tccr2a, TCCR2A
.equ ocr1bl, OCR1BL
.equ ocr1bh, OCR1BH
.equ ocr1al, OCR1AL
.equ ocr1ah, OCR1AH
.equ icr1l, ICR1L
.equ icr1h, ICR1H
.equ tcnt1l, TCNT1L
.equ tcnt1h, TCNT1H
.equ tccr1c, TCCR1C
.equ tccr1b, TCCR1B
.equ tccr1a, TCCR1A
.equ didr1, DIDR1
.equ didr0, DIDR0
.equ admux, ADMUX
.equ adcsrb, ADCSRB
.equ adcsra, ADCSRA
.equ adch, ADCH
.equ adcl, ADCL
.equ timsk2, TIMSK2
.equ timsk1, TIMSK1
.equ timsk0, TIMSK0
.equ pcmsk1, PCMSK1
.equ pcmsk2, PCMSK2
.equ pcmsk0, PCMSK0
.equ eicra, EICRA
.equ pcicr, PCICR
.equ osccal, OSCCAL
.equ prr, PRR
.equ clkpr, CLKPR
.equ wdtcsr, WDTCSR
.equ sreg, SREG
.equ spl, SPL
.equ sph, SPH
.equ spmcsr, SPMCSR
.equ mcucr, MCUCR
.equ mcusr, MCUSR
.equ smcr, SMCR
.equ acsr, ACSR
.equ spdr, SPDR
.equ spsr, SPSR
.equ spcr, SPCR
.equ gpior2, GPIOR2
.equ gpior1, GPIOR1
.equ ocr0b, OCR0B
.equ ocr0a, OCR0A
.equ tcnt0, TCNT0
.equ tccr0b, TCCR0B
.equ tccr0a, TCCR0A
.equ gtccr, GTCCR
.equ eearh, EEARH
.equ eearl, EEARL
.equ eedr, EEDR
.equ eecr, EECR
.equ gpior0, GPIOR0
.equ eimsk, EIMSK
.equ eifr, EIFR
.equ pcifr, PCIFR
.equ tifr2, TIFR2
.equ tifr1, TIFR1
.equ tifr0, TIFR0
.equ portd, PORTD
.equ ddrd, DDRD
.equ pind, PIND
.equ portc, PORTC
.equ ddrc, DDRC
.equ pinc, PINC
.equ portb, PORTB
.equ ddrb, DDRB
.equ pinb, PINB
.equ udr0_0, UDR0_0
.equ udr0_1, UDR0_1
.equ udr0_2, UDR0_2
.equ udr0_3, UDR0_3
.equ udr0_4, UDR0_4
.equ udr0_5, UDR0_5
.equ udr0_6, UDR0_6
.equ udr0_7, UDR0_7
.equ mpcm0, MPCM0
.equ u2x0, U2X0
.equ upe0, UPE0
.equ dor0, DOR0
.equ fe0, FE0
.equ udre0, UDRE0
.equ txc0, TXC0
.equ rxc0, RXC0
.equ txb80, TXB80
.equ rxb80, RXB80
.equ ucsz02, UCSZ02
.equ txen0, TXEN0
.equ rxen0, RXEN0
.equ udrie0, UDRIE0
.equ txcie0, TXCIE0
.equ rxcie0, RXCIE0
.equ ucpol0, UCPOL0
.equ ucsz00, UCSZ00
.equ ucpha0, UCPHA0
.equ ucsz01, UCSZ01
.equ udord0, UDORD0
.equ usbs0, USBS0
.equ upm00, UPM00
.equ upm01, UPM01
.equ umsel00, UMSEL00
.equ umsel0, UMSEL0
.equ umsel01, UMSEL01
.equ umsel1, UMSEL1
.equ ubrr8, UBRR8
.equ ubrr9, UBRR9
.equ ubrr10, UBRR10
.equ ubrr11, UBRR11
.equ _ubrr0, _UBRR0
.equ _ubrr1, _UBRR1
.equ ubrr2, UBRR2
.equ ubrr3, UBRR3
.equ ubrr4, UBRR4
.equ ubrr5, UBRR5
.equ ubrr6, UBRR6
.equ ubrr7, UBRR7
.equ twam0, TWAM0
.equ twamr0, TWAMR0
.equ twam1, TWAM1
.equ twamr1, TWAMR1
.equ twam2, TWAM2
.equ twamr2, TWAMR2
.equ twam3, TWAM3
.equ twamr3, TWAMR3
.equ twam4, TWAM4
.equ twamr4, TWAMR4
.equ twam5, TWAM5
.equ twamr5, TWAMR5
.equ twam6, TWAM6
.equ twamr6, TWAMR6
.equ twbr0, TWBR0
.equ twbr1, TWBR1
.equ twbr2, TWBR2
.equ twbr3, TWBR3
.equ twbr4, TWBR4
.equ twbr5, TWBR5
.equ twbr6, TWBR6
.equ twbr7, TWBR7
.equ twie, TWIE
.equ twen, TWEN
.equ twwc, TWWC
.equ twsto, TWSTO
.equ twsta, TWSTA
.equ twea, TWEA
.equ twint, TWINT
.equ twps0, TWPS0
.equ twps1, TWPS1
.equ tws3, TWS3
.equ tws4, TWS4
.equ tws5, TWS5
.equ tws6, TWS6
.equ tws7, TWS7
.equ twd0, TWD0
.equ twd1, TWD1
.equ twd2, TWD2
.equ twd3, TWD3
.equ twd4, TWD4
.equ twd5, TWD5
.equ twd6, TWD6
.equ twd7, TWD7
.equ twgce, TWGCE
.equ twa0, TWA0
.equ twa1, TWA1
.equ twa2, TWA2
.equ twa3, TWA3
.equ twa4, TWA4
.equ twa5, TWA5
.equ twa6, TWA6
.equ toie1, TOIE1
.equ ocie1a, OCIE1A
.equ ocie1b, OCIE1B
.equ icie1, ICIE1
.equ tov1, TOV1
.equ ocf1a, OCF1A
.equ ocf1b, OCF1B
.equ icf1, ICF1
.equ wgm10, WGM10
.equ wgm11, WGM11
.equ com1b0, COM1B0
.equ com1b1, COM1B1
.equ com1a0, COM1A0
.equ com1a1, COM1A1
.equ cs10, CS10
.equ cs11, CS11
.equ cs12, CS12
.equ wgm12, WGM12
.equ wgm13, WGM13
.equ ices1, ICES1
.equ icnc1, ICNC1
.equ foc1b, FOC1B
.equ foc1a, FOC1A
.equ psrsync, PSRSYNC
.equ tsm, TSM
.equ toie2, TOIE2
.equ toie2a, TOIE2A
.equ ocie2a, OCIE2A
.equ ocie2b, OCIE2B
.equ tov2, TOV2
.equ ocf2a, OCF2A
.equ ocf2b, OCF2B
.equ wgm20, WGM20
.equ wgm21, WGM21
.equ com2b0, COM2B0
.equ com2b1, COM2B1
.equ com2a0, COM2A0
.equ com2a1, COM2A1
.equ cs20, CS20
.equ cs21, CS21
.equ cs22, CS22
.equ wgm22, WGM22
.equ foc2b, FOC2B
.equ foc2a, FOC2A
.equ tcnt2_0, TCNT2_0
.equ tcnt2_1, TCNT2_1
.equ tcnt2_2, TCNT2_2
.equ tcnt2_3, TCNT2_3
.equ tcnt2_4, TCNT2_4
.equ tcnt2_5, TCNT2_5
.equ tcnt2_6, TCNT2_6
.equ tcnt2_7, TCNT2_7
.equ ocr2a_0, OCR2A_0
.equ ocr2a_1, OCR2A_1
.equ ocr2a_2, OCR2A_2
.equ ocr2a_3, OCR2A_3
.equ ocr2a_4, OCR2A_4
.equ ocr2a_5, OCR2A_5
.equ ocr2a_6, OCR2A_6
.equ ocr2a_7, OCR2A_7
.equ ocr2b_0, OCR2B_0
.equ ocr2b_1, OCR2B_1
.equ ocr2b_2, OCR2B_2
.equ ocr2b_3, OCR2B_3
.equ ocr2b_4, OCR2B_4
.equ ocr2b_5, OCR2B_5
.equ ocr2b_6, OCR2B_6
.equ ocr2b_7, OCR2B_7
.equ tcr2bub, TCR2BUB
.equ tcr2aub, TCR2AUB
.equ ocr2bub, OCR2BUB
.equ ocr2aub, OCR2AUB
.equ tcn2ub, TCN2UB
.equ as2, AS2
.equ exclk, EXCLK
.equ psrasy, PSRASY
.equ psr2, PSR2
.equ mux0, MUX0
.equ mux1, MUX1
.equ mux2, MUX2
.equ mux3, MUX3
.equ adlar, ADLAR
.equ refs0, REFS0
.equ refs1, REFS1
.equ adps0, ADPS0
.equ adps1, ADPS1
.equ adps2, ADPS2
.equ adie, ADIE
.equ adif, ADIF
.equ adate, ADATE
.equ adsc, ADSC
.equ aden, ADEN
.equ adts0, ADTS0
.equ adts1, ADTS1
.equ adts2, ADTS2
.equ acme, ACME
.equ adch0, ADCH0
.equ adch1, ADCH1
.equ adch2, ADCH2
.equ adch3, ADCH3
.equ adch4, ADCH4
.equ adch5, ADCH5
.equ adch6, ADCH6
.equ adch7, ADCH7
.equ adcl0, ADCL0
.equ adcl1, ADCL1
.equ adcl2, ADCL2
.equ adcl3, ADCL3
.equ adcl4, ADCL4
.equ adcl5, ADCL5
.equ adcl6, ADCL6
.equ adcl7, ADCL7
.equ adc0d, ADC0D
.equ adc1d, ADC1D
.equ adc2d, ADC2D
.equ adc3d, ADC3D
.equ adc4d, ADC4D
.equ adc5d, ADC5D
.equ acis0, ACIS0
.equ acis1, ACIS1
.equ acic, ACIC
.equ acie, ACIE
.equ aci, ACI
.equ aco, ACO
.equ acbg, ACBG
.equ acd, ACD
.equ ain0d, AIN0D
.equ ain1d, AIN1D
.equ portb0, PORTB0
.equ pb0, PB0
.equ portb1, PORTB1
.equ pb1, PB1
.equ portb2, PORTB2
.equ pb2, PB2
.equ portb3, PORTB3
.equ pb3, PB3
.equ portb4, PORTB4
.equ pb4, PB4
.equ portb5, PORTB5
.equ pb5, PB5
.equ portb6, PORTB6
.equ pb6, PB6
.equ portb7, PORTB7
.equ pb7, PB7
.equ ddb0, DDB0
.equ ddb1, DDB1
.equ ddb2, DDB2
.equ ddb3, DDB3
.equ ddb4, DDB4
.equ ddb5, DDB5
.equ ddb6, DDB6
.equ ddb7, DDB7
.equ pinb0, PINB0
.equ pinb1, PINB1
.equ pinb2, PINB2
.equ pinb3, PINB3
.equ pinb4, PINB4
.equ pinb5, PINB5
.equ pinb6, PINB6
.equ pinb7, PINB7
.equ portc0, PORTC0
.equ pc0, PC0
.equ portc1, PORTC1
.equ pc1, PC1
.equ portc2, PORTC2
.equ pc2, PC2
.equ portc3, PORTC3
.equ pc3, PC3
.equ portc4, PORTC4
.equ pc4, PC4
.equ portc5, PORTC5
.equ pc5, PC5
.equ portc6, PORTC6
.equ pc6, PC6
.equ ddc0, DDC0
.equ ddc1, DDC1
.equ ddc2, DDC2
.equ ddc3, DDC3
.equ ddc4, DDC4
.equ ddc5, DDC5
.equ ddc6, DDC6
.equ pinc0, PINC0
.equ pinc1, PINC1
.equ pinc2, PINC2
.equ pinc3, PINC3
.equ pinc4, PINC4
.equ pinc5, PINC5
.equ pinc6, PINC6
.equ portd0, PORTD0
.equ pd0, PD0
.equ portd1, PORTD1
.equ pd1, PD1
.equ portd2, PORTD2
.equ pd2, PD2
.equ portd3, PORTD3
.equ pd3, PD3
.equ portd4, PORTD4
.equ pd4, PD4
.equ portd5, PORTD5
.equ pd5, PD5
.equ portd6, PORTD6
.equ pd6, PD6
.equ portd7, PORTD7
.equ pd7, PD7
.equ ddd0, DDD0
.equ ddd1, DDD1
.equ ddd2, DDD2
.equ ddd3, DDD3
.equ ddd4, DDD4
.equ ddd5, DDD5
.equ ddd6, DDD6
.equ ddd7, DDD7
.equ pind0, PIND0
.equ pind1, PIND1
.equ pind2, PIND2
.equ pind3, PIND3
.equ pind4, PIND4
.equ pind5, PIND5
.equ pind6, PIND6
.equ pind7, PIND7
.equ toie0, TOIE0
.equ ocie0a, OCIE0A
.equ ocie0b, OCIE0B
.equ tov0, TOV0
.equ ocf0a, OCF0A
.equ ocf0b, OCF0B
.equ wgm00, WGM00
.equ wgm01, WGM01
.equ com0b0, COM0B0
.equ com0b1, COM0B1
.equ com0a0, COM0A0
.equ com0a1, COM0A1
.equ cs00, CS00
.equ cs01, CS01
.equ cs02, CS02
.equ wgm02, WGM02
.equ foc0b, FOC0B
.equ foc0a, FOC0A
.equ tcnt0_0, TCNT0_0
.equ tcnt0_1, TCNT0_1
.equ tcnt0_2, TCNT0_2
.equ tcnt0_3, TCNT0_3
.equ tcnt0_4, TCNT0_4
.equ tcnt0_5, TCNT0_5
.equ tcnt0_6, TCNT0_6
.equ tcnt0_7, TCNT0_7
.equ ocr0a_0, OCR0A_0
.equ ocr0a_1, OCR0A_1
.equ ocr0a_2, OCR0A_2
.equ ocr0a_3, OCR0A_3
.equ ocr0a_4, OCR0A_4
.equ ocr0a_5, OCR0A_5
.equ ocr0a_6, OCR0A_6
.equ ocr0a_7, OCR0A_7
.equ ocr0b_0, OCR0B_0
.equ ocr0b_1, OCR0B_1
.equ ocr0b_2, OCR0B_2
.equ ocr0b_3, OCR0B_3
.equ ocr0b_4, OCR0B_4
.equ ocr0b_5, OCR0B_5
.equ ocr0b_6, OCR0B_6
.equ ocr0b_7, OCR0B_7
.equ psr10, PSR10
.equ isc00, ISC00
.equ isc01, ISC01
.equ isc10, ISC10
.equ isc11, ISC11
.equ int0, INT0
.equ int1, INT1
.equ intf0, INTF0
.equ intf1, INTF1
.equ pcie0, PCIE0
.equ pcie1, PCIE1
.equ pcie2, PCIE2
.equ pcint16, PCINT16
.equ pcint17, PCINT17
.equ pcint18, PCINT18
.equ pcint19, PCINT19
.equ pcint20, PCINT20
.equ pcint21, PCINT21
.equ pcint22, PCINT22
.equ pcint23, PCINT23
.equ pcint8, PCINT8
.equ pcint9, PCINT9
.equ pcint10, PCINT10
.equ pcint11, PCINT11
.equ pcint12, PCINT12
.equ pcint13, PCINT13
.equ pcint14, PCINT14
.equ pcint0, PCINT0
.equ pcint1, PCINT1
.equ pcint2, PCINT2
.equ pcint3, PCINT3
.equ pcint4, PCINT4
.equ pcint5, PCINT5
.equ pcint6, PCINT6
.equ pcint7, PCINT7
.equ pcif0, PCIF0
.equ pcif1, PCIF1
.equ pcif2, PCIF2
.equ spdr0, SPDR0
.equ spdr1, SPDR1
.equ spdr2, SPDR2
.equ spdr3, SPDR3
.equ spdr4, SPDR4
.equ spdr5, SPDR5
.equ spdr6, SPDR6
.equ spdr7, SPDR7
.equ spi2x, SPI2X
.equ wcol, WCOL
.equ spif, SPIF
.equ spr0, SPR0
.equ spr1, SPR1
.equ cpha, CPHA
.equ cpol, CPOL
.equ mstr, MSTR
.equ dord, DORD
.equ spe, SPE
.equ spie, SPIE
.equ wdp0, WDP0
.equ wdp1, WDP1
.equ wdp2, WDP2
.equ wde, WDE
.equ wdce, WDCE
.equ wdp3, WDP3
.equ wdie, WDIE
.equ wdif, WDIF
.equ sreg_c, SREG_C
.equ sreg_z, SREG_Z
.equ sreg_n, SREG_N
.equ sreg_v, SREG_V
.equ sreg_s, SREG_S
.equ sreg_h, SREG_H
.equ sreg_t, SREG_T
.equ sreg_i, SREG_I
.equ cal0, CAL0
.equ cal1, CAL1
.equ cal2, CAL2
.equ cal3, CAL3
.equ cal4, CAL4
.equ cal5, CAL5
.equ cal6, CAL6
.equ cal7, CAL7
.equ clkps0, CLKPS0
.equ clkps1, CLKPS1
.equ clkps2, CLKPS2
.equ clkps3, CLKPS3
.equ clkpce, CLKPCE
.equ selfprgen, SELFPRGEN
.equ spmen, SPMEN
.equ pgers, PGERS
.equ pgwrt, PGWRT
.equ blbset, BLBSET
.equ rwwsre, RWWSRE
.equ sigrd, SIGRD
.equ rwwsb, RWWSB
.equ spmie, SPMIE
.equ ivce, IVCE
.equ ivsel, IVSEL
.equ pud, PUD
.equ bodse, BODSE
.equ bods, BODS
.equ porf, PORF
.equ extrf, EXTRF
.equ extref, EXTREF
.equ borf, BORF
.equ wdrf, WDRF
.equ se, SE
.equ sm0, SM0
.equ sm1, SM1
.equ sm2, SM2
.equ gpior20, GPIOR20
.equ gpior21, GPIOR21
.equ gpior22, GPIOR22
.equ gpior23, GPIOR23
.equ gpior24, GPIOR24
.equ gpior25, GPIOR25
.equ gpior26, GPIOR26
.equ gpior27, GPIOR27
.equ gpior10, GPIOR10
.equ gpior11, GPIOR11
.equ gpior12, GPIOR12
.equ gpior13, GPIOR13
.equ gpior14, GPIOR14
.equ gpior15, GPIOR15
.equ gpior16, GPIOR16
.equ gpior17, GPIOR17
.equ gpior00, GPIOR00
.equ gpior01, GPIOR01
.equ gpior02, GPIOR02
.equ gpior03, GPIOR03
.equ gpior04, GPIOR04
.equ gpior05, GPIOR05
.equ gpior06, GPIOR06
.equ gpior07, GPIOR07
.equ pradc, PRADC
.equ prusart0, PRUSART0
.equ prspi, PRSPI
.equ prtim1, PRTIM1
.equ prtim0, PRTIM0
.equ prtim2, PRTIM2
.equ prtwi, PRTWI
.equ eear0, EEAR0
.equ eear1, EEAR1
.equ eear2, EEAR2
.equ eear3, EEAR3
.equ eear4, EEAR4
.equ eear5, EEAR5
.equ eear6, EEAR6
.equ eear7, EEAR7
.equ eear8, EEAR8
.equ eear9, EEAR9
.equ eedr0, EEDR0
.equ eedr1, EEDR1
.equ eedr2, EEDR2
.equ eedr3, EEDR3
.equ eedr4, EEDR4
.equ eedr5, EEDR5
.equ eedr6, EEDR6
.equ eedr7, EEDR7
.equ eere, EERE
.equ eepe, EEPE
.equ eempe, EEMPE
.equ eerie, EERIE
.equ eepm0, EEPM0
.equ eepm1, EEPM1
.equ lb1, LB1
.equ lb2, LB2
.equ blb01, BLB01
.equ blb02, BLB02
.equ blb11, BLB11
.equ blb12, BLB12
.equ cksel0, CKSEL0
.equ cksel1, CKSEL1
.equ cksel2, CKSEL2
.equ cksel3, CKSEL3
.equ sut0, SUT0
.equ sut1, SUT1
.equ ckout, CKOUT
.equ ckdiv8, CKDIV8
.equ bootrst, BOOTRST
.equ bootsz0, BOOTSZ0
.equ bootsz1, BOOTSZ1
.equ eesave, EESAVE
.equ wdton, WDTON
.equ spien, SPIEN
.equ dwen, DWEN
.equ rstdisbl, RSTDISBL
.equ bodlevel0, BODLEVEL0
.equ bodlevel1, BODLEVEL1
.equ bodlevel2, BODLEVEL2
.equ flashend, FLASHEND
.equ ioend, IOEND
.equ sram_start, SRAM_START
.equ sram_size, SRAM_SIZE
.equ ramend, RAMEND
.equ xramend, XRAMEND
.equ e2end, E2END
.equ eepromend, EEPROMEND
.equ eeadrbits, EEADRBITS
.equ nrww_start_addr, NRWW_START_ADDR
.equ nrww_stop_addr, NRWW_STOP_ADDR
.equ rww_start_addr, RWW_START_ADDR
.equ rww_stop_addr, RWW_STOP_ADDR
.equ pagesize, PAGESIZE
.equ firstbootstart, FIRSTBOOTSTART
.equ secondbootstart, SECONDBOOTSTART
.equ thirdbootstart, THIRDBOOTSTART
.equ fourthbootstart, FOURTHBOOTSTART
.equ smallbootstart, SMALLBOOTSTART
.equ largebootstart, LARGEBOOTSTART
.equ int0addr, INT0addr
.equ int1addr, INT1addr
.equ pci0addr, PCI0addr
.equ pci1addr, PCI1addr
.equ pci2addr, PCI2addr
.equ wdtaddr, WDTaddr
.equ oc2aaddr, OC2Aaddr
.equ oc2baddr, OC2Baddr
.equ ovf2addr, OVF2addr
.equ icp1addr, ICP1addr
.equ oc1aaddr, OC1Aaddr
.equ oc1baddr, OC1Baddr
.equ ovf1addr, OVF1addr
.equ oc0aaddr, OC0Aaddr
.equ oc0baddr, OC0Baddr
.equ ovf0addr, OVF0addr
.equ spiaddr, SPIaddr
.equ urxcaddr, URXCaddr
.equ udreaddr, UDREaddr
.equ utxcaddr, UTXCaddr
.equ adccaddr, ADCCaddr
.equ erdyaddr, ERDYaddr
.equ aciaddr, ACIaddr
.equ twiaddr, TWIaddr
.equ spmraddr, SPMRaddr
.equ int_vectors_size, INT_VECTORS_SIZE
