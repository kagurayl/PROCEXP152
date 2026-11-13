param([string]$Configuration = 'Release', [string]$BuildDirectory = 'build\native-release')
$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path -Parent $PSScriptRoot
$taskVswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$taskVs = & $taskVswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $taskVs) { throw 'MSVC x64 build tools were not found.' }
$taskVc = Get-ChildItem -LiteralPath (Join-Path $taskVs 'VC\Tools\MSVC') -Directory | Sort-Object Name -Descending | Select-Object -First 1
$taskKits = Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10'
$taskSdk = Get-ChildItem -LiteralPath (Join-Path $taskKits 'Include') -Directory | Where-Object { Test-Path -LiteralPath (Join-Path $_.FullName 'km\ntddk.h') } | Sort-Object Name -Descending | Select-Object -First 1
if (-not $taskSdk) { throw 'Windows Driver Kit headers were not found.' }
$taskBin = Join-Path $taskVc.FullName 'bin\Hostx64\x64'
$taskSdkBin = Join-Path $taskKits "bin\$($taskSdk.Name)\x64"
$env:PATH = "$taskBin;$taskSdkBin;$env:PATH"
$env:INCLUDE = "$($taskVc.FullName)\include;$($taskSdk.FullName)\ucrt;$($taskSdk.FullName)\shared;$($taskSdk.FullName)\um"
$env:LIB = "$($taskVc.FullName)\lib\x64;$taskKits\Lib\$($taskSdk.Name)\ucrt\x64;$taskKits\Lib\$($taskSdk.Name)\um\x64"
$taskBuild = Join-Path $taskRoot $BuildDirectory
New-Item -ItemType Directory -Path (Join-Path $taskBuild 'temp') -Force | Out-Null
$env:TEMP = Join-Path $taskBuild 'temp'
$env:TMP = $env:TEMP
$env:_CL_ = '/Z7'
& cmake -S $taskRoot -B $taskBuild -G Ninja "-DCMAKE_BUILD_TYPE=$Configuration" '-DCMAKE_C_COMPILER=cl.exe' '-DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY'
if ($LASTEXITCODE) { throw 'CMake configuration failed.' }
& cmake --build $taskBuild --verbose
if ($LASTEXITCODE) { throw 'Build failed.' }

