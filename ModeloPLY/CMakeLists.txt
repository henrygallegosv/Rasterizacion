cmake_minimum_required(VERSION 3.17)
project(ModeloPLY)

set(CMAKE_CXX_STANDARD 14)

#add_executable(ModeloPLY main.cpp glut_ply.cpp glut_ply.h glad.c)
add_executable(ModeloPLY main_freeglut.cpp glut_ply.cpp glut_ply.h )

target_link_libraries(ModeloPLY -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3 -lGLEW32)
