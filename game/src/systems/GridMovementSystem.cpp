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
#include <cmath>

namespace {
    float normalizeAngle(const float degrees) {
        float result = std::fmod(degrees + 180.0f, 360.0f);
        if (result < 0.0f) {
            result += 360.0f;
        }
        return result - 180.0f;
    }

    float moveTowardsAngle(const float current, const float target, const float maxDelta) {
        const float delta = normalizeAngle(target - current);
        if (std::fabs(delta) <= maxDelta) {
            return target;
        }
        return current + (delta > 0.0f ? maxDelta : -maxDelta);
    }
}

void GridMovementSystem::update(entt::registry &registry, float deltaTime) {
    int directionX = 0;
    int directionZ = 0;

    if (cbit::Input::isKeyDown(cbit::Keyboard::W)) {
        directionZ += 1;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::A)) {
        directionX += 1;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::S)) {
        directionZ -= 1;
    } else if (cbit::Input::isKeyDown(cbit::Keyboard::D)) {
        directionX -= 1;
    }

    const auto view = registry.view<GridMovementComponent, cbit::TransformComponent>();

    for (const auto entity: view) {
        auto &gridMovement = view.get<GridMovementComponent>(entity);
        auto &transform = view.get<cbit::TransformComponent>(entity);

        if (!gridMovement.hasBaseRotation) {
            gridMovement.baseRotation = transform.rotation;
            gridMovement.targetRotationZ = gridMovement.baseRotation.z;
            gridMovement.hasBaseRotation = true;
        }

        if (directionX != 0 || directionZ != 0) {
            float zOffset = 0.0f;
            if (directionX == 1) {
                zOffset = -90.0f;
            } else if (directionX == -1) {
                zOffset = 90.0f;
            } else if (directionZ == 1) {
                zOffset = 180.0f;
            } else {
                zOffset = 0.0f;
            }
            gridMovement.targetRotationZ = gridMovement.baseRotation.z + zOffset;
        }

        const float newZRotation = moveTowardsAngle(
            transform.rotation.z,
            gridMovement.targetRotationZ,
            gridMovement.turnSpeed * deltaTime);
        transform.rotation.x = gridMovement.baseRotation.x;
        transform.rotation.y = gridMovement.baseRotation.y;
        transform.rotation.z = newZRotation;

        gridMovement.moveCooldown -= deltaTime;
        if (gridMovement.moveCooldown > 0.0f) {
            continue;
        }

        const int newX = gridMovement.x + directionX;
        const int newZ = gridMovement.z + directionZ;
        if (directionX != 0 || directionZ != 0) {
            if (newX >= 0 && newX < _gridWidth && newZ >= 0 && newZ < _gridHeight) {
                gridMovement.x = newX;
                gridMovement.z = newZ;
                transform.position = _gridToWorldFunction(gridMovement.x, gridMovement.z);
                gridMovement.moveCooldown = 0.2f; // Cooldown time between moves
            }
        }
    }

}


