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

# set FreeType path
set(FREETYPE_PATH vendors/freetype)
include_directories(${FREETYPE_PATH}/include/freetype2)
link_directories(${FREETYPE_PATH}/lib)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${FREETYPE_PATH})

find_package(Freetype REQUIRED)

# set glad path (Move to engine CMakeLists.txt)
# This is commented out because glad is now included in the engine subdirectory
# If you want to use glad directly, uncomment the following lines:
#set(GLAD_PATH vendors/glad)
#include_directories(${GLAD_PATH}/include)

# set rapidjson path
set(RAPIDJSON_PATH vendors/rapidjson)
include_directories(${RAPIDJSON_PATH}/include)

# set stb_image path
set(STB_IMAGE_PATH vendors/stb)
include_directories(${STB_IMAGE_PATH})

# add resources folder
file(COPY engine/resources DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(engine)

add_subdirectory(game)