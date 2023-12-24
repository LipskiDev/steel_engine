cmake_minimum_required(VERSION 3.0.0)
project(steel_engine VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(glm_DIR includes/glm)
find_package(glm REQUIRED)


add_executable(steel_engine 
    src/main.cpp
    src/Shader.cpp 
    src/Shader.h
    src/stb_image.cpp
    src/stb_image.h
    lib/glad.h 
    lib/glad.c
)

target_link_libraries(steel_engine 
    glfw
    OpenGL::GL
    glm::glm
)