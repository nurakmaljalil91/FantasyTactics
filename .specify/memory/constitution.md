<!--
Sync Impact Report
- Version change: 0.1.0 → 1.0.0
- Modified principles:
  - [PRINCIPLE_1_NAME] → Engine/Game Boundary & Reuse
  - [PRINCIPLE_2_NAME] → Rendering & Graphics Stack
  - [PRINCIPLE_3_NAME] → ECS with EnTT
  - [PRINCIPLE_4_NAME] → Testability, Determinism & Performance
  - [PRINCIPLE_5_NAME] → Cross-Platform Build & Repository Layout
- Added sections:
  - Development Workflow, Reviews & Quality Gates
- Removed sections:
  - None (template sections repurposed)
- Templates requiring updates:
  - ✅ .specify/templates/plan-template.md (Constitution Check will reference this constitution)
  - ✅ .specify/templates/spec-template.md (no conflicting assumptions; testing focus aligned)
  - ✅ .specify/templates/tasks-template.md (path conventions clarified in notes)
  - ⚠ .specify/templates/commands/placeholder.md (no changes required now; real command docs may be added later)
- Deferred TODOs:
  - TODO(CONSTITUTION_VERSION_HISTORY): Maintain historical versions in docs/CHANGELOG.md or dedicated governance doc if needed.
-->

# Fantasy Tactics Constitution

## Core Principles

### I. Engine/Game Boundary & Reuse

The repository is split into two primary domains:

- `engine/` is a reusable, game-agnostic engine.
  - MUST contain only general-purpose, reusable functionality: rendering
    primitives, ECS core utilities, resource loading, math, logging,
    time/loop control, platform abstraction and common utilities.
  - MUST NOT depend on Fantasy Tactics–specific concepts such as jobs,
    classes, abilities, story, specific units, or map layouts.
  - MUST be safe to reuse for future tactics or non-tactics games with
    minimal or no modification.
- `game/` is Fantasy Tactics–specific code.
  - MUST contain gameplay rules, units, abilities, jobs/classes, AI
    behaviors, balance data, UI flows, scenes, story, and scenario logic.
  - MAY build on and extend generic engine concepts via composition,
    configuration or data, but MUST NOT leak Fantasy Tactics–specific
    types back into `engine/`.

Non-negotiable rules:

- New functionality MUST default to `engine/` if it is general-purpose,
  reusable and not tied to the Fantasy Tactics domain.
- New functionality MUST go into `game/` if it expresses concrete game
  content or behavior (e.g., a "Fireball" ability, a "Knight" job, a
  specific campaign scene).
- Shared interfaces (e.g., `IRenderable`, ECS tag components for teams,
  input action abstractions) SHOULD live in `engine/` as long as they
  remain domain-agnostic.
- When in doubt, design the abstraction in `engine/` and plug concrete
  instances or data from `game/`.

Rationale: Keeping the engine game-agnostic maximizes reuse across
future games and reduces coupling. Fantasy Tactics remains a consumer of
its own engine, not the engine’s owner.

### II. Rendering & Graphics Stack

Fantasy Tactics is an OpenGL-based game using SDL2 (or GLFW in legacy
parts) for windowing and input and a set of vendor libraries to
implement rendering.

Non-negotiable rules:

- OpenGL is the primary rendering API.
  - Rendering APIs, GPU state management, shader compilation, textures,
    meshes, framebuffers and render passes MUST live in `engine/`.
  - Vendor libraries such as glad (loader), glm (math), stb_image
    (textures), and imgui (debug UI) are considered engine-level
    dependencies.
- SDL2/GLFW windowing and input handling MUST be encapsulated in
  `engine/` as a thin abstraction layer.
  - `game/` code interacts through engine-provided interfaces
    (e.g., input states/actions, window events), not raw SDL2/GLFW
    calls.
- The `engine/graphics` (or equivalent) module owns:
  - The render loop entry points and frame lifecycle.
  - Scene rendering hooks (e.g., callbacks/systems that game scenes
    register with ECS to draw themselves).
  - Camera, viewport, and projection utilities built on glm.
- `game/` code MAY define:
  - Concrete shaders, materials, sprite atlases, UI layouts and
    post-processing chains tuned for Fantasy Tactics.
  - Scene composition (which render systems are active, in which order).

Rationale: Centralizing the graphics stack in the engine makes
performance, portability and debugging more consistent, while allowing
`game/` to focus on expressing tactical gameplay and aesthetics.

### III. ECS with EnTT

The project uses EnTT as a strict Entity Component System (ECS)
backbone.

Non-negotiable rules:

- ECS is the primary architecture for runtime game state.
  - Game entities are identified by EnTT entity IDs.
  - Data lives in components; logic lives in systems.
  - Systems operate over views/registries, not via direct object graphs
    with hidden side effects.
- `engine/ecs` (or equivalent) MUST define:
  - Shared ECS helper utilities (e.g., registry ownership, entity
    factories, system scheduling, common tags).
  - Core component types that are generally useful across games (e.g.,
    transform, renderable, input tag, physics-related primitives if
    present).
  - Scene/level management abstractions that wire ECS worlds to
    rendering and input.
- `game/` MUST define domain-specific components and systems:
  - Components for jobs, stats, abilities, buffs/debuffs, AI state,
    turn order, map tiles, and scenario metadata.
  - Systems for turn resolution, AI decision-making, damage calculation
    and win/loss conditions.
- Scenes are ECS configurations:
  - Each scene is defined by an ECS world, with systems registered and
    entities spawned via data or scripts.
  - Scene transitions are implemented by replacing or reconfiguring ECS
    worlds, not by hard-wiring monolithic state machines.

Rationale: A data-oriented ECS architecture improves testability,
performance and reuse across different tactics games and scenarios.

### IV. Testability, Determinism & Performance

Fantasy Tactics MUST behave predictably and be maintainable over time.

Testability:

- `engine/` code MUST be written to be testable in isolation.
  - Pure functions for math, pathfinding, targeting, damage formulas
    MUST be unit-testable without rendering.
  - ECS systems SHOULD be testable by running against a test registry
    and asserting on resulting component state.
- `game/` logic for core combat rules (e.g., turn order, hit chance,
  damage, status effects) SHOULD have tests where practical, especially
  when behavior is non-trivial or easily regressed.
- New engine features of non-trivial complexity MUST include at least
  one automated test (unit or small integration) or a clear TODO with a
  tracking issue if temporarily skipped.

Determinism:

- Core gameplay logic MUST be deterministic given the same initial
  state and random seed.
  - RNG usage (for hit/miss, damage variance, AI choices) MUST be
    centralized and seedable.
  - ECS systems that depend on RNG MUST accept a seed or RNG provider
    to allow reproducible simulations.
- Floating point behavior SHOULD be made as stable as possible across
  platforms (e.g., via consistent math functions and avoiding
  unnecessary stateful GL calls in logic paths).

Performance:

- The engine targets a smooth 60 FPS on typical development hardware for
  benchmark scenes defined by the project.
- Performance-sensitive paths MUST:
  - Use data-oriented layouts and avoid excessive heap allocations.
  - Minimize per-frame allocations and virtual dispatch in inner loops.
  - Prefer ECS-friendly structures (e.g., contiguous component pools).
- Any change that noticeably degrades frame time or memory usage MUST
  include either an optimization, a regression note, or an explicit
  justification.

Rationale: Deterministic, testable gameplay is critical for debugging,
AI iteration and long-term maintainability. Performance awareness keeps
Fantasy Tactics responsive and scalable.

### V. Cross-Platform Build & Repository Layout

The project uses CMake as the primary build system and aims to be
cross-platform (Windows-focused today with future Linux/macOS support).

Non-negotiable rules:

- `CMakeLists.txt` at the root defines top-level configuration and
  delegates to `engine/` and `game/`.
  - `engine/` builds a reusable library target (e.g., `engine` or
    `ENGINE_LIB`).
  - `game/` builds an executable that links against the engine.
- New modules MUST integrate via CMake targets rather than ad-hoc build
  scripts.
- Platform-specific code (e.g., Windows-only APIs) MUST be isolated
  behind engine abstractions and guarded by CMake options or
  preprocessor checks, so the engine can be compiled on other platforms
  with reasonable effort.
- Dependencies sourced via `vendors/` or external package managers MUST
  be exposed as CMake targets or imported libraries, not via hard-coded
  include/lib paths in game code.

Repository layout expectations:

- Engine source lives under `engine/src/` with subfolders for core,
  ECS, graphics, utilities and similar concerns.
- Game source lives under `game/src/` with subfolders for scenes,
  gameplay systems, UI, and domain-specific utilities.
- Shared assets reside under `assets/` or `resources/` with clear
  ownership;
  - Engine-level assets (e.g., debug fonts) live in engine resources.
  - Game-level assets (maps, sprites, audio, story text) live in game
    resources.

Rationale: A clean CMake-based structure and clear layout keeps the
project portable, maintainable, and easy to onboard.

## Additional Constraints & Technology Stack

This section captures cross-cutting constraints that apply across both
`engine/` and `game/`.

- Language: C++ (C++17 or later as configured in CMake).
- Graphics stack: OpenGL (3.3 core or higher), glad, glm, stb_image,
  imgui (for debug and tooling), SDL2/GLFW for windowing/input.
- Logging: spdlog or equivalent engine-level logging facility.
- Documentation: Doxygen is used for API/reference documentation.

Constraints:

- Public engine APIs SHOULD be documented with Doxygen comments.
- Game-specific documentation (e.g., mechanics, balance notes) SHOULD
  live under `docs/` or game-specific readmes.
- Any new third-party library MUST be:
  - Added through CMake in a way that keeps engine reuse in mind.
  - Licensed compatibly with the project.
- Scripts (e.g., install helpers) MUST NOT become hidden build
  dependencies for core gameplay; the primary build MUST be achievable
  via documented CMake commands.

## Development Workflow, Reviews & Quality Gates

This section defines expectations for day-to-day development.

Branches & specifications:

- New work SHOULD be tracked via feature branches and corresponding
  `specs/[###-feature-name]/` documents when using the SpecKit tooling.
- `plan.md`, `spec.md` and `tasks.md` SHOULD reflect:
  - Whether changes affect `engine/`, `game/`, or both.
  - Any required migrations for save data or configuration.

Quality gates before merge:

- Code MUST compile on the primary supported platform (currently
  Windows/MinGW via CMake) with no new warnings introduced when
  reasonably avoidable.
- Engine changes of non-trivial complexity MUST include at least one of:
  - New or updated automated tests, or
  - A clear TODO with justification and a tracking issue.
- Game logic changes that affect core combat, AI or progression SHOULD
  be tested via either automated tests or documented manual test plans.
- Rendering or performance-sensitive changes SHOULD be validated via a
  local performance run (e.g., known heavy scenes) and any regressions
  documented.

Code review expectations:

- Every non-trivial change SHOULD be reviewed by at least one other
  contributor.
- Reviewers MUST check:
  - Engine/game boundary is respected.
  - EnTT ECS patterns are followed (no hidden global state, systems vs
    components separation).
  - Determinism and performance constraints are not violated without
    justification.
  - Documentation and comments are clear where behavior is non-obvious.

SpecKit alignment:

- The "Constitution Check" section of `.specify/templates/plan-template.md`
  MUST reference these principles when generating gates (e.g., checks
  for engine/game separation and ECS usage).
- Tasks generated from `.specify/templates/tasks-template.md` SHOULD
  group work items by domain (`engine/` vs `game/`) and user story, and
  MUST clearly indicate file paths.

## Governance

This constitution governs how the Fantasy Tactics repository—both the
reusable engine and the game built on top of it—is evolved over time.

Authority & scope:

- This constitution supersedes informal conventions for topics it
  addresses (engine/game boundaries, ECS usage, graphics stack,
  testability, performance, and workflow).
- Where the constitution is silent, maintainers MAY establish
  conventions via code review and documentation, but SHOULD consider
  whether those conventions merit a future amendment.

Amendment process:

- Any change to core principles (sections I–V) or Governance MUST be
  treated as a **governance change** and:
  - Be proposed in a dedicated pull request labeled clearly
    (e.g., `governance` or `constitution`).
  - Include a summary of the change, rationale, and impact on existing
    engine and game code.
  - Update this constitution’s version and Last Amended date.
- Non-semantic edits (typos, formatting, clarifications without behavior
  change) MAY be batched and treated as patch-level changes.

Versioning policy:

- **MAJOR** version bump (X.0.0) when:
  - Principles are added/removed in a way that breaks prior expectations
    (e.g., abandoning ECS, replacing OpenGL as the primary API, or
    changing the engine/game boundary model).
  - Governance rules change in ways that invalidate prior workflows.
- **MINOR** version bump (1.Y.0) when:
  - New principles or sections are added that extend governance,
    without contradicting existing rules.
  - Existing principles gain materially new constraints or guidance.
- **PATCH** version bump (1.0.Z) when:
  - Wording is clarified, examples added, or minor non-semantic
    corrections are made.

Compliance & review:

- Pull requests MUST note any intentional deviations from this
  constitution and, where appropriate, link to an open governance issue
  or amendment proposal.
- Periodic reviews (at least once per release cycle or significant
  milestone) SHOULD:
  - Re-evaluate the engine/game split based on new reuse candidates.
  - Confirm ECS usage remains consistent and maintainable.
  - Assess whether performance and determinism targets are still being
    met.

Future reuse & multi-game support:

- As new games adopt the engine:
  - Shared functionality discovered in `game/` SHOULD be promoted into
    `engine/` via refactors and corresponding constitution updates.
  - Engine changes that may break existing games MUST follow semantic
    versioning and provide migration notes.
- When engine evolution requires incompatible changes to existing games,
  maintainers MAY:
  - Branch engine versions per game, or
  - Introduce compatibility layers, but MUST document the chosen
    strategy in this constitution or related governance docs.

**Version**: 1.0.0 | **Ratified**: 2025-12-08 | **Last Amended**: 2025-12-08
