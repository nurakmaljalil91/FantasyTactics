cmake_minimum_required(VERSION 3.27)

project(FantasyTactics)

set(CMAKE_CXX_STANDARD 17)

# set sdl2 path
#set(SDL2_PATH vendors/SDL2-2.28.5/x86_64-w64-mingw32)
#include_directories(${SDL2_PATH}/include)
#link_directories(${SDL2_PATH}/lib)
#file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

# set glfw path
set(GLFW_PATH vendors/glfw)
include_directories(${GLFW_PATH}/include)
link_directories(${GLFW_PATH}/lib)

# set GLM path
set(GLM_PATH vendors/glm)
include_directories(${GLM_PATH}/include)

# set spdlog path
set(spdlog_PATH vendors/spdlog)
include_directories(${spdlog_PATH}/include)
link_directories(${spdlog_PATH}/lib)

# Set CMAKE_PREFIX_PATH to include spdlog_DIR
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${spdlog_PATH})

find_package(spdlog REQUIRED)

# set glad path
set(GLAD_PATH vendors/glad)
include_directories(${GLAD_PATH}/include)

# set rapidjson path
set(RAPIDJSON_PATH vendors/rapidjson)
include_directories(${RAPIDJSON_PATH}/include)

# set stb_image path
set(STB_IMAGE_PATH vendors/stb)
include_directories(${STB_IMAGE_PATH})

# add resources folder
file(COPY src/resources DESTINATION ${CMAKE_BINARY_DIR})

# Add source to this project's executable.
add_executable(
        FantasyTactics
        src/main.cpp
        ${GLAD_PATH}/src/glad.c
        src/graphics/Shader.cpp
        src/utilities/Logger.cpp
        src/graphics/Texture2D.cpp
        src/graphics/Camera.cpp
        src/graphics/FPSCamera.cpp
        src/graphics/FPSCamera.h
        src/graphics/OrbitCamera.cpp
        src/graphics/OrbitCamera.h
        src/graphics/Mesh.cpp
        src/graphics/Mesh.h
        src/graphics/Cube.cpp
        src/graphics/Cube.h
        src/graphics/Circle.cpp
        src/graphics/Circle.h
)

# add link libraries
#target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32)
target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32 spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)