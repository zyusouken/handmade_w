@echo off

:::::::::::
:: BUILD ::
:::::::::::
call build.bat

:::::::::
:: RUN ::
:::::::::
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed. Aborting run.
    echo [ERROR] Build failed. Aborting run.
    echo [ERROR] Build failed. Aborting run.
    echo [ERROR] Build failed. Aborting run.
    echo [ERROR] Build failed. Aborting run.
    echo [ERROR] Build failed. Aborting run.
    exit /b %errorlevel%
)
..\..\build\win32_handmade.exe