cmake_minimum_required(VERSION 3.17)
project(matrices)

set(CMAKE_CXX_STANDARD 14)

add_executable(matrices main_freeglut.cpp glut_ply.cpp glut_ply.h cgmath.cpp cgmath.h)

target_link_libraries(matrices -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3 -lGLEW32)