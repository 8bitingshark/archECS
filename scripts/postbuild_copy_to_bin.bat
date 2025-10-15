@echo off
setlocal

REM === Arguments ===
REM %1 -> Source file (TargetPath)
REM %2 -> Destination directory (bin folder)

if "%~1"=="" (
    echo [ERROR] Missing source path argument.
    exit /b 1
)

if "%~2"=="" (
    echo [ERROR] Missing destination directory argument.
    exit /b 1
)

set SRC=%~1
set DEST=%~2

REM === Create destination directory if not exists ===
if not exist "%DEST%" (
    mkdir "%DEST%"
)

REM === Copy file only if newer (/D) and overwrite (/Y) ===
xcopy /Y /D "%SRC%" "%DEST%" >nul

echo [PostBuild] Copied "%~nx1" to "%DEST%"
endlocal