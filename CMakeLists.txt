cmake_minimum_required(VERSION 3.27)
project(NoodleRT)

set(CMAKE_CXX_STANDARD 23)

add_executable(NoodleRT main.cpp
        object.cpp
        object.h
        camera.h
        perspective.cpp)
