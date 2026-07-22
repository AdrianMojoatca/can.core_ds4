<#
.SYNOPSIS
  TEST/bench: union the DS4 CORE + thin-FW images into one flashable .enc.

.DESCRIPTION
  Single-boot bench artifact: CORE (0x8100..0x37FFF) + thin FW (0x38000..0x3FEFF)
  merged into one image, the CORE region filled contiguous to the FW base (the
  sector-20 fix, see make_ds4_enc.ps1), then encrypted via hex2enc_933. Lets us
  flash one image to validate the split-compiled CORE->FW handoff without the
  two-stage flashing flow. Production would flash CORE.enc + FW.enc separately.

  One srec_cat does the whole merge+fill (it errors if CORE and FW overlap):
    srec_cat CORE.hex -intel FW.hex -intel -fill 0xFF 0x8100 0x38000 -o comb -intel
  then hex2enc_933 comb. Run cygwin tools directly (NOT the Git-Bash Claude tool);
  needs cygwin fork healthy (rebaseall).

.EXAMPLE
  powershell -NoProfile -ExecutionPolicy Bypass -File `
    "...\core_ds4\build\scripts\combine_core_fw_ds4.ps1"
#>
param(
    [string]$CoreHex = "D:\ADI\DB3\Based_on_latest_core\DS4_CORE_SPLIT\core_ds4\build\ds4_build\main_ngmm_release.hex",
    [string]$FwHex   = "D:\ADI\DB3\Based_on_latest_core\DS4_CORE_SPLIT\fw_frd37eu_ds4\build\ds4_build\main_ngmm_release.hex",
    [string]$OutDir  = "D:\ADI\DB3\Based_on_latest_core\DS4_CORE_SPLIT\core_ds4\build\ds4_build",
    [string]$Bash    = 'C:\cygwin\bin\bash.exe',
    [uint32]$FwBase  = 0x38000
)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function ConvertTo-CygPath { param([string]$p)
    $f=[IO.Path]::GetFullPath($p); return "/cygdrive/$($f.Substring(0,1).ToLower())$(($f.Substring(2)) -replace '\\','/')" }

foreach($f in @($CoreHex,$FwHex)){ if(-not(Test-Path $f)){ throw "hex not found: $f (rebuild it first)" } }
if(-not(Test-Path $Bash)){ throw "cygwin bash not found: $Bash" }

$combined  = Join-Path $OutDir 'ds4_combined.hex'
$coreFill  = Join-Path $OutDir 'core_filled.hex'
$srec = 'C:\cygwin\home\T\bin\srec_cat.exe'
$fillEnd=('0x{0:X}' -f $FwBase)
# srec_cat.exe called DIRECTLY (Windows fwd-slash paths) - bash /cygdrive is flaky here.
function Win2Fwd($p){ ([IO.Path]::GetFullPath($p)) -replace '\\','/' }
$coreF=Win2Fwd $CoreHex; $fwF=Win2Fwd $FwHex; $cfF=Win2Fwd $coreFill; $cbF=Win2Fwd $combined

Write-Host "combine_core_fw_ds4:"
Write-Host "  CORE: $CoreHex"
Write-Host "  FW  : $FwHex"
Remove-Item $coreFill,$combined -ErrorAction SilentlyContinue

# NOTE: filling during a 2-input merge conflicts ("contradictory 0x8100"), so do it in
# 2 steps: (1) fill the CORE region contiguous, (2) union the filled CORE with the FW.
# 1) fill CORE region [0x8100, FW base) contiguous
& $srec $coreF -intel -fill 0xFF 0x8100 $fillEnd -o $cfF -intel
if($LASTEXITCODE -ne 0 -or -not(Test-Path $coreFill)){ throw "srec_cat fill(CORE) failed ($LASTEXITCODE)" }
# 2) union filled CORE + FW (srec_cat errors on any overlap)
& $srec $cfF -intel $fwF -intel -o $cbF -intel
if($LASTEXITCODE -ne 0 -or -not(Test-Path $combined)){ throw "srec_cat union(CORE+FW) failed ($LASTEXITCODE) - overlap?" }

# 2) enc the combined image
$before = @(Get-ChildItem $OutDir -Filter '933*.enc' -File -ErrorAction SilentlyContinue | ForEach-Object Name)
$enc = "cd '$(ConvertTo-CygPath $OutDir)' && hex2enc_933 '$(Split-Path $combined -Leaf)'"
$prev=$ErrorActionPreference; $ErrorActionPreference='Continue'
& $Bash -lc $enc
$ErrorActionPreference=$prev

$new = @(Get-ChildItem $OutDir -Filter '933*.enc' -File | Sort-Object LastWriteTimeUtc -Descending)
if(-not $new){ throw "no 933*.enc produced (cygwin fork / rebaseall?)" }
$e = $new[0]
Write-Host ""
Write-Host "COMBINED ENC ready (flash this):" -ForegroundColor Green
Write-Host ("  {0}  ({1} bytes)" -f $e.FullName, $e.Length)
exit 0
