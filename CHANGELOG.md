# Changelog

## 2026-03-29

### Added
- Added a top-level changelog for tracking notable repository changes.
- Added `ModelOffsetComponent` in [engine/src/ecs/Components.h](/C:/Users/User/Developments/FantasyTactics/engine/src/ecs/Components.h) to support per-entity render-time mesh calibration without polluting gameplay transforms.
- Added per-entity texture flip control to `TextureComponent` for assets that should not use the engine's default vertical flip path.
- Extended `GridMovementComponent` with configurable turn-axis and persistent position offset data for model-specific movement alignment.
- Added `TurnControlSystem` and `TurnControlComponent` in [game/src/systems/TurnControlSystem.h](/C:/Users/User/Developments/FantasyTactics/game/src/systems/TurnControlSystem.h) and [game/src/systems/TurnControlSystem.cpp](/C:/Users/User/Developments/FantasyTactics/game/src/systems/TurnControlSystem.cpp) to manage which unit is currently allowed to move.
- Added a `PlayScene` turn-status UI label and control hint text so the active unit is visible on screen during play.
- Added animated player clip setup for `assets/animations/low-poly-character/idle.glb` and `assets/animations/low-poly-character/walking.glb` in [game/src/scenes/PlayScene.cpp](/C:/Users/User/Developments/FantasyTactics/game/src/scenes/PlayScene.cpp).

### Changed
- Integrated the low-poly fox as the `PlayScene` player character in [game/src/scenes/PlayScene.cpp](/C:/Users/User/Developments/FantasyTactics/game/src/scenes/PlayScene.cpp).
- Switched the fox asset from the original FBX workflow to the GLB asset at `assets/animations/low-poly-fox/low_poly_fox_by_pixelmannen_animated.glb`.
- Updated the fox setup to use:
  - `SkinnedMeshComponent`
  - `AnimatorComponent`
  - `TextureComponent`
  - `ModelOffsetComponent`
- Refactored fox transform handling so gameplay heading stays in the entity transform while GLB orientation correction is applied at render time.
- Updated `GridMovementSystem` to support configurable heading axes instead of assuming every model turns on Z.
- Updated `GridMovementSystem` to preserve per-entity movement offsets when snapping to new grid cells.
- Updated texture loading so vertical image flipping is selected per asset instead of globally for all textures.
- Configured the fox texture to load with vertical flip disabled to better match the GLB UV layout.
- Updated [AGENTS.md](/C:/Users/User/Developments/FantasyTactics/AGENTS.md) so build instructions match the CLion-managed CMake and Ninja toolchain used in this repository.
- Updated `PlayScene` so both the player and enemy participate in the turn order and can be cycled with keyboard input.
- Updated turn controls so `H` selects the previous controllable unit and `L` selects the next controllable unit instead of hard-switching to fixed characters.
- Updated `GridMovementSystem` so only the currently active turn-controlled unit responds to movement input.
- Updated the `player` unit to use a skinned GLB character with automatic idle and walking state selection.
- Updated `GridMovementSystem` animation selection so it prefers `walking` when available and falls back to legacy `run` clips for older assets.
- Updated `PlayScene` entity naming so the fox is tagged as `enemy` and the animated humanoid unit is tagged as `player`.

### Fixed
- Fixed the fox animation setup so `GridMovementSystem` no longer overrides the active clip with missing state names.
- Fixed the skinned fragment shader path so animated meshes use their assigned texture instead of the temporary green debug tint.
- Fixed fox spawn/move height consistency by introducing persistent movement offset handling.
- Fixed fox turning behavior by separating model calibration from the gameplay heading.
- Fixed inverted fox facing by calibrating the mesh orientation instead of changing movement logic repeatedly.
- Fixed upside-down fox rendering by correcting the GLB model offset rotation.
- Fixed the likely fox texture orientation issue caused by unconditional texture flipping.
- Fixed the new player character being effectively invisible by recalibrating its scale and render-time mesh offset.
- Fixed the new player character rendering upside down by correcting its model offset rotation.
- Fixed player tile placement drift by keeping the gameplay transform snapped to the grid and moving only the rendered mesh onto the tile top with `ModelOffsetComponent`.

### Verified
- Verified multiple successful builds with the CLion toolchain using:
  - `C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -G Ninja -S C:\Users\User\Developments\FantasyTactics -B C:\Users\User\Developments\FantasyTactics\cmake-build-debug`
  - `C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe --build C:\Users\User\Developments\FantasyTactics\cmake-build-debug`
