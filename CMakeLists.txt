cmake_minimum_required(VERSION 3.27)

project(FantasyTactics)

set(CMAKE_CXX_STANDARD 17)

# set sdl2 path
#set(SDL2_PATH vendors/SDL2-2.28.5/x86_64-w64-mingw32)
#include_directories(${SDL2_PATH}/include)
#link_directories(${SDL2_PATH}/lib)
#file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

# set glfw path
set(GLFW_PATH vendors/glfw-3.3.9.bin.WIN64)
include_directories(${GLFW_PATH}/include)
link_directories(${GLFW_PATH}/lib-mingw-w64)

# set glm path
set(GLM_PATH vendors/glm-0.9.9.8/glm)
include_directories(${GLM_PATH})

# set spdlog path
if (NOT TARGET spdlog)
    # Stand-alone build
    set(spdlog_DIR vendors/spdlog-1.12.0)
    find_package(spdlog REQUIRED)
endif ()

# set glad path
set(GLAD_PATH vendors/glad)
include_directories(${GLAD_PATH}/include)

# set rapidjson path
set(RAPIDJSON_PATH vendors/rapidjson-1.1.0)
include_directories(${RAPIDJSON_PATH}/include)

# set stb_image path
set(STB_IMAGE_PATH vendors/stb)
include_directories(${STB_IMAGE_PATH})

# add assets folder
file(COPY src/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add source to this project's executable.
add_executable(
        FantasyTactics
        src/main.cpp
        src/glad.c
        src/graphics/shader.h
        src/graphics/shader.cpp
        src/utilities/logger.cpp
        src/utilities/logger.h
        src/graphics/texture_2d.cpp
        src/graphics/texture_2d.h
)

# add link libraries
#target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32)
target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32 spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)