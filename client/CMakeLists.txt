cmake_minimum_required(VERSION 3.10)
project(index)

set(CMAKE_CXX_STANDARD 17)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(PLATFORM_WEB "PLATFORM_WEB")
    
else()
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
endif()

include_directories(include)
include_directories(include/stage)
include_directories(include/player)
include_directories(include/raylib)
include_directories(include/physics)


if(EMSCRIPTEN)
    include_directories(${EMSCRIPTEN_ROOT_PATH}/upstream/emscripten/system/include/emscripten)
    include_directories(${EMSDK}/upstream/emscripten/system/include/emscripten)
    include_directories($ENV{EMSDK}/upstream/emscripten/cache/sysroot/include)
endif()

file(GLOB SOURCES
    "src/*.cpp"
    "src/stage/*.cpp"
    "src/player/*.cpp"
    "src/physics/*.cpp"
)

set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_executable(index ${SOURCES})
if(USE_LOCAL)
    target_compile_definitions(index PRIVATE LOCAL)
endif()

if(EMSCRIPTEN)
    if (APPLE)
        set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylibwebmac.a")
    elseif (Win32)
        set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylibweb.a")
    else()
        set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylibweb.a")
    endif()

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 \
                                                          -DPLATFORM_WEB \
                                                          -static \
                                                          --preload-file ../assets \
                                                          -s ASSERTIONS=1 \
                                                          -s WASM=1 \
                                                          -s ASYNCIFY \
                                                          -s FORCE_FILESYSTEM=1 \
                                                          -s NO_DISABLE_EXCEPTION_CATCHING \
                                                          -s INITIAL_MEMORY=268435456")
    target_compile_definitions(index PRIVATE ${PLATFORM_WEB})
else()
    set(RAYLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libraylib.a")
endif()

target_link_libraries(index ${RAYLIB_LIBRARY} -lwebsocket.js)

if(NOT EMSCRIPTEN)
    target_link_libraries(index -lwinmm -lgdi32 -lopengl32)
endif()
