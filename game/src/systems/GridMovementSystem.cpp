/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#include "GridMovementSystem.h"

#include "core/Input.h"
#include "ecs/Components.h"

void GridMovementSystem::update(entt::registry &registry, float deltaTime) {
    int directionX = 0;
    int directionZ = 0;

    if (cbit::Input::isKeyPressed(cbit::Keyboard::W)) {
        directionZ -= 1;
    } else if (cbit::Input::isKeyPressed(cbit::Keyboard::A)) {
        directionX -= 1;
    } else if (cbit::Input::isKeyPressed(cbit::Keyboard::S)) {
        directionZ += 1;
    } else if (cbit::Input::isKeyPressed(cbit::Keyboard::D)) {
        directionX += 1;
    }

    const auto view = registry.view<GridMovementComponent, cbit::TransformComponent>();

    for (const auto entity: view) {
        auto &[x, z, moveCooldown]= view.get<GridMovementComponent>(entity);
        auto &transform = view.get<cbit::TransformComponent>(entity);

        moveCooldown -= deltaTime;
        if (moveCooldown > 0.0f) {
            continue;
        }

        const int newX = x + directionX;
        const int newZ = z + directionZ;
        if (directionX != 0 || directionZ != 0) {
            if (newX >= 0 && newX < _gridWidth && newZ >= 0 && newZ < _gridHeight) {
                x = newX;
                z = newZ;
                transform.position = _gridToWorldFunction(x, z);
                moveCooldown = 0.2f; // Cooldown time between moves
            }
        }
    }

}


