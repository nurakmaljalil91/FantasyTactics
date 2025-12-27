# Repository Guidelines

## Project Overview
This repository contains the source code for a C++ game engine and a game built on top. The engine is designed to be 
modular, efficient, and extensible, with a focus on performance and ease of use. The game, Fantasy Tactics, is a 
turn-based strategy game that showcases the engine's capabilities.

The engine uses
- OpenGL 3.3 for rendering
- GLFW for window and input management
- GLM for mathematics
- EnTT for the entity-component-system architecture
- spdlog for logging
- CMake as the build system
- Doxygen for documentation generation
- Freetype for font rendering

Primary namespace for game engine code is `cbit`.

---

## Project Structure & Module Organization
- `engine/` holds the core rendering, ECS, math, and utilities code. Source lives in `engine/src/`. namespace cbit.
- `game/` contains gameplay logic and scenes, with sources in `game/src/`. no namespace prefix.
- `assets/` stores art and branding assets copied into the build output.
- `vendors/` is for third-party libraries (GLFW, GLM, spdlog, glad, freetype, stb, rapidjson, entt).
- `docs/` contains generated documentation artifacts; the Doxygen config is at `Doxyfile`.

## Important Files / Entry Points
- `engine/src/core/Application.cpp` is the main application loop and entry point for the engine.
- `engine/src/core/Window.cpp` manages window creation and input handling.
- `engine/src/core/SceneManager.cpp` handles scene transitions and updates.
- `engine/src/core/Scene.cpp` is the base class for all game scenes.
- `engine/src/ecs/EntityComponentSystem.cpp` and related files implement the ECS architecture.
- `game/src/main.cpp` is the entry point for the game executable.
- `CMakeLists.txt` is the root CMake build configuration file.

## Build, Test, and Development Commands
- `cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug` configures the build.
- `cmake --build cmake-build-debug` builds the engine and game targets.
- `cmake-build-debug\FantasyTactics.exe` runs the game executable.
- `npm run build` and `npm run run` wrap the CMake build and run steps.
- `doxygen Doxyfile` generates HTML docs in `docs/html`.

## Coding Style & Naming Conventions
- C++17 is the project standard; keep new code compatible.
- Use 4-space indentation, braces on the same line, and spaces around operators.
- Files and classes use PascalCase (e.g., `SceneManager.cpp`, `PlayScene`).
- Member variables commonly use a leading underscore (e.g., `_scenesManager`).
- Keep namespaces under `cbit` where applicable to match existing code.
- Prefer full words in names; avoid abbreviations unless widely recognized.

## Testing Guidelines
- No automated test framework is configured yet.
- If you add tests, propose a `tests/` folder and document how to run them in this file.

## Commit & Pull Request Guidelines
- Commit messages in history are short, imperative, and sentence case (e.g., `Refactor rendering to use ECS camera and lighting`).
- For PRs, include a concise summary, the motivation, and any relevant issue links.
- Add screenshots or short clips for visual or rendering changes.
- Note any new vendor dependencies or required setup steps in the PR description.
