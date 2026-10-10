# ============================================================
#  Projeto AVR Assembly (ATmega328P) — VS Code + WSL + MPLAB
#  Sintaxe do código: AVRASM2 (Microchip Studio)
# ============================================================
MCU         = atmega328p
SRC         = main.asm
BUILD       = build
ELF         = $(BUILD)/main.elf
HEX         = $(BUILD)/main.hex
INCDIR      = include

# Clock da placa/simulação (Arduino Uno: 16 MHz)
F_CPU      ?= 16000000

# Upload (Arduino Uno via bootloader optiboot)
PORT       ?= /dev/ttyACM0
BAUD       ?= 115200
PROGRAMMER ?= arduino

# Fim da SRAM (RAMEND). O debugger MPLAB precisa do símbolo __stack para
# resolver a call stack; como linkamos sem o runtime C (que o definiria),
# nós o definimos aqui. ATmega328P: 0x8FF.
RAMEND     ?= 0x8FF

# Flags comuns de assembler/link (DWARF apontando para o .asm original)
ASFLAGS = -mmcu=$(MCU) -nostartfiles -nostdlib -x assembler-with-cpp \
          -Wa,-gdwarf2 -Wa,-I,$(INCDIR) -Wl,--defsym,__stack=$(RAMEND)

# O prólogo (1) inclui o m328Pdef.inc automaticamente, como o Microchip
# Studio faz ao selecionar o device — o seu .asm não precisa do .include
# (mas pode tê-lo; o .equ do GNU as é redefinível) — e (2) injeta um
# símbolo global "main" (o debugger MPLAB executa "run to main" em todo
# launch) sem alterar o seu código-fonte.
define PROLOGO
{ echo '.include "m328Pdef.inc"'; echo '.global main'; echo 'main:'; echo '# 1 "$(1)"'; awk -f avrasm-compat.awk $(1) $(1); } > $(2)
endef

all: $(HEX)

$(BUILD):
	mkdir -p $(BUILD)

# ---- main.asm -------------------------------------------------------------
$(BUILD)/main.S: $(SRC) avrasm-compat.awk | $(BUILD)
	$(call PROLOGO,$(SRC),$@)

$(ELF): $(BUILD)/main.S
	avr-gcc $(ASFLAGS) $< -o $@

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $< $@
	@echo ">> Build OK: $(ELF) e $(HEX)"

# ---- exemplo/*.asm (um ELF para cada) -------------------------------------
$(BUILD)/%.S: exemplo/%.asm avrasm-compat.awk | $(BUILD)
	$(call PROLOGO,exemplo/$*.asm,$@)

$(BUILD)/%.elf: $(BUILD)/%.S
	avr-gcc $(ASFLAGS) $< -o $@

$(BUILD)/%.hex: $(BUILD)/%.elf
	avr-objcopy -O ihex -R .eeprom $< $@

exemplo: $(BUILD)/serial-eco.elf
	@echo ">> Exemplo OK"

# ---- ferramentas de simulação (compiladas sob demanda) ---------------------
$(BUILD)/avr-ciclos: ferramentas/avr-ciclos.c | $(BUILD)
	gcc -O2 -Wall $< -o $@ -lsimavr

$(BUILD)/avr-serial: ferramentas/avr-serial.c | $(BUILD)
	gcc -O2 -Wall $< -o $@ -lsimavr -lpthread

ferramentas: $(BUILD)/avr-ciclos $(BUILD)/avr-serial
	@echo ">> Ferramentas OK"

# Cronômetro de ciclos/tempo. Exemplos:
#   make ciclos DE=loopit ATE=loopit         (uma volta do loop)
#   make ciclos ALVO=build/serial-eco.elf DE=delay ATE=loop
#   make rotulos                             (lista os rótulos do programa)
ALVO ?= $(ELF)
DE   ?=
ATE  ?=
CICLOS_FLAGS = $(if $(DE),--de $(DE)) $(if $(ATE),--ate $(ATE))

ciclos: $(ALVO) $(BUILD)/avr-ciclos
	@./$(BUILD)/avr-ciclos $(ALVO) --freq $(F_CPU) $(CICLOS_FLAGS)

rotulos: $(ALVO) $(BUILD)/avr-ciclos
	@./$(BUILD)/avr-ciclos $(ALVO) --rotulos

# Terminal serial no simulador (USART0 ligada ao seu terminal; Ctrl+C sai).
#   make serial                              (usa o exemplo de eco)
#   make serial ALVO=build/main.elf
SERIAL_ALVO ?= $(BUILD)/serial-eco.elf

serial: $(SERIAL_ALVO) $(BUILD)/avr-serial
	@./$(BUILD)/avr-serial $(SERIAL_ALVO) --freq $(F_CPU)

# ---- gravação e simulação ---------------------------------------------------
# Grava na placa (make flash, ou make flash-eco para o exemplo de eco serial)
flash: $(HEX)
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(HEX):i

flash-eco: $(BUILD)/serial-eco.hex
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

# Simulador alternativo (simavr como servidor GDB na porta 1234)
sim: $(ELF)
	@echo ">> simavr aguardando GDB em :1234"
	simavr -g -m $(MCU) -f $(F_CPU) $(ELF)

clean:
	rm -rf $(BUILD)

.PHONY: all exemplo ferramentas ciclos rotulos serial flash flash-eco sim clean
