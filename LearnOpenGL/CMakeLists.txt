cmake_minimum_required(VERSION 3.17)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

#add_executable(LearnOpenGL glad.c stb_image.h stb_image.cpp filesystem.h shader_s.h transformations.cpp)
add_executable(LearnOpenGL glad.c stb_image.h stb_image.cpp filesystem.h
        shader_s.h camera_mouse_zoom.cpp)

target_link_libraries(LearnOpenGL -lOpenGL32 -lGLEW32 -lglfw3)