---
name: write-opengl-shader
description: Use when creating or modifying an OpenGL shader or its integration. Helps define the visual goal, required data, shader stages, and engine-side hookups clearly.
---

# Write OpenGL Shader

Use this skill when implementing a new shader or modifying an existing one.

## Goals
- Produce a clear visual effect
- Keep GLSL readable
- Make engine-side integration explicit
- Document assumptions about inputs, coordinate spaces, and state

## Workflow

1. Define the visual goal
    - What should the player see?
    - Is this effect for terrain, units, water, UI, outlines, or post-process?
    - Is the goal readability, atmosphere, or feedback?

2. Define required inputs
    - Vertex attributes
    - Uniforms
    - Textures
    - Material properties
    - Camera/light data
    - Any render-state assumptions

3. Define stage responsibilities
    - What belongs in vertex shader?
    - What belongs in fragment shader?
    - Do not push work into the wrong stage without reason

4. Check coordinate spaces
    - Object space
    - World space
    - View space
    - Clip space
    - Normal space
      Make assumptions explicit

5. Keep the shader readable
    - Use clear names
    - Add concise comments for non-obvious math
    - Avoid overcomplicated one-liners

6. Define integration requirements
    - What uniforms must C++ set?
    - What textures must be bound?
    - What toggles or material flags are required?
    - What render state must be enabled or disabled?

7. Validate visually
    - Confirm the effect works from gameplay camera distance
    - Confirm it supports tactical readability
    - Check for artifacts, incorrect normals, wrong spaces, depth issues, and blending issues

## Reminders
- Tactical readability matters more than flashy complexity
- Expensive effects should be justified
- A good shader includes good integration notes
- If the problem looks visual, verify whether the root cause is actually mesh data, transforms, or GL state