---
name: implement-scene-feature
description: Use when changing a scene such as PlayScene. Helps keep scene files readable by separating orchestration, gameplay logic, and reusable helpers.
---

# Implement Scene Feature

Use this skill when working on a scene file or scene-specific feature.

## Goals
- Keep scene files readable
- Let scenes orchestrate rather than own every detail
- Move reusable or repeated logic into systems/helpers when appropriate
- Preserve clarity of initialization, update, and render flow

## Workflow

1. Inspect the scene responsibilities
    - What is initialization/setup?
    - What is gameplay logic?
    - What is rendering integration?
    - What is debug-only behavior?
    - What can be extracted?

2. Decide what belongs in the scene
   Keep in scene:
    - high-level entity setup
    - level composition
    - scene flow
    - simple wiring of systems

   Move out of scene when practical:
    - reusable gameplay behavior
    - repeated entity construction logic
    - utility math/helpers used elsewhere
    - large blocks of specialized logic
    - rendering implementation details

3. Keep the flow readable
   Prefer scene structure that is easy to scan:
    - initialize setup
    - update flow
    - render flow
    - clearly named helpers where useful

4. Keep scene code local and intentional
    - Avoid giant initialize methods when helpers improve clarity
    - Avoid hiding critical gameplay flow in too many tiny abstractions
    - Balance readability with simplicity

5. Document non-obvious flow
    - Explain why entities or systems are created
    - Explain scene-specific assumptions
    - Clarify any ordering requirements

6. Validate
    - Build the project
    - Run the scene
    - Verify camera, entity setup, input flow, and scene transitions if affected

## Reminders
- A scene is not the entire game architecture
- A scene should coordinate the experience, not absorb every system detail
- If logic is reusable, consider a system or helper
- If logic is specific to this scene, keeping it local may be correct