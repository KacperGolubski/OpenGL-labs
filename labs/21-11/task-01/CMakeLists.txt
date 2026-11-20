cmake_minimum_required(VERSION 3.20)

project(OpenGL_Learning LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(OpenGL_Learning
        objfile.cpp
        CProgram.cpp    
        CMesh.cpp
        glad.c
)

target_include_directories(OpenGL_Learning PRIVATE
        Libraries/include
)

target_link_libraries(OpenGL_Learning PRIVATE
        glfw
        glm::glm
        OpenGL::GL
)

file(GLOB SHADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.glsl")
add_custom_target(copy_shaders
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER_FILES} "$<TARGET_FILE_DIR:OpenGL_Learning>"
        VERBATIM
)
add_dependencies(OpenGL_Learning copy_shaders)

add_custom_command(TARGET OpenGL_Learning POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/monkey.obj"
                "$<TARGET_FILE_DIR:OpenGL_Learning>/monkey.obj"
        VERBATIM
)
