#!/usr/bin/env bash
# ============================================================
#  setup-wsl.sh — Configura o ambiente AVR Assembly no WSL
#  Tutorial: assembly do ATmega328P no VS Code + WSL + MPLAB
#
#  Uso (dentro do Ubuntu/WSL, na raiz do repositório):
#      ./setup/setup-wsl.sh
#
#  O que este script faz:
#   1. Instala a toolchain AVR (avr-gcc, avrdude, simavr, gdb...)
#   2. Adiciona seu usuário ao grupo dialout (acesso à porta serial)
#   3. Gera o include m328Pdef.inc (sintaxe Microchip Studio) a partir
#      do pack oficial da Microchip (com fallback para a cópia local)
#   4. Instala as extensões do VS Code no lado WSL
#   5. Oferece executar a parte Windows (setup-windows.ps1)
# ============================================================
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJ_DIR="$REPO_DIR/projeto"
DFP_VERSION="3.6.299"   # versão fixa do pack = tutorial reprodutível
DFP_URL="https://packs.download.microchip.com/Microchip.ATmega_DFP.${DFP_VERSION}.atpack"

echo "==> [1/5] Instalando a toolchain AVR via apt..."
sudo apt-get update
sudo apt-get install -y make gcc gcc-avr binutils-avr avr-libc gdb-avr \
                        avrdude simavr libsimavr-dev picocom wget unzip

echo "==> [2/5] Adicionando '$USER' ao grupo dialout (porta serial)..."
sudo usermod -aG dialout "$USER"

echo "==> [3/5] Gerando include/m328Pdef.inc (sintaxe AVRASM2 -> GNU as)..."
convert_inc() {
    # Converte o m328Pdef.inc oficial (sintaxe AVRASM2) para GNU as.
    # Mantém todos os 687 símbolos (DDRB, PORTB, TCCR0A, ...).
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
    echo "    OK: gerado a partir do pack oficial (${DFP_VERSION})."
else
    echo "    Aviso: download do pack falhou; usando a cópia incluída no repositório."
fi
rm -rf "$TMPD"
[ -s "$PROJ_DIR/include/m328Pdef.inc" ] || { echo "ERRO: include/m328Pdef.inc ausente."; exit 1; }

echo "==> [4/5] Instalando extensões do VS Code (lado WSL)..."
# Atenção: dentro do WSL, o comando 'code' do PATH é o do WINDOWS (interop) e
# instalaria as extensões no lado errado. O CLI correto é o code-server do
# VS Code Server, que só existe depois que o VS Code abre a pasta no WSL.
WSL_EXTS=(
    microchip.mplab-extension-pack   # simulador, debugger, IO View
    ms-vscode.cpptools               # motor de debug do simavr (cppdbg)
    eclipse-cdt.memory-inspector     # visualizador de memória
    rockcat.avr-support              # realce de sintaxe AVR assembly
)
find_code_server() { ls "$HOME"/.vscode-server/bin/*/bin/code-server 2>/dev/null | head -1; }

CODE_SERVER="$(find_code_server)"
if [ -z "$CODE_SERVER" ]; then
    echo "    VS Code Server ainda não instalado no WSL; abrindo o projeto para instalá-lo..."
    (cd "$PROJ_DIR" && code . >/dev/null 2>&1 &)
    for _ in $(seq 1 60); do
        sleep 2
        CODE_SERVER="$(find_code_server)"
        [ -n "$CODE_SERVER" ] && break
    done
fi

if [ -n "$CODE_SERVER" ]; then
    for ext in "${WSL_EXTS[@]}"; do
        if "$CODE_SERVER" --install-extension "$ext" --force >/dev/null 2>&1; then
            echo "    OK: $ext"
        else
            echo "    FALHOU: $ext (instale pela aba Extensions do VS Code)"
        fi
    done
    echo "    (recarregue a janela do VS Code para ativar: Ctrl+Shift+P -> Developer: Reload Window)"
else
    echo "    Aviso: VS Code Server não encontrado."
    echo "    Abra a pasta no VS Code (cd $PROJ_DIR && code .) e rode este script de novo,"
    echo "    ou instale pela aba Extensions: ${WSL_EXTS[*]}"
fi

echo "==> [5/5] Parte Windows (usbipd + extensões locais do VS Code)..."
if [ "${1:-}" != "--no-windows" ] && command -v powershell.exe >/dev/null 2>&1; then
    read -r -p "    Executar setup/setup-windows.ps1 agora? [S/n] " resp
    if [[ ! "$resp" =~ ^[nN] ]]; then
        powershell.exe -ExecutionPolicy Bypass -File "$(wslpath -w "$REPO_DIR/setup/setup-windows.ps1")"
    fi
fi

echo
echo "============================================================"
echo " Pronto! Próximos passos:"
echo "   1. FECHE e reabra este terminal (grupo dialout só vale em sessão nova)"
echo "   2. cd $PROJ_DIR && code ."
echo "   3. Ctrl+Shift+B para buildar, F5 para simular/debugar"
echo "      (o primeiro F5 baixa o backend MPLAB, ~1 GB — só uma vez)"
echo "   4. Para gravar na placa: setup/attach-arduino.ps1 (no Windows)"
echo "      e depois a task 'Upload para o Arduino'"
echo "============================================================"
