if ($args.Count -eq 0 -or $args[0] -eq "native") {
    mkdir build -ErrorAction SilentlyContinue
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
    cd ..
} elseif ($args[0] -eq "clean") {
    Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
} elseif ($args[0] -eq "web") {
    mkdir build -ErrorAction SilentlyContinue
    cd build 
    emcmake cmake -DCMAKE_TOOLCHAIN_FILE="$env:EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ..
    emmake make
    cd ..
} elseif ($args[0] -eq "rebuild") {
    Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
    cd ..
}