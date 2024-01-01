cmake_minimum_required(VERSION 3.27)

project(FantasyTactics)

set(CMAKE_CXX_STANDARD 17)

# Set the output folder where your program will be created
include_directories(FantasyTactics PRIVATE include)

# add_subdirectory(lib/glfw)
link_directories(FantasyTactics PRIVATE lib)

if(NOT TARGET spdlog)
    # Stand-alone build
    set(spdlog_DIR "C:/Program Files (x86)/spdlog")
    find_package(spdlog REQUIRED)
endif()

# add assets folder
file(COPY src/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add source to this project's executable.
add_executable(
        FantasyTactics
        src/main.cpp
        src/glad.c
        src/shaderProgram.h
        src/shaderProgram.cpp
)

# add link libraries
#target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32)
target_link_libraries(FantasyTactics PRIVATE glfw3 opengl32 spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)