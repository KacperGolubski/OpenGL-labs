cmake_minimum_required(VERSION 3.20)

project(OpenGL_Learning)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)

add_executable(OpenGL_Learning
        main.cpp
        Libraries/src/glad.c
        co-to-jest.h
)

target_include_directories(OpenGL_Learning PRIVATE
        Libraries/include
)

target_link_libraries(OpenGL_Learning PRIVATE
        glfw
)

# Keep runtime shaders next to the executable, including shader-only updates.
file(GLOB SHADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.glsl")
add_custom_target(copy_shaders
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OpenGL_Learning>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILES} "$<TARGET_FILE_DIR:OpenGL_Learning>"
        VERBATIM
)
add_dependencies(OpenGL_Learning copy_shaders)
