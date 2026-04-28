# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Fantasy Tactics is a C++ turn-based tactical RPG (inspired by Final Fantasy Tactics) built on a custom engine called **cbit**. The repository contains two CMake targets: the `engine` static library (`namespace cbit`) and the `FantasyTactics` executable (no namespace).

**Core dependencies (all in `vendors/`):** OpenGL 3.3 via GLAD, GLFW 3.3, GLM, EnTT (ECS), spdlog, FreeType, stb_image, RapidJSON, Assimp.

## Build Commands

The project uses CMake + Ninja. On Linux, use the system CMake directly:

```bash
# Configure (debug)
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B cmake-build-debug

# Build
cmake --build cmake-build-debug

# Run the game
./cmake-build-debug/FantasyTactics

# Generate Doxygen HTML docs → docs/html/
doxygen Doxyfile
```

On Windows the project uses CLion-managed CMake/Ninja binaries (paths in `AGENTS.md`).

There is no automated test framework. The `assets/` and `engine/resources/` directories are copied into the build output directory automatically by CMake.

## Architecture

### Startup Flow

`game/src/main.cpp` constructs a `cbit::Application` (with `WindowConfig`), registers scenes with `SceneManager`, sets the active scene, then calls `app.run()`. The application loop calls `SceneManager::update(dt)` and `SceneManager::render(w, h)` each frame.

The currently active scene is `PlayScene`.

### Scene System

All game scenes derive from `cbit::Scene` (`engine/src/core/Scene.h`) and override:
- `initialize()` — load assets, create entities, register systems
- `update(float deltaTime)` — game logic; always call `Scene::update(deltaTime)` to tick ECS systems
- `render()` — custom draw calls; always call `Scene::render()` for built-in rendering

Scenes are registered by name in `main.cpp` and switched via `SceneManager::setActiveScene("SceneName")`. Access `SceneManager` from within a scene via `_sceneManager`.

Current game scenes live in `game/src/scenes/`: `MenuScene`, `PlayScene`, `TestScene`, `MeshScene`.

### Entity-Component System (ECS)

Each `Scene` owns an `EntityComponentSystem` (`engine/src/ecs/EntityComponentSystem.h`), accessed via `getWorld()`. The ECS wraps EnTT's `entt::registry`.

**Creating entities** — `addComponent` is chainable:
```cpp
auto player = getWorld().createGameObject("Player")
    .addComponent<cbit::TransformComponent>()
    .addComponent<cbit::SkinnedMeshComponent>("assets/models/character.glb")
    .addComponent<cbit::AnimatorComponent>();
```

**Looking up entities by name:**
```cpp
auto obj = getWorld().getGameObject("Player");
if (obj.getEntity() != entt::null) { /* found */ }
```

**All available components** are declared in `engine/src/ecs/Components.h`, grouped by category:
- **Default:** `TagComponent`, `IdComponent`, `TransformComponent`
- **Graphics:** `TextureComponent`, `ShaderOverrideComponent`, `SkyboxComponent`, `BaseColorComponent`, `ModelOffsetComponent`
- **Mesh:** `CubeComponent`, `QuadComponent`, `SphereComponent`, `CircleComponent`, `EllipsoidComponent`, `MeshComponent`, `SkinnedMeshComponent`, `AnimatorComponent`
- **Camera:** `CameraComponent` (Perspective/Orthographic/Isometric), `ActiveCameraComponent`
- **Lighting:** `DirectionalLightComponent`, `PointLightComponent`, `SpotLightComponent`
- **UI:** `UIAnchorComponent`, `ButtonComponent`, `UIColorRectangleComponent`, `UIImageComponent`, `UITextComponent`, `UIPointerCallbacksComponent`, `UIPointerState`

**Built-in ECS systems** (run automatically each frame inside `EntityComponentSystem`): `MeshRenderSystem`, `AnimationSystem`, `UISystem`.

**Custom game-side systems** implement `cbit::ISystem` (`engine/src/ecs/ISystem.h`) with `update(registry, dt)` and optional `render(registry)`, then are registered in `initialize()`:
```cpp
getWorld().addSystem<MySystem>(/* constructor args */);
```

Current game systems in `game/src/systems/`: `TurnControlSystem` (turn-order and active-unit tracking), `GridMovementSystem` (grid-based character movement), `DebugTransformSystem` (debug overlays).

**Game-side components** (e.g., `TurnControlComponent`, `GridMovementComponent`) are defined in their system's header file, not in `Components.h`.

### Input

Use `cbit::Input` (include `core/Input.h`) with key constants from `cbit::Keyboard`:
```cpp
cbit::Input::isKeyPressed(cbit::Keyboard::C)  // true for exactly one frame on press
cbit::Input::isKeyDown(cbit::Keyboard::Up)    // true every frame while held
```

### Animation System

`AnimatorComponent` holds a map of named `AnimationClip` objects and drives `SkinnedMeshComponent` via the built-in `AnimationSystem`.

- `autoState = true` — `AnimationSystem` automatically switches between an `"idle"` and `"walking"` clip based on whether `GridMovementComponent` is moving. Use this for player/enemy characters.
- `autoState = false` — the active clip is controlled manually by setting `animator.activeClip`. Use this for entities with a single looping animation (e.g., the fox).

Clips are loaded by name from a GLB file; the clip name must match the animation track name inside the file.

### Rendering Pipeline

`MeshRenderSystem` iterates entities with mesh + transform components and draws them. Shaders are loaded at runtime from the `resources/shaders/` directory relative to the binary. Use `ShaderOverrideComponent` on an entity to select a non-default shader (paths are relative to the binary output directory). The active camera entity is identified by `ActiveCameraComponent`; only one should exist at a time.

`ModelOffsetComponent` applies a local-space position/rotation/scale offset during rendering, used to correct coordinate-system mismatches baked into imported assets.

Skybox: call `Scene::setSkyboxTexture(path, radius)` in `initialize()`.

### Shader Catalog (`engine/resources/shaders/`)

Key shaders: `default`, `basic`, `cel_shading`, `cel_shading_texture`, `lighting_blinn-phong`, `lighting_dir_point_spot`, `skinned` (skeletal animation), `sky`, `text`, `ui`, `color_ui`.

## Coding Conventions

- C++17; 4-space indentation; braces on same line; spaces around operators.
- Classes and files use **PascalCase** (`PlayScene`, `TurnControlSystem`).
- Private/protected member variables use a **leading underscore** (`_sceneManager`, `_registry`).
- Engine code lives under `namespace cbit`; game code has no namespace prefix.
- Prefer full words in identifiers; avoid abbreviations unless standard (e.g., `dt`, `UV`).
- Commit messages are short, imperative, sentence case: `Add animated player character`.
