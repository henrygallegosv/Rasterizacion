cmake_minimum_required(VERSION 3.17)
project(E1_basico)

set(CMAKE_CXX_STANDARD 14)

add_executable(E1_basico main.cpp)
target_link_libraries(E1_basico -lOpenGL32 -lGLU32 -lfreeGLUT)