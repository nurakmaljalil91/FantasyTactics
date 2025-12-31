# game/CMakeLists.txt
add_executable(FantasyTactics
        # scenes
        src/scenes/MenuScene.cpp

        src/main.cpp
        src/scenes/MeshScene.cpp
        src/scenes/PlayScene.cpp
        src/scenes/PlayScene.h
        src/scenes/TestScene.cpp
        src/scenes/TestScene.h
        src/systems/DebugTransformSystem.cpp
        src/systems/DebugTransformSystem.h
        src/systems/GridMovementSystem.cpp
        src/systems/GridMovementSystem.h
        src/scenes/MeshScene.cpp
        src/scenes/MeshScene.h
)
# force its .exe into the top-level build dir
set_target_properties(FantasyTactics PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# tell it where to find your engine headers:
target_include_directories(FantasyTactics
        PRIVATE
        ${CMAKE_SOURCE_DIR}/engine/src
)

# link to your engine
target_link_libraries(FantasyTactics
        PRIVATE
        engine
)
