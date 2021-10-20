cmake_minimum_required(VERSION 3.17)
project(Frustum)

set(CMAKE_CXX_STANDARD 20)

add_executable(Frustum
        AABox.h AABox.cpp
        FrustumG.h FrustumG.cpp
        frustummain.cpp
        Plane.h Plane.cpp
        Vec3.h Vec3.cpp)

target_link_libraries(Frustum -lOpenGL32 -lGLU32 -lfreeGLUT)
