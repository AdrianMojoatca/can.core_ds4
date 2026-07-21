<#
.SYNOPSIS
  Produce a FLASHABLE .enc for the DS4 (933/NGMM) split image.

.DESCRIPTION
  Two things this does that the stock Keil out.bat enc does NOT:
    1. FILL the CORE flash region contiguously up to the FW base (0x38000) with
       0xFF before encrypting. This is REQUIRED: CORE_DS4_BUILD publishes the ABI
       info + API table at the fixed addresses 0x37000 / 0x37040 (via __at), which
       land in the SAME 32 KB flash sector (20: 0x30000-0x37FFF) as the tail of
       the CORE code, but separated by a ~8 KB gap. A non-contiguous second burst
       in an already-written sector makes the bootloader's flasher abort partway
       (~90%). Filling the gap makes sector 20 a single contiguous write, exactly
       like a normal (contiguous) monolith image -> flashes cleanly.
    2. Run the cygwin enc chain (hex2enc_933) via C:\cygwin\bin\bash.exe directly
       (NOT the Claude Bash tool, which is Git Bash / MSYS2). Requires cygwin fork
       to be healthy (rebaseall).

  Pipeline: srec_cat <hex> -fill 0xFF <FillStart> <FwBase> -> filled hex
            hex2enc_933 <filled hex>  -> 933.<name>.<ver>.enc  (in the hex dir)

  DS5 S1-S4: single MONOLITH image -> fill + enc.
  DS5 S5+  : combine core_ds4 + fw_frd37eu_ds4 first, then fill the CORE range +
             enc (this grows into combine_core_fw_ds4.ps1). FW (>= 0x38000) is a
             separate sector (21) and is NOT filled.
#>
param(
    [string]$Hex       = (Join-Path $PSScriptRoot '..\ds4_build\main_ngmm_release.hex'),
    [string]$Bash      = 'C:\cygwin\bin\bash.exe',
    # Contiguous-fill window: [FillStart, FwBase). Matches core_contract_ds4_image_layout.h
    # (CORE 0x8100.., FW base 0x38000). ABOUT (0x8000) is below FillStart and left alone.
    [uint32]$FillStart = 0x8100,
    [uint32]$FwBase    = 0x38000
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function ConvertTo-CygPath {
    param([string]$WinPath)
    $full  = [System.IO.Path]::GetFullPath($WinPath)
    $drive = $full.Substring(0, 1).ToLower()
    $rest  = ($full.Substring(2)) -replace '\\', '/'
    return "/cygdrive/$drive$rest"
}

if (-not (Test-Path $Bash)) { throw "cygwin bash not found: $Bash" }
$hexFull = [System.IO.Path]::GetFullPath($Hex)
if (-not (Test-Path $hexFull)) { throw "hex not found: $hexFull" }

$hexDir     = Split-Path -Parent $hexFull
$hexBase    = Split-Path -Leaf   $hexFull
$filledBase = 'ds4_flash_filled.hex'
$hexDirCyg  = ConvertTo-CygPath $hexDir
$fillStartHex = ('0x{0:X}' -f $FillStart)
$fwBaseHex    = ('0x{0:X}' -f $FwBase)

Write-Host "make_ds4_enc: fill [$fillStartHex,$fwBaseHex) + enc  <- $hexBase"

# Snapshot enc set before, so we can report exactly what was produced.
function Get-EncNames { Get-ChildItem -Path $hexDir -Filter '933*.enc' -File -ErrorAction SilentlyContinue | ForEach-Object { $_.Name } }
$before = @(Get-EncNames)

$cmd = "cd '$hexDirCyg' && rm -f '$filledBase'* 2>/dev/null; " +
       "srec_cat '$hexBase' -intel -fill 0xFF $fillStartHex $fwBaseHex -o '$filledBase' -intel && " +
       "hex2enc_933 '$filledBase'"

# Native-exe call; do not use 2>&1 (PS 5.1 wraps native stderr as errors). cygwin
# prints a cosmetic find_fast_cwd/mkpasswd warning to stderr - harmless.
$prev = $ErrorActionPreference; $ErrorActionPreference = 'Continue'
& $Bash -lc $cmd
$ErrorActionPreference = $prev

$after = @(Get-EncNames)
$new = @($after | Where-Object { $before -notcontains $_ })
$enc = if ($new.Count -gt 0) {
    $new | ForEach-Object { Get-Item (Join-Path $hexDir $_) } | Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1
} else {
    # name reused (overwrite) -> take the newest 933*.enc touched just now
    Get-ChildItem -Path $hexDir -Filter '933*.enc' -File | Sort-Object LastWriteTimeUtc -Descending | Select-Object -First 1
}
if (-not $enc) { throw "no 933*.enc produced in $hexDir (check cygwin fork / rebaseall)" }

Write-Host ""
Write-Host "ENC ready (flash this):" -ForegroundColor Green
Write-Host ("  {0}  ({1} bytes)" -f $enc.FullName, $enc.Length)
exit 0
