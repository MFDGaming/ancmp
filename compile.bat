@echo off

cd /D "%~dp0"

if not exist build-win32 (
    echo "Created build"
    mkdir build-win32
)
cd build-win32
cmake -G "MinGW Makefiles" ..
cmake --build .
cd ..