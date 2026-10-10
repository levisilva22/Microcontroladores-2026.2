# ============================================================
#  setup-windows.ps1 — Parte Windows do ambiente AVR + WSL
#
#  O que este script faz:
#   1. Verifica se o WSL está instalado (pré-requisito do tutorial)
#   2. Instala o usbipd-win (repassa o USB do Arduino para o WSL)
#   3. Instala as extensões do VS Code no lado Windows
#
#  Uso: clique-direito > "Executar com PowerShell", ou no terminal:
#      powershell -ExecutionPolicy Bypass -File setup\setup-windows.ps1
# ============================================================
$ErrorActionPreference = "Stop"

Write-Host "==> [1/3] Verificando o WSL..." -ForegroundColor Cyan
$distros = (wsl -l -q) -join ' '
if (-not $distros) {
    Write-Host "    ERRO: WSL sem distribuicao. Instale com: wsl --install -d Ubuntu" -ForegroundColor Red
    exit 1
}
Write-Host "    OK: WSL encontrado."

Write-Host "==> [2/3] Instalando usbipd-win (USB -> WSL)..." -ForegroundColor Cyan
$usbipd = winget list --id dorssel.usbipd-win --accept-source-agreements 2>$null | Select-String "usbipd"
if ($usbipd) {
    Write-Host "    OK: usbipd-win ja instalado."
} else {
    winget install --id dorssel.usbipd-win -e --accept-source-agreements --accept-package-agreements
    Write-Host "    OK: usbipd-win instalado (pode pedir confirmacao UAC)."
}

Write-Host "==> [3/3] Instalando extensoes do VS Code (lado Windows)..." -ForegroundColor Cyan
$exts = @(
    "ms-vscode-remote.remote-wsl",       # abre pastas dentro do WSL
    "microchip.mplab-extension-pack",    # pacote MPLAB completo
    "rockcat.avr-support"                # sintaxe AVR assembly
)
foreach ($e in $exts) {
    code --install-extension $e --force | Out-Null
    Write-Host "    OK: $e"
}

Write-Host ""
Write-Host "============================================================" -ForegroundColor Green
Write-Host " Parte Windows concluida!"
Write-Host " Proximo passo: no terminal do Ubuntu (WSL), rode:"
Write-Host "     ./setup/setup-wsl.sh    (se ainda nao rodou)"
Write-Host " Para conectar o Arduino ao WSL na hora de gravar:"
Write-Host "     setup\attach-arduino.ps1"
Write-Host "============================================================" -ForegroundColor Green
