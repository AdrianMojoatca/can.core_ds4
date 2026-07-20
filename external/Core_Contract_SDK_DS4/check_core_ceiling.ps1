# Verifica soft-ceiling-ul CORE dupa build.
# Uz:  .\check_core_ceiling.ps1 [cale_catre_map]
param([string]$Map = "D:\ADI\DB3\Based_on_latest_core\DS4_CORE_SPLIT\core_ds4\build\ds4_build\main_ngmm_release.map")
$soft = 0x36000; $hard = 0x37000
if(-not (Test-Path $Map)){ Write-Host "Map negasit: $Map" -ForegroundColor Red; exit 2 }
$txt = Get-Content $Map -Raw
$m = [regex]::Match($txt, 'Load Region LR_IROM_APP1 \(Base: (0x[0-9A-Fa-f]+), Size: (0x[0-9A-Fa-f]+)')
if(-not $m.Success){ Write-Host "LR_IROM_APP1 negasit in $Map" -ForegroundColor Red; exit 2 }
$base=[Convert]::ToUInt32($m.Groups[1].Value,16); $size=[Convert]::ToUInt32($m.Groups[2].Value,16); $end=$base+$size
$freeSoft=$soft-$end; $freeHard=$hard-$end
"CORE flash: 0x{0:X} .. 0x{1:X}  ({2} bytes / {3:N1} KB)" -f $base,$end,$size,($size/1KB)
"Soft ceiling 0x{0:X}: {1} (spatiu {2} B / {3:N1} KB)" -f $soft, $(if($end -lt $soft){"OK"}else{"DEPASIT!"}), $freeSoft, ($freeSoft/1KB)
"Hard cap    0x{0:X}: {1} (spatiu {2} B / {3:N1} KB)" -f $hard, $(if($end -lt $hard){"OK"}else{"OVERFLOW!"}), $freeHard, ($freeHard/1KB)
if($end -ge $hard){exit 2}elseif($end -ge $soft){Write-Host "AVERTISMENT: peste soft-ceiling" -ForegroundColor Yellow; exit 1}else{Write-Host "OK" -ForegroundColor Green; exit 0}
