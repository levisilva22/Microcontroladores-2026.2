# ============================================================
#  attach-arduino.ps1 — Conecta o USB do Arduino ao WSL
#
#  Detecta a placa (Arduino oficial, CH340, FTDI ou CP210x),
#  faz o bind (uma vez, requer admin) e o attach ao WSL.
#  Rode novamente sempre que replugar a placa, ou use -AutoAttach
#  para reconectar automaticamente a cada replug.
#
#  Uso:  powershell -ExecutionPolicy Bypass -File setup\attach-arduino.ps1 [-AutoAttach]
# ============================================================
param([switch]$AutoAttach)
$ErrorActionPreference = "Stop"

# VID:PID de placas/conversores comuns em Arduinos
$knownVids = @("2341", "2a03", "1a86", "0403", "10c4")

$lines = usbipd list | Out-String
$devices = @()
foreach ($line in ($lines -split "`r?`n")) {
    if ($line -match "^(\d+-\d+)\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})\s+(.+?)\s{2,}(\S.*)$") {
        $devices += [pscustomobject]@{
            BusId = $Matches[1]; Vid = $Matches[2].ToLower()
            Desc  = $Matches[4].Trim(); State = $Matches[5].Trim()
        }
    }
}
$arduinos = $devices | Where-Object { $knownVids -contains $_.Vid }

if (-not $arduinos) {
    Write-Host "Nenhum Arduino detectado. Placas USB atuais:" -ForegroundColor Yellow
    usbipd list
    Write-Host "`nConecte a placa e rode de novo. Se ela aparece na lista acima com outro"
    Write-Host "chip serial, faca manualmente: usbipd bind --busid <ID>; usbipd attach --wsl --busid <ID>"
    exit 1
}
if ($arduinos.Count -gt 1) {
    Write-Host "Mais de uma placa encontrada:" -ForegroundColor Yellow
    $arduinos | Format-Table BusId, Vid, Desc, State
    $busid = Read-Host "Digite o BUSID desejado"
} else {
    $busid = $arduinos[0].BusId
    Write-Host ("Placa encontrada: {0} (busid {1})" -f $arduinos[0].Desc, $busid) -ForegroundColor Green
}

$dev = $devices | Where-Object BusId -eq $busid
if ($dev.State -notmatch "Shared|Attached") {
    Write-Host "==> bind (requer admin; aceite o UAC)..." -ForegroundColor Cyan
    Start-Process usbipd -ArgumentList "bind --busid $busid" -Verb RunAs -Wait
}

Write-Host "==> attach ao WSL..." -ForegroundColor Cyan
if ($AutoAttach) {
    Write-Host "    (modo auto-attach: mantenha esta janela aberta; Ctrl+C para sair)"
    usbipd attach --wsl --busid $busid --auto-attach
} else {
    usbipd attach --wsl --busid $busid
    Start-Sleep -Seconds 2
    Write-Host "==> Porta serial vista pelo WSL:" -ForegroundColor Green
    wsl -e bash -lc "ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null || echo '(nenhuma - aguarde 1-2s e verifique de novo)'"
}
