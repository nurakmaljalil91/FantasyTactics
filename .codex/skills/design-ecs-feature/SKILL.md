---
name: design-ecs-feature
description: Use when adding or refactoring an ECS-based gameplay or engine feature. Helps identify the right components, systems, ownership boundaries, and update flow.
---

# Design ECS Feature

Use this skill when implementing a new feature in an ECS-oriented codebase.

## Goals
- Keep components data-oriented
- Keep systems focused
- Separate engine concerns from game concerns
- Keep ownership and update flow clear
- Avoid god objects and god systems

## Workflow

1. Read the relevant files first
    - Identify where the feature currently lives
    - Identify whether it belongs in engine or game
    - Note current components, systems, and scene orchestration

2. Define the feature boundary
    - What data belongs in components?
    - What behavior belongs in systems?
    - What should remain scene-level orchestration?

3. Choose the smallest safe structure
    - Reuse existing components/systems where reasonable
    - Add new components only if they represent stable data
    - Add new systems only if they have a focused responsibility

4. Check update flow
    - What systems must run before or after others?
    - Are there hidden dependencies?
    - Is the data flow easy to follow?

5. Check ownership and performance
    - Avoid unnecessary allocations in hot paths
    - Avoid hidden copies
    - Avoid mixing unrelated responsibilities

6. Implement
    - Keep the diff localized
    - Document non-obvious decisions
    - Preserve project conventions

7. Validate
    - Build the project
    - Run or test the affected scenario
    - Confirm the feature behaves correctly without breaking existing flow

## Reminders
- Components are mostly data
- Systems own behavior
- Scenes orchestrate high-level setup and flow
- Engine code should stay generic
- Game code should own tactical rules