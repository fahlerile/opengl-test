cmake_minimum_required(VERSION 3.18.4)
project(opengl-triangle)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(triangle src/main.cpp)

target_include_directories(triangle PUBLIC lib/include)

target_link_libraries(triangle opengl32.lib)
