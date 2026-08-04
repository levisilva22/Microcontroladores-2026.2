#!/usr/bin/env bash
# ============================================================
#  setup-linux.sh — Configura o ambiente AVR Assembly no Linux
#  (para quem NÃO usa Windows/WSL; no Windows use setup-wsl.sh)
#
#  Uso, na raiz do repositório:
#      ./setup/setup-linux.sh
#
#  O que este script faz:
#   1. Instala a toolchain AVR (avr-gcc, avrdude, gdb, simavr...)
#   2. Garante o simavr com headers (compila do fonte se a sua
#      distribuição não o empacotar)
#   3. Dá ao seu usuário acesso à porta serial do Arduino
#   4. Gera o include m328Pdef.inc a partir do pack oficial da Microchip
#   5. Instala as extensões do VS Code
#   6. Confere tudo e mostra um resumo
# ============================================================
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJ_DIR="$REPO_DIR/projeto"
DFP_VERSION="3.6.299"   # versão fixa do pack = tutorial reprodutível
DFP_URL="https://packs.download.microchip.com/Microchip.ATmega_DFP.${DFP_VERSION}.atpack"
SIMAVR_VERSION="v1.7"   # usado só se a distribuição não empacotar o simavr

aviso()  { printf '    %s\n' "$*"; }
etapa()  { printf '\n==> %s\n' "$*"; }
erro()   { printf '    ERRO: %s\n' "$*" >&2; }

# ------------------------------------------------------------
# 0. Verificações iniciais
# ------------------------------------------------------------
if grep -qi microsoft /proc/version 2>/dev/null; then
    erro "Você está no WSL. Use ./setup/setup-wsl.sh, que também configura o lado Windows."
    exit 1
fi

if [ "$(id -u)" -eq 0 ]; then
    erro "Não rode como root: o script precisa saber quem é o seu usuário."
    aviso "Rode como você mesmo; ele pedirá a senha do sudo quando precisar."
    exit 1
fi

command -v sudo >/dev/null 2>&1 || { erro "sudo não encontrado — instale-o ou rode os comandos manualmente."; exit 1; }

# ------------------------------------------------------------
# 1. Toolchain AVR conforme a distribuição
# ------------------------------------------------------------
etapa "[1/6] Instalando a toolchain AVR..."

GERENCIADOR=""
for g in apt-get dnf pacman zypper; do
    if command -v "$g" >/dev/null 2>&1; then GERENCIADOR="$g"; break; fi
done

case "$GERENCIADOR" in
    apt-get)
        aviso "Distribuição baseada em Debian/Ubuntu."
        sudo apt-get update
        sudo apt-get install -y \
            gcc make wget unzip git \
            gcc-avr binutils-avr avr-libc gdb-avr avrdude picocom \
            simavr libsimavr-dev libelf-dev
        ;;
    dnf)
        aviso "Distribuição baseada em Fedora/RHEL."
        sudo dnf install -y \
            gcc make wget unzip git \
            avr-gcc avr-binutils avr-libc avr-gdb avrdude picocom \
            elfutils-libelf-devel
        ;;
    pacman)
        aviso "Distribuição baseada em Arch."
        sudo pacman -Sy --needed --noconfirm \
            gcc make wget unzip git \
            avr-gcc avr-binutils avr-libc avr-gdb avrdude picocom \
            libelf
        ;;
    zypper)
        aviso "Distribuição baseada em openSUSE."
        sudo zypper --non-interactive install \
            gcc make wget unzip git \
            cross-avr-gcc cross-avr-binutils avr-libc cross-avr-gdb avrdude picocom \
            libelf-devel
        ;;
    *)
        erro "Não reconheci o gerenciador de pacotes desta distribuição."
        aviso "Instale manualmente: avr-gcc, avr-binutils, avr-libc, avr-gdb, avrdude,"
        aviso "picocom, simavr (com headers), gcc, make, wget, unzip, git — e rode este script de novo."
        exit 1
        ;;
esac

# ------------------------------------------------------------
# 2. simavr com headers (necessário para o cronômetro de ciclos
#    e para o terminal serial do simulador)
# ------------------------------------------------------------
etapa "[2/6] Verificando o simulador (simavr)..."

tem_simavr_dev() { [ -f /usr/include/simavr/sim_avr.h ] || [ -f /usr/local/include/simavr/sim_avr.h ]; }

if tem_simavr_dev; then
    aviso "OK: headers do simavr já presentes."
else
    aviso "Sua distribuição não empacota o simavr com headers; compilando ${SIMAVR_VERSION} do fonte..."
    TMP_SIM="$(mktemp -d)"
    if git clone --depth 1 --branch "$SIMAVR_VERSION" https://github.com/buserror/simavr.git "$TMP_SIM/simavr" 2>/dev/null \
       && make -C "$TMP_SIM/simavr/simavr" -j"$(nproc)" >/dev/null \
       && sudo make -C "$TMP_SIM/simavr/simavr" install PREFIX=/usr/local >/dev/null; then
        # o binário do simulador se chama run_avr no código-fonte
        if [ -x "$TMP_SIM/simavr/simavr/run_avr" ] && ! command -v simavr >/dev/null 2>&1; then
            sudo install -m 755 "$TMP_SIM/simavr/simavr/run_avr" /usr/local/bin/simavr
        fi
        sudo ldconfig
        aviso "OK: simavr instalado em /usr/local."
    else
        erro "Não consegui compilar o simavr."
        aviso "O build e a gravação na placa continuam funcionando; o cronômetro de ciclos"
        aviso "e o terminal serial do simulador ficarão indisponíveis."
    fi
    rm -rf "$TMP_SIM"
fi

# ------------------------------------------------------------
# 3. Acesso à porta serial
# ------------------------------------------------------------
etapa "[3/6] Liberando o acesso à porta serial do Arduino..."

GRUPO_SERIAL=""
for g in dialout uucp plugdev; do
    if getent group "$g" >/dev/null 2>&1; then GRUPO_SERIAL="$g"; break; fi
done

if [ -n "$GRUPO_SERIAL" ]; then
    sudo usermod -aG "$GRUPO_SERIAL" "$USER"
    aviso "OK: '$USER' adicionado ao grupo '$GRUPO_SERIAL'."
else
    erro "Não encontrei um grupo de porta serial (dialout/uucp)."
fi

# O brltty (leitor de tela em braile) captura conversores CH340 de clones
# do Arduino e impede a gravação. Só desativamos a regra dele para esses
# conversores — o programa continua instalado e funcional.
REGRA_BRLTTY=/usr/lib/udev/rules.d/85-brltty.rules
if [ -f "$REGRA_BRLTTY" ] && grep -q "1a86" "$REGRA_BRLTTY" 2>/dev/null; then
    aviso "Ajustando o brltty, que costuma capturar clones com chip CH340..."
    sudo sed -i 's/^\(ENV{PRODUCT}=="1a86\/7523.*\)/# \1/' "$REGRA_BRLTTY" || true
    sudo udevadm control --reload-rules 2>/dev/null || true
fi

# ------------------------------------------------------------
# 4. Include do ATmega328P (sintaxe do Microchip Studio)
# ------------------------------------------------------------
etapa "[4/6] Gerando include/m328Pdef.inc (sintaxe AVRASM2 -> GNU as)..."

convert_inc() {
    # Converte o m328Pdef.inc oficial (sintaxe AVRASM2) para GNU as,
    # mantendo todos os símbolos (DDRB, PORTB, UCSR0A, XL/XH/YL/YH/ZL/ZH...).
    sed -E \
        -e 's/^([[:space:]]*)\.equ[[:space:]]+([A-Za-z_][A-Za-z_0-9]*)[[:space:]]*=[[:space:]]*/\1.equ \2, /' \
        -e 's/^[[:space:]]*\.def[[:space:]]+([A-Za-z_][A-Za-z_0-9]*)[[:space:]]*=[[:space:]]*([Rr][0-9]+).*/#define \1 \2/' \
        -e 's/^([[:space:]]*)\.device/\1; .device/' \
        "$1"
}

TMPD="$(mktemp -d)"
if wget -q -O "$TMPD/dfp.atpack" "$DFP_URL" \
   && unzip -p "$TMPD/dfp.atpack" avrasm/inc/m328Pdef.inc > "$TMPD/m328Pdef.orig.inc" 2>/dev/null; then
    convert_inc "$TMPD/m328Pdef.orig.inc" > "$TMPD/conv.inc"
    {
        echo "; m328Pdef.inc convertido do pack oficial Microchip ATmega_DFP ${DFP_VERSION}"
        echo "; (sintaxe adaptada para o GNU assembler; nomes de registradores/bits idênticos)"
        cat "$TMPD/conv.inc"
        echo ""
        echo "; --- Aliases em minúsculas (o AVRASM2 é case-insensitive; o GNU as não) ---"
        sed -nE 's/^[[:space:]]*\.equ[[:space:]]+([A-Za-z_][A-Za-z_0-9]*)[[:space:]]*,.*/.equ \L\1\E, \1/p' "$TMPD/conv.inc" \
            | grep -vE '^\.equ ([a-z_0-9]+), \1$' || true
    } > "$PROJ_DIR/include/m328Pdef.inc"
    aviso "OK: gerado a partir do pack oficial (${DFP_VERSION})."
else
    aviso "Aviso: download do pack falhou; usando a cópia incluída no repositório."
fi
rm -rf "$TMPD"
[ -s "$PROJ_DIR/include/m328Pdef.inc" ] || { erro "include/m328Pdef.inc ausente."; exit 1; }

# ------------------------------------------------------------
# 5. Extensões do VS Code
# ------------------------------------------------------------
etapa "[5/6] Instalando extensões do VS Code..."

EXTENSOES=(
    microchip.mplab-extension-pack   # debugger, IO View e ferramentas MPLAB
    ms-vscode.cpptools               # usado na sessão de debug com o simavr
    eclipse-cdt.memory-inspector     # visualizador de memória
    rockcat.avr-support              # realce de sintaxe do assembly AVR
)

if command -v code >/dev/null 2>&1; then
    for ext in "${EXTENSOES[@]}"; do
        if code --install-extension "$ext" --force >/dev/null 2>&1; then
            aviso "OK: $ext"
        else
            erro "falhou: $ext (instale pela aba de extensões do VS Code)"
        fi
    done
else
    erro "Comando 'code' não encontrado."
    aviso "Instale o VS Code (https://code.visualstudio.com) e, se usar Flatpak/Snap,"
    aviso "habilite o comando no PATH. Depois instale: ${EXTENSOES[*]}"
fi

# ------------------------------------------------------------
# 6. Conferência final
# ------------------------------------------------------------
etapa "[6/6] Conferindo o ambiente..."

falhou=0
checa() {  # checa <descrição> <comando...>
    local desc="$1"; shift
    if "$@" >/dev/null 2>&1; then printf '    [ok]    %s\n' "$desc"
    else printf '    [FALTA] %s\n' "$desc"; falhou=1; fi
}

checa "montador/compilador AVR (avr-gcc)"     command -v avr-gcc
checa "utilitários binários (avr-objdump)"    command -v avr-objdump
checa "gravador da placa (avrdude)"           command -v avrdude
checa "depurador (avr-gdb)"                   command -v avr-gdb
checa "simulador (simavr)"                    command -v simavr
checa "headers do simavr (ciclos e serial)"   tem_simavr_dev
checa "monitor serial (picocom)"              command -v picocom
checa "make"                                  command -v make
checa "VS Code (code)"                        command -v code
checa "include do ATmega328P"                 test -s "$PROJ_DIR/include/m328Pdef.inc"

echo
if [ "$falhou" -eq 0 ]; then
    echo "============================================================"
    echo " Tudo pronto!"
else
    echo "============================================================"
    echo " Ambiente configurado, com as pendências marcadas acima."
fi
echo
echo " Próximos passos:"
echo "   1. FAÇA LOGOUT E LOGIN de novo (o acesso à porta serial só vale"
echo "      em uma sessão nova) — ou rode: newgrp ${GRUPO_SERIAL:-dialout}"
echo "   2. cd $PROJ_DIR && code ."
echo "   3. Ctrl+Shift+B para buildar, F5 para simular/debugar"
echo "      (o primeiro F5 baixa o backend MPLAB, ~1 GB — só uma vez)"
echo "   4. Conecte o Arduino e use a task 'Upload para o Arduino'"
echo "============================================================"
