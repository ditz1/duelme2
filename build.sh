#!/bin/bash

if [ $# -eq 0 ] || [ "$1" = "web" ]; then
    mkdir -p build
    cd build 
    emcmake cmake -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ..
    emmake make
    mkdir ../server/build
    cd ../server/build
    cmake ..
    make
elif [ "$1" = "clean" ]; then
    rm -rf build
elif [ "$1" = "native" ]; then
    mkdir -p build
    cd build
    cmake -G "Unix Makefiles" ..
    make
    cd ..
elif [ "$1" = "rebuild" ]; then
    rm -rf build
    mkdir build
    cd build
    cmake -G "Unix Makefiles" ..
    make
    cd ..
else
    echo "Invalid argument. Use 'native', 'clean', 'web', or 'rebuild'."
    exit 1
fi