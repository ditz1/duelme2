cmake_minimum_required(VERSION 3.10)

project(gen)

set(CMAKE_CXX_STANDARD 17)

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

include_directories(include)

if(EMSCRIPTEN)
    include_directories($ENV{EMSDK}/upstream/emscripten/cache/sysroot/include)
endif()

file(GLOB SOURCES src/*.cpp)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_executable(gen ${SOURCES})

if(EMSCRIPTEN)
    set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylibweb.a")
    target_link_libraries(gen ${RAYLIB_LIBRARY} -lwebsocket.js -std=c++17)
else()
    set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a")
    target_link_libraries(gen ${RAYLIB_LIBRARY} -lwinmm -lgdi32 -lopengl32)
endif()