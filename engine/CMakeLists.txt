# engine/CMakeLists.txt
add_library(engine
        # core
        src/core/Application.cpp
        src/core/Scene.cpp
        src/core/SceneManager.cpp
        src/core/Window.cpp

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
        src/graphics/meshes/Sphere.cpp

        # renderers
        src/graphics/renderers/ShaderProgram.cpp
        src/graphics/renderers/TextRenderer.cpp
        src/graphics/renderers/Texture2D.cpp

        # utilities
        src/utilities/Logger.cpp

        # UI components
        src/graphics/ui/Quad2D.cpp
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
        spdlog::spdlog
        Freetype::Freetype
        $<$<BOOL:${MINGW}>:ws2_32>
)

