cmake_minimum_required(VERSION 3.10)
project(your_project_name)

set(CMAKE_CXX_STANDARD 17)

if (NOT BOOST_ROOT)
    message(WARNING "BOOST_ROOT is not set. Please provide it via -DBOOST_ROOT or as an environment variable.")
else()
    set(BOOST_ROOT ${BOOST_ROOT})
    message(STATUS "Using BOOST_ROOT: ${BOOST_ROOT}")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/stage/lib")
endif()

find_package(Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
else()
    message(FATAL_ERROR "Boost not found. Make sure BOOST_ROOT is set correctly.")
endif()

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
