cmake_minimum_required(VERSION 3.17)
project(MarchingCubes)

set(CMAKE_CXX_STANDARD 14)

add_executable(MarchingCubes main.cpp
        AABB.h
        Camera.h
        DistanceField.h
        Frustum.h
        glut_ply.h glut_ply.cpp
        MarchinCubes.h
        mutable_priority_queue.h
        Plane.h
        Render.h
        Triangle.cpp
        Triangle.h
        Voxel.h
        vmath.h)

target_link_libraries(MarchingCubes -lOpenGL32 -lGLU32 -lfreeGLUT -lglfw3 -lGLEW32)