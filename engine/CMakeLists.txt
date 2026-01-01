# engine/CMakeLists.txt
add_library(engine
        # core
        src/core/Application.cpp
        src/core/Scene.cpp
        src/core/SceneManager.cpp
        src/core/Window.cpp

        # ecs
        src/ecs/EntityComponentSystem.cpp
        src/ecs/GameObject.cpp
        src/ecs/AnimationSystem.cpp
        src/ecs/MeshRenderSystem.cpp
        src/ecs/MeshRenderSystem.h
        src/ecs/UISystem.cpp

        # camera code
        src/graphics/cameras/Camera.cpp
        src/graphics/cameras/OrbitCamera.cpp
        src/graphics/cameras/IsometricCamera.cpp
        src/graphics/cameras/FirstPersonCamera.cpp

        # meshes
        src/graphics/meshes/Circle.cpp
        src/graphics/meshes/Cube.cpp
        src/graphics/meshes/Ellipsoid.cpp
        src/graphics/meshes/Mesh.cpp
        src/graphics/meshes/Quad.cpp
        src/graphics/meshes/SkinnedMesh.cpp
        src/graphics/meshes/Sphere.cpp

        # animation
        src/graphics/animation/AnimationClip.cpp
        src/graphics/animation/Animator.cpp

        # renderers
        src/graphics/renderers/ShaderProgram.cpp
        src/graphics/renderers/TextRenderer.cpp
        #        src/graphics/renderers/Texture2D.cpp

        # math
        src/math/Color.h
        src/math/Vector2.h
        src/math/Vector3.h

        # utilities
        src/utilities/Logger.cpp
        src/utilities/UUIDGenerator.cpp

        # UI components
        src/graphics/ui/Quad2D.cpp

        # OpenGL includes
        src/OpenGLInclude.h
        src/graphics/renderers/Texture.cpp
        src/graphics/renderers/Texture.h
        src/core/Input.cpp
        src/core/Input.h
        src/core/Keyboard.h
        src/core/Mouse.h
        src/core/Gamepad.h
        src/ecs/ISystem.h
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(GLAD_PATH ${CMAKE_SOURCE_DIR}/vendors/glad)
add_library(glad STATIC
        ${GLAD_PATH}/src/glad.c
)
target_include_directories(glad
        PUBLIC ${GLAD_PATH}/include
)

target_link_libraries(engine
        PUBLIC
        glad
        glfw3
        opengl32
        $<$<AND:$<BOOL:${MINGW}>,$<CONFIG:Debug>>:${CMAKE_SOURCE_DIR}/vendors/assimp/lib/libassimpd.dll.a>
        $<$<AND:$<BOOL:${MINGW}>,$<NOT:$<CONFIG:Debug>>>:${CMAKE_SOURCE_DIR}/vendors/assimp/lib/libassimp.dll.a>
        $<$<NOT:$<BOOL:${MINGW}>>:assimp>
        spdlog::spdlog
        Freetype::Freetype
        $<$<BOOL:${MINGW}>:ws2_32>
)
