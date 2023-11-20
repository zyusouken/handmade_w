@echo off

:::::::::::
:: BUILD ::
:::::::::::
mkdir ..\..\build
pushd ..\..\build
cl -FC -Zi ..\handmade\code\win32_handmade.cpp user32.lib Gdi32.lib
popd

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