if [ $# -eq 0 ] || [ "$1" = "web" ]; then
    mkdir -p client/build
    cd client/build 
    emcmake cmake -DUSE_LOCAL=OFF -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ..
    emmake make
    mkdir ../../server/build
    cd ../../server/build
    cmake ..
    make
elif [ "$1" = "local" ]; then
    mkdir -p client/build
    cd client/build 
    emcmake cmake -DUSE_LOCAL=ON -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ..
    emmake make
    mkdir ../../server/build
    cd ../../server/build
    cmake ..
    make
elif [ "$1" = "clean" ]; then
    rm -rf client/build
    rm -rf server/build
elif [ "$1" = "native" ]; then
    mkdir -p client/build
    cd client/build
    cmake -G "Unix Makefiles" ..
    make
    cd ..
elif [ "$1" = "rebuild" ]; then
    rm -rf client/build
    mkdir client/build
    cd client/build
    cmake -G "Unix Makefiles" ..
    make
    cd ..
else
    echo "Invalid argument. Use 'native', 'clean', 'web', or 'rebuild'."
    exit 1
fi