@echo off
setlocal
cd /d "%~dp0"
set "SLN=%~dp0..\ComputacaoGrafica.sln"
echo Building "%SLN%" Configuration=Debug Platform=x64...

set "MSBUILD="
for /f "usebackq delims=" %%i in (`where msbuild 2^>nul`) do if not defined MSBUILD set "MSBUILD=%%i"

if not defined MSBUILD if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
  for /f "usebackq delims=" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
    if exist "%%i\MSBuild\Current\Bin\MSBuild.exe" set "MSBUILD=%%i\MSBuild\Current\Bin\MSBuild.exe"
  )
)

if not defined MSBUILD if exist "%ProgramFiles%\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" (
  set "MSBUILD=%ProgramFiles%\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
)

if not defined MSBUILD if exist "%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\IDE\devenv.com" (
  echo Using devenv.com...
  "%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\IDE\devenv.com" "%SLN%" /Build "Debug|x64"
  if errorlevel 1 goto build_failed
  goto run_exe
)

if not defined MSBUILD (
  echo MSBuild not found and devenv.com not found.
  echo Open Visual Studio Installer and ensure the MSBuild component is installed.
  goto build_failed
)

echo Using MSBuild: "%MSBUILD%"
"%MSBUILD%" "%SLN%" /p:Configuration=Debug /p:Platform=x64 /m
if errorlevel 1 goto build_failed

rem run the expected output from the solution's x64\Debug folder
:run_exe
set "EXE=%~dp0..\x64\Debug\ComputacaoGrafica.exe"
if not exist "%EXE%" (
  echo Executable not found: "%EXE%"
  pause
  exit /b 1
)
echo Running "%EXE%"
start "" /wait "%EXE%"
pause
endlocal
exit /b 0

:build_failed
echo Build failed.
pause
endlocal
exit /b 1
