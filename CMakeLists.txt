cmake_minimum_required(VERSION 3.27)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracer src/main.cpp
        src/vec3.h
        src/colour.h
        src/ray.h
        src/hittable.h
        src/sphere.h
        src/hittables.h
        src/rayTracer.h
        src/interval.h
        src/camera.h
        src/material.h)
