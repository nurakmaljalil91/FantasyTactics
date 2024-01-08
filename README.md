# Fantasy Tactics

## Description

This is a tactical RPG game inspired by Final Fantasy Tactics. It is a turn-based game where the player controls a party of characters and fights against enemies. The player can move their characters around the map and attack enemies. The player can also use items and abilities to heal their characters or damage enemies. The player wins the game by defeating all enemies.

## Prerequisites

- JetBrains CLion as main IDE
- CMake 3.17.3 as build system
- MinGW 8.1.0 as compiler
- Ninja 1.10.0 as build tool
- Python 3.8.3 as scripting language
- Git 2.27.0 as version control system
- GLFW 3.3 as windowing library
- GLAD as OpenGL loader
- GLM as OpenGL mathematics library
- SPDLOG as logging library
- STB_IMAGE as image loading library

## Installation

- Clone the repository
```bash
git clone https://github.com/nurakmaljalil91/FantasyTactics.git
```
- Open the project in CLion 
- Download GLFW 3.3 from https://www.glfw.org/download.html and extract the files
- Copy the include file into the project directory and Copy the lib-mingw-w64 file into the project lib directory
- Go to GLAD website https://glad.dav1d.de/ and set the following options:
    - Language: C/C++
    - Specification: OpenGL
    - API: gl Version 3.3
    - Profile: Core
    - Options: Generate a loader
    - Click Generate
- Download the zip file and extract the files
- Clone Spdlog from https://github.com/gabime/spdlog
- Use this guide to install SPDLOG: https://github.com/gabime/spdlog/wiki/9.-CMake

### Installing Spdlog

```bash
cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX="C:\Program Files (x86)\spdlog" -G "MinGW Makefiles" -DCMAKE_CXX_STANDARD=17
```

```bash
cmake --build _builds --target install
```

> NOTE: Change the mingw toolchain in CLion to the one you installed instead of the default one

- Download GLM from https://github.com/g-truc/glm/releases/tag/0.9.9.8
- Extract the files and copy the glm folder into the project include directory
- Git clone STB_IMAGE from

```bash
git clone https://github.com/nothings/stb.git
```

- Copy the stb_image.h file into the project include directory
- You do not need other header files from the stb repository