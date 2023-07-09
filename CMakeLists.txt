# cmake .. -D CMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

cmake_minimum_required(VERSION 3.18.4)
project(opengl-triangle)

set(
  SOURCES
  src/main.cpp
  src/ShaderProgram.cpp
)

add_executable(triangle ${SOURCES})

target_include_directories(triangle PUBLIC lib/include)
target_include_directories(triangle PUBLIC src/include)

find_library(
  GLFW_LIB
  NAMES glfw3
  # HINTS "${CMAKE_SOURCE_DIR}/lib"
  # NO_DEFAULT_PATH
)
if(NOT GLFW_LIB)
  message(FATAL_ERROR "GLFW library not found")
endif()

find_library(
  GLEW_LIB
  NAMES glew32s
  # HINTS "${CMAKE_SOURCE_DIR}/lib"
  # NO_DEFAULT_PATH
)
if(NOT GLEW_LIB)
  message(FATAL_ERROR "GLEW library not found")
endif()

target_link_libraries(
  triangle
  ${GLFW_LIB}
  ${GLEW_LIB}
  opengl32
  gdi32
  -static  # dll error if there is no this option (https://stackoverflow.com/questions/69545451/cant-cross-compile-c-files-in-mingw-properly)
)
