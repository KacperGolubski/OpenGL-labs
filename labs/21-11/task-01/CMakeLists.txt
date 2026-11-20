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

set(SCENE_ASSETS
        "${CMAKE_CURRENT_SOURCE_DIR}/vertex.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/fragment.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/terrain.obj"
        "${CMAKE_CURRENT_SOURCE_DIR}/rock.obj"
        "${CMAKE_CURRENT_SOURCE_DIR}/palm.obj"
        "${CMAKE_CURRENT_SOURCE_DIR}/bark.obj"
        "${CMAKE_CURRENT_SOURCE_DIR}/grass.obj"
)
add_custom_target(copy_scene_assets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SCENE_ASSETS} "$<TARGET_FILE_DIR:OpenGL_Learning>"
        VERBATIM
)
add_dependencies(OpenGL_Learning copy_scene_assets)
