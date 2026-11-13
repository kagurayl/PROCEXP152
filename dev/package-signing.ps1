param(
    [string]$InputSys = (Join-Path $PSScriptRoot '..\build\Release\PROCEXP152.sys'),
    [string]$KitVersion = '10.0.28000.0',
    [string]$OperatingSystems = '10_NI_X64,10_GE_X64'
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$inputFile = Get-Item -LiteralPath $InputSys
$kitRoot = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10'
$inf2cat = Join-Path $kitRoot "bin\$KitVersion\x86\Inf2Cat.exe"
$infverif = Join-Path $kitRoot "Tools\$KitVersion\x64\infverif.exe"
foreach ($toolPath in @($inf2cat, $infverif)) {
    if (-not (Test-Path -LiteralPath $toolPath)) { throw "Required tool missing: $toolPath" }
}
$packageRoot = Join-Path $projectRoot ('build\signing\Release-' + (Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
$driverDir = Join-Path $packageRoot 'PROCEXP152'
New-Item -ItemType Directory -Path $driverDir | Out-Null
$packedSys = Join-Path $driverDir 'PROCEXP152.sys'
Copy-Item -LiteralPath $inputFile.FullName -Destination $packedSys
$date = Get-Date -Format 'MM/dd/yyyy'
$inf = @'
[Version]
Signature="$WINDOWS NT$"
Class=System
ClassGuid={4d36e97d-e325-11ce-bfc1-08002be10318}
Provider=%ProviderName%
DriverVer=__DATE__,1.0.0.0
CatalogFile=PROCEXP152.cat
PnpLockdown=1

[DefaultInstall.NTamd64]
CopyFiles=DriverCopyFiles

[DefaultInstall.NTamd64.Services]
AddService=PROCEXP152,0,DriverService

[DestinationDirs]
DriverCopyFiles=13

[SourceDisksNames]
1=%DiskName%,,,

[SourceDisksFiles]
PROCEXP152.sys=1

[DriverCopyFiles]
PROCEXP152.sys

[DriverService]
DisplayName=%ServiceName%
ServiceType=1
StartType=3
ErrorControl=1
ServiceBinary=%13%\PROCEXP152.sys

[Strings]
ProviderName="PROCEXP152"
ServiceName="PROCEXP152"
DiskName="PROCEXP152 Driver Package"
'@
$infPath = Join-Path $driverDir 'PROCEXP152.inf'
Set-Content -LiteralPath $infPath -Value $inf.Replace('__DATE__', $date) -Encoding Unicode
& $infverif /w /v $infPath 2>&1 | Tee-Object (Join-Path $packageRoot 'infverif.log')
if ($LASTEXITCODE -ne 0) { throw 'INF validation failed.' }
& $inf2cat "/driver:$driverDir" "/os:$OperatingSystems" /uselocaltime /verbose 2>&1 | Tee-Object (Join-Path $packageRoot 'inf2cat.log')
if ($LASTEXITCODE -ne 0) { throw 'Catalog generation failed.' }
$ddf = @'
.OPTION EXPLICIT
.Set CabinetFileCountThreshold=0
.Set FolderFileCountThreshold=0
.Set FolderSizeThreshold=0
.Set MaxCabinetSize=0
.Set MaxDiskFileCount=0
.Set MaxDiskSize=0
.Set CompressionType=MSZIP
.Set Cabinet=on
.Set Compress=on
.Set CabinetNameTemplate=PROCEXP152.cab
.Set DiskDirectoryTemplate=.
.Set DestinationDir=PROCEXP152
'@
foreach ($name in @('PROCEXP152.inf','PROCEXP152.sys','PROCEXP152.cat')) {
    $ddf += "`r`n" + '"' + (Join-Path $driverDir $name) + '"'
}
$ddfPath = Join-Path $packageRoot 'PROCEXP152.ddf'
Set-Content -LiteralPath $ddfPath -Value $ddf -Encoding ASCII
Push-Location $packageRoot
try {
    & makecab.exe /F $ddfPath 2>&1 | Tee-Object 'makecab.log'
    if ($LASTEXITCODE -ne 0) { throw 'CAB generation failed.' }
    New-Item -ItemType Directory -Path 'verify-expanded' | Out-Null
    & expand.exe '-F:*' 'PROCEXP152.cab' 'verify-expanded' 2>&1 | Tee-Object 'expand.log'
    if ($LASTEXITCODE -ne 0) { throw 'CAB expansion failed.' }
    $expandedFiles = @(Get-ChildItem -LiteralPath 'verify-expanded' -File -Recurse)
    if ($expandedFiles.Count -ne 3) { throw 'Unexpected CAB member count.' }
    $members = foreach ($name in @('PROCEXP152.inf','PROCEXP152.sys','PROCEXP152.cat')) {
        $sourceHash = (Get-FileHash -LiteralPath (Join-Path $driverDir $name) -Algorithm SHA256).Hash
        $expanded = @($expandedFiles | Where-Object Name -eq $name)
        if ($expanded.Count -ne 1) { throw "Missing or duplicate CAB member: $name" }
        if ((Get-FileHash -LiteralPath $expanded[0].FullName -Algorithm SHA256).Hash -ne $sourceHash) { throw "CAB member mismatch: $name" }
        [ordered]@{name=$name; bytes=$expanded[0].Length; sha256=$sourceHash}
    }
    $inputHash = (Get-FileHash -LiteralPath $inputFile.FullName -Algorithm SHA256).Hash
    if ((Get-FileHash -LiteralPath $packedSys -Algorithm SHA256).Hash -ne $inputHash) { throw 'Packaged SYS differs from input.' }
    $cab = Get-Item -LiteralPath 'PROCEXP152.cab'
    [ordered]@{
        input=$inputFile.FullName
        input_sha256=$inputHash
        cab=$cab.FullName
        cab_bytes=$cab.Length
        cab_sha256=(Get-FileHash -LiteralPath $cab.FullName -Algorithm SHA256).Hash
        catalog_os=$OperatingSystems
        signed=$false
        verified_members=@($members)
    } | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath 'package-verification.json' -Encoding UTF8
    Write-Output "CAB ready: $($cab.FullName) ($($cab.Length) bytes)"
} finally { Pop-Location }

