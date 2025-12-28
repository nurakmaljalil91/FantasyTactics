cmake_minimum_required(VERSION 3.27)

project(FantasyTactics)

set(CMAKE_CXX_STANDARD 17)

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

# set entt path
include_directories(vendors/entt)

# set stb_image path
set(STB_IMAGE_PATH vendors/stb)
include_directories(${STB_IMAGE_PATH})

# set assimp path (prebuilt)
set(ASSIMP_PATH vendors/assimp)
include_directories(${ASSIMP_PATH}/include)
link_directories(${ASSIMP_PATH}/lib)

# add resources folder
file(COPY engine/resources DESTINATION ${CMAKE_BINARY_DIR})

# add assets folder
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# copy assimp runtime dll (prebuilt)
if (EXISTS ${ASSIMP_PATH}/bin/libassimp-6d.dll)
    file(COPY ${ASSIMP_PATH}/bin/libassimp-6d.dll DESTINATION ${CMAKE_BINARY_DIR})
elseif (EXISTS ${ASSIMP_PATH}/bin/libassimp-6.dll)
    file(COPY ${ASSIMP_PATH}/bin/libassimp-6.dll DESTINATION ${CMAKE_BINARY_DIR})
endif ()

add_subdirectory(engine)

add_subdirectory(game)
