cmake_minimum_required(VERSION 3.17)
project(E1_basico)

set(CMAKE_CXX_STANDARD 14)

#add_executable(E1_basico box.cpp)
#add_executable(E1_basico COpenGL.h COpenGL.cpp primitiva.h primitiva.cpp opengl01.cpp)
#add_executable(E1_basico viewports.cpp)
add_executable(E1_basico hello_triangle_indexed.cpp glad.c)

target_link_libraries(E1_basico -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3)