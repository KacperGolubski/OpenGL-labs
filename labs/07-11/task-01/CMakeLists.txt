cmake_minimum_required(VERSION 3.20)

project(OpenGL_Learning)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)

add_executable(OpenGL_Learning
        main.cpp
        Libraries/src/glad.c
        house.h
)

target_include_directories(OpenGL_Learning PRIVATE
        Libraries/include
)

target_link_libraries(OpenGL_Learning PRIVATE
        glfw
)