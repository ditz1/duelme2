cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(PLATFORM_WEB "PLATFORM_WEB")
else()
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

include_directories(include)

if(EMSCRIPTEN)
    include_directories(${EMSCRIPTEN_ROOT_PATH}/upstream/emscripten/system/include/emscripten)
    include_directories($ENV{EMSDK}/upstream/emscripten/cache/sysroot/include)
endif()

file(GLOB SOURCES src/*.cpp)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_executable(game ${SOURCES})

if(EMSCRIPTEN)
    set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylibwebmac.a")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s FORCE_FILESYSTEM=1")
    target_compile_definitions(game PRIVATE ${PLATFORM_WEB})
else()
    set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a")
endif()

target_link_libraries(game ${RAYLIB_LIBRARY} -lwebsocket.js)

if(NOT EMSCRIPTEN)
    target_link_libraries(game -lwinmm -lgdi32 -lopengl32)
endif()