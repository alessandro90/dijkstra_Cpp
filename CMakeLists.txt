cmake_minimum_required(VERSION 3.0.0)
project(dijkstra)
 
include_directories(src/include)
 
set(SOURCES
    src/main.cpp
    src/graph.cpp
    src/dijkstra.cpp
    src/io.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -O2)
endif(MSVC)

add_executable(dijkstra ${SOURCES})
target_link_libraries(dijkstra sfml-window sfml-graphics sfml-system)