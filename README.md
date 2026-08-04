# Assembly do AVR (ATmega328P) no VS Code + MPLAB

Ambiente completo para **codar em assembly com a sintaxe do Microchip Studio (AVRASM2)**,
buildar, **simular e debugar** (registradores, memória, step por instrução) com o
**MPLAB Simulator** dentro do VS Code, e **gravar no Arduino** com o AVRDUDE.

Funciona no **Windows (via WSL)** e no **Linux**. Placa: Arduino Uno (ATmega328P).

## Instalação (2 comandos)

Em ambos os casos você precisa do **VS Code** (https://code.visualstudio.com).

### Windows

Pré-requisito: **WSL com Ubuntu** — em um PowerShell como administrador:
`wsl --install -d Ubuntu` (reinicie e crie seu usuário Linux quando pedir).

No terminal do **Ubuntu (WSL)**:

```bash
git clone https://github.com/ErickMascarenhas/microcontroladores-mplab-vsc.git
cd microcontroladores-mplab-vsc && ./setup/setup-wsl.sh
```

O script instala a toolchain AVR no WSL, gera o include `m328Pdef.inc`, instala as
extensões do VS Code e, ao final, oferece rodar a parte Windows (usbipd + extensões
locais). Depois **feche e reabra o terminal** (necessário para o grupo `dialout`).

### Linux

```bash
git clone https://github.com/ErickMascarenhas/microcontroladores-mplab-vsc.git
cd microcontroladores-mplab-vsc && ./setup/setup-linux.sh
```

O script reconhece a distribuição (Debian/Ubuntu, Fedora, Arch, openSUSE), instala a
toolchain, libera o acesso à porta serial, gera o include, instala as extensões do
VS Code e confere tudo no final. Depois **faça logout e login** (ou `newgrp dialout`).
Aqui o Arduino é acessado direto (`/dev/ttyACM0`): o `attach-arduino.ps1` é só do Windows.

## Usando

```bash
cd projeto && code .
```

As tasks são executadas por `Ctrl+Shift+P` → *Tasks: Run Task*.

| Ação | Como |
|---|---|
| **Buildar** (gera `build/main.elf` + `build/main.hex`) | `Ctrl+Shift+B` |
| Buildar o exemplo | task **"Build exemplo"** |
| **Simular/Debugar (MPLAB Simulator)** | `F5` — config "Simular: MPLAB Simulator" |
| Debugar o exemplo | `F5` — config "Debugar serial-eco (MPLAB Simulator)" |
| Step por instrução assembly | `Ctrl+Shift+F11` (ou o botão extra na barra de debug) |
| Registradores (r0–r31, SREG, SP, PC) | *Run and Debug* → *Variables* → *Registers* → *CPU* |
| **Memória completa** (SRAM/flash) | Com o debug **pausado**: `Ctrl+Shift+P` → **Memory: Show Memory Inspector** (o adaptador MPLAB suporta ler *e* escrever memória) |
| **SFRs/periféricos (USART, portas, timers)** | `Ctrl+Shift+P` → **MPLAB IO View: Show** durante o debug |
| **Gravar na placa** | task **"Upload para o Arduino"** (no Windows, rode antes `setup\attach-arduino.ps1`) |
| Gravar o exemplo na placa | task **"Upload serial-eco para o Arduino"** |
| **Monitor serial (USART na placa)** | task **"Monitor serial (placa via USB)"** (picocom; `Ctrl+A Ctrl+X` sai) |
| **Ciclos de clock / tempo de uma rotina** | task **"Medir ciclos / tempo"** (veja [Clock e ciclos](#clock-e-ciclos)) |
| Ver os rótulos que podem ser medidos | task **"Listar rótulos do programa"** |
| **Serial no simulador, sem placa** | task **"Terminal serial no simulador (serial-eco)"** |
| Simulador alternativo (simavr) | `F5` — config "Simular: simavr + GDB" |
| Apagar os arquivos gerados | task **"Limpar (make clean)"** |

## Clock e ciclos

O debugger MPLAB no VS Code ainda não tem o painel de Stopwatch do MPLAB X, então
o projeto traz um cronômetro próprio ([ferramentas/avr-ciclos.c](projeto/ferramentas/avr-ciclos.c),
compilado no primeiro uso): ele roda o programa num simulador ciclo-preciso e diz
quantos ciclos de clock e quanto tempo se passam entre dois pontos do seu código.

Use a task **"Medir ciclos / tempo"** (ela pergunta de onde até onde) ou o terminal:

```bash
make rotulos                                          # que rótulos posso medir?
make ciclos DE=loopit ATE=loopit                      # uma volta do loop
make ciclos ATE=loopit                                # do reset até o loop
```

Saída típica — aqui, uma volta do loop do `main.asm` (`out` + `rjmp` = 3 ciclos):

```
Firmware : build/main.elf (atmega328p @ 16.000 MHz)
De       : loopit (0x0006)
Até      : loopit (0x0006)
---------------------------------------------
Ciclos   : 3
Tempo    : 187.5 ns
```

O clock padrão é 16 MHz (Arduino Uno); mude com `make ciclos F_CPU=8000000`.

## USART

Segue um exemplo, [exemplo/serial-eco.asm](projeto/exemplo/serial-eco.asm)
(Código que converte para maiúsculas, 9600 bauds).

**No simulador, sem placa (TX+RX):** task **"Terminal serial no simulador
(serial-eco)"** — a USART0 simulada fica ligada ao terminal do VS Code: digite
`abcdef` e o programa responde `ABCDEF`. `Ctrl+C` encerra. Pelo terminal:

```bash
make serial                            # exemplo de eco
make serial SERIAL_ALVO=build/main.elf # o seu programa
```

Isso usa [ferramentas/avr-serial.c](projeto/ferramentas/avr-serial.c), que roda o
firmware no simulador com a USART0 conectada à sua entrada/saída padrão.

**Na placa real:** execute a task **"Upload serial-eco para o Arduino"** e depois
**"Monitor serial (placa via USB)"** (picocom). No Windows, rode antes o
`setup\attach-arduino.ps1`; no Linux basta conectar a placa.

## Compatibilidade com o AVRASM2

O código é escrito exatamente como no Microchip Studio: o build converte a
sintaxe para o GNU assembler a cada compilação ([avrasm-compat.awk](projeto/avrasm-compat.awk)),
preservando os números de linha.

## Estrutura

```
setup/
  setup-wsl.sh        # Windows: toolchain AVR + include + extensões (roda no Ubuntu do WSL)
  setup-windows.ps1   # Windows: usbipd-win + extensões VS Code
  attach-arduino.ps1  # Windows: conecta o USB do Arduino ao WSL (a cada reconexão)
  setup-linux.sh      # Linux: instala e configura tudo (dispensa os três acima)
projeto/
  main.asm            # seu código (sintaxe AVRASM2)
  Makefile            # build / flash / sim  (make, make flash, make sim)
  avrasm-compat.awk   # conversão transparente AVRASM2 -> GNU as
  ferramentas/        # cronômetro de ciclos e terminal serial do simulador
  include/m328Pdef.inc# registradores/bits do ATmega328P (gerado do pack oficial)
  .vscode/            # tasks, launch (MPLAB Simulator + simavr), projeto MPLAB
```

## Solução de problemas

- **`avrdude: can't open device /dev/ttyACM0`** — confira a porta com
  `ls /dev/ttyACM* /dev/ttyUSB*` (clones com CH340 aparecem como `/dev/ttyUSB0`:
  use `make flash PORT=/dev/ttyUSB0`). No Windows, rode `setup\attach-arduino.ps1`
  a cada reconexão da placa.
- **`Permission denied` na porta serial** — falta a sessão nova depois de entrar no
  grupo: no Windows feche e reabra o terminal; no Linux faça logout e login.
- **Clone com CH340 não aparece no Linux** — o `brltty` costuma capturar esse
  conversor; o `setup-linux.sh` desativa a regra dele, mas pode ser preciso
  reconectar a placa depois.
- **F5 do MPLAB não inicia** — veja o painel *Output* → *MPLAB*; utilize a opção
  (`simavr + GDB`) para debug.
- **"Failed to start session: No source lines found"** — o ELF está sem o símbolo
  `main`: builde pelo `Ctrl+Shift+B` (é o Makefile que injeta o símbolo).
- **Breakpoint não fixa no .asm** — confirme que abriu a pasta `projeto/` e que o build (`Ctrl+Shift+B`) rodou sem erros.
- **MPLAB IO View mostra só "?" no simavr** — esperado: o IO View lê pelos canais
  do adaptador MPLAB.
- **Não encontro "Memory: Show Memory Inspector" na paleta** — o comando é da
  categoria *Memory* (não "Memory Inspector") e só aparece com uma sessão de debug
  **pausada**, abrindo a pasta `projeto/` — é o `.vscode/settings.json` dela que
  registra o debugger do MPLAB no visualizador.
- **Memory Inspector pede endereço** — digite `0` (ou um símbolo) e Enter.
- **"Cannot read field 'pcAddr' because 'csFrame' is null"** — bug conhecido do
  backend MPLAB (beta) ao resolver a call stack em certas pausas; não afeta a
  execução.
