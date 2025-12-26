# Repository Guidelines

## Project Structure & Module Organization
- `engine/` holds the core rendering, ECS, math, and utilities code. Source lives in `engine/src/`.
- `game/` contains gameplay logic and scenes, with sources in `game/src/`.
- `assets/` stores art and branding assets copied into the build output.
- `vendors/` is for third-party libraries (GLFW, GLM, spdlog, glad, freetype, stb, rapidjson, entt).
- `docs/` contains generated documentation artifacts; the Doxygen config is at `Doxyfile`.

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

## Testing Guidelines
- No automated test framework is configured yet.
- If you add tests, propose a `tests/` folder and document how to run them in this file.

## Commit & Pull Request Guidelines
- Commit messages in history are short, imperative, and sentence case (e.g., `Refactor rendering to use ECS camera and lighting`).
- For PRs, include a concise summary, the motivation, and any relevant issue links.
- Add screenshots or short clips for visual or rendering changes.
- Note any new vendor dependencies or required setup steps in the PR description.
