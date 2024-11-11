cmake_minimum_required(VERSION 3.10)
project(your_project_name)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(BOOST_ROOT "/Users/daniel/boost")
    set(BOOST_LIBRARYDIR "/Users/daniel/boost/stage/lib")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BOOST_ROOT "/home/daniel/boost")
    set(BOOST_LIBRARYDIR "/home/daniel/boost/stage/lib")
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads)

include_directories(inc ${Boost_INCLUDE_DIRS})
include_directories(inc/)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.cc")

file(GLOB_RECURSE HEADERS "inc/*.h" "inc/*.hpp" "inc/*.hh")

add_executable(program ${SOURCES} ${HEADERS})

target_link_libraries(program ${Boost_LIBRARIES} ${RAYLIB_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/program
)
