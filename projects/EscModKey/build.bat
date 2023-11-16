@echo off
set "CURRENT_DIR=%CD%"
set "PATH_WORKSPACE_ROOT=%CURRENT_DIR%\..\..\"

if not exist "build" (
    mkdir "build"
)

cd build

if "%1"=="clean" (
    rmdir /s /q .
)

if exist "Makefile" (
    mingw32-make clean
) else (
    cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
)

if exist "compile_commands.json" (
    copy compile_commands.json "%PATH_WORKSPACE_ROOT%"
)

mingw32-make -j8

