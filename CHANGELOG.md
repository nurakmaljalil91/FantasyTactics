# Changelog

## 2026-03-29

### Added
- Added a top-level changelog for tracking notable repository changes.
- Added `ModelOffsetComponent` in [engine/src/ecs/Components.h](/C:/Users/User/Developments/FantasyTactics/engine/src/ecs/Components.h) to support per-entity render-time mesh calibration without polluting gameplay transforms.
- Added per-entity texture flip control to `TextureComponent` for assets that should not use the engine's default vertical flip path.
- Extended `GridMovementComponent` with configurable turn-axis and persistent position offset data for model-specific movement alignment.

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

### Fixed
- Fixed the fox animation setup so `GridMovementSystem` no longer overrides the active clip with missing state names.
- Fixed the skinned fragment shader path so animated meshes use their assigned texture instead of the temporary green debug tint.
- Fixed fox spawn/move height consistency by introducing persistent movement offset handling.
- Fixed fox turning behavior by separating model calibration from the gameplay heading.
- Fixed inverted fox facing by calibrating the mesh orientation instead of changing movement logic repeatedly.
- Fixed upside-down fox rendering by correcting the GLB model offset rotation.
- Fixed the likely fox texture orientation issue caused by unconditional texture flipping.

### Verified
- Verified multiple successful builds with the CLion toolchain using:
  - `C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -G Ninja -S C:\Users\User\Developments\FantasyTactics -B C:\Users\User\Developments\FantasyTactics\cmake-build-debug`
  - `C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe --build C:\Users\User\Developments\FantasyTactics\cmake-build-debug`
