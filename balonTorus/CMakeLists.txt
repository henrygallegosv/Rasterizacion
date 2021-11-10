cmake_minimum_required(VERSION 3.17)
project(balonTorus)

set(CMAKE_CXX_STANDARD 14)

add_executable(balonTorus ballAndTorusClipped.cpp hemisphere.cpp hemisphere.h shader.cpp shader.h torus.cpp torus.h vertex.h)

target_link_libraries(balonTorus -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3 -lGLEW32)