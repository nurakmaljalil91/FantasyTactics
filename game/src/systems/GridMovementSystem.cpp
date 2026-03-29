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
#include "TurnControlSystem.h"
#include <cmath>

namespace {
    std::string pickClipName(
        const cbit::AnimatorComponent &animatorComponent,
        std::initializer_list<const char *> candidates) {
        for (const auto *candidate: candidates) {
            if (animatorComponent.clips.find(candidate) != animatorComponent.clips.end()) {
                return candidate;
            }
        }

        return {};
    }

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

    float getHeadingForAxis(const cbit::Vector3 &rotation, const GridTurnAxis axis) {
        switch (axis) {
            case GridTurnAxis::X:
                return rotation.x;
            case GridTurnAxis::Y:
                return rotation.y;
            case GridTurnAxis::Z:
            default:
                return rotation.z;
        }
    }

    void setHeadingForAxis(cbit::Vector3 &rotation, const GridTurnAxis axis, const float heading) {
        switch (axis) {
            case GridTurnAxis::X:
                rotation.x = heading;
                break;
            case GridTurnAxis::Y:
                rotation.y = heading;
                break;
            case GridTurnAxis::Z:
            default:
                rotation.z = heading;
                break;
        }
    }

    cbit::Vector3 addVector3(const cbit::Vector3 &lhs, const cbit::Vector3 &rhs) {
        return {
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
        };
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

    const bool wantsJump = cbit::Input::isKeyPressed(cbit::Keyboard::Space);
    const bool isMoving = directionX != 0 || directionZ != 0;

    const auto view = registry.view<GridMovementComponent, TurnControlComponent, cbit::TransformComponent>();

    for (const auto entity: view) {
        auto &gridMovement = view.get<GridMovementComponent>(entity);
        auto &turnControl = view.get<TurnControlComponent>(entity);
        auto &transform = view.get<cbit::TransformComponent>(entity);
        auto *animatorComponent = registry.try_get<cbit::AnimatorComponent>(entity);

        if (!turnControl.canMove) {
            continue;
        }

        if (animatorComponent && animatorComponent->autoState) {
            auto setClip = [&](const std::string &name, bool loop) {
                if (!name.empty() && animatorComponent->clips.find(name) != animatorComponent->clips.end()) {
                    animatorComponent->activeClip = name;
                    animatorComponent->loop = loop;
                }
            };

            const std::string idleClip = pickClipName(*animatorComponent, {"idle"});
            const std::string moveClip = pickClipName(*animatorComponent, {"walking", "run"});

            if (wantsJump) {
                setClip("jump", false);
            } else if (animatorComponent->activeClip == "jump" && animatorComponent->animator.isFinished()) {
                setClip(isMoving ? moveClip : idleClip, true);
            } else if (animatorComponent->activeClip != "jump") {
                setClip(isMoving ? moveClip : idleClip, true);
            }
        }

        if (!gridMovement.hasBaseRotation) {
            gridMovement.baseRotation = transform.rotation;
            gridMovement.targetHeading = getHeadingForAxis(gridMovement.baseRotation, gridMovement.turnAxis);
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
            gridMovement.targetHeading = getHeadingForAxis(gridMovement.baseRotation, gridMovement.turnAxis) + zOffset;
        }

        const float currentHeading = getHeadingForAxis(transform.rotation, gridMovement.turnAxis);
        const float newHeading = moveTowardsAngle(
            currentHeading,
            gridMovement.targetHeading,
            gridMovement.turnSpeed * deltaTime);
        transform.rotation.x = gridMovement.baseRotation.x;
        transform.rotation.y = gridMovement.baseRotation.y;
        transform.rotation.z = gridMovement.baseRotation.z;
        setHeadingForAxis(transform.rotation, gridMovement.turnAxis, newHeading);

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
                transform.position = addVector3(
                    _gridToWorldFunction(gridMovement.x, gridMovement.z),
                    gridMovement.positionOffset);
                gridMovement.moveCooldown = 0.2f; // Cooldown time between moves
            }
        }
    }

}
