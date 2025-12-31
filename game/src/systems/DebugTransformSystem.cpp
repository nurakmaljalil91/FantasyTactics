/**
 * @file
 * @brief
 * @details
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-31
 */

#include "DebugTransformSystem.h"

#include "ecs/Components.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

void DebugTransformSystem::update(entt::registry &registry, float deltaTime) {
    (void)deltaTime;

    const auto view = registry.view<DebugFollowComponent, cbit::TransformComponent>();

    for (const auto entity: view) {
        auto &follow = view.get<DebugFollowComponent>(entity);
        auto &transform = view.get<cbit::TransformComponent>(entity);

        if (follow.target == entt::null || !registry.valid(follow.target) ||
            !registry.all_of<cbit::TransformComponent>(follow.target)) {
            continue;
        }

        auto &targetTransform = registry.get<cbit::TransformComponent>(follow.target);

        cbit::Vector3 offset = follow.positionOffset;
        if (follow.offsetInLocalSpace) {
            const glm::quat targetRotation = glm::quat(glm::radians(targetTransform.rotation.toGLM()));
            const glm::vec3 rotatedOffset = targetRotation * offset.toGLM();
            offset = cbit::Vector3{rotatedOffset.x, rotatedOffset.y, rotatedOffset.z};
        }

        transform.position = cbit::Vector3{
            targetTransform.position.x + offset.x,
            targetTransform.position.y + offset.y,
            targetTransform.position.z + offset.z
        };

        if (follow.followRotation) {
            transform.rotation = cbit::Vector3{
                targetTransform.rotation.x + follow.rotationOffset.x,
                targetTransform.rotation.y + follow.rotationOffset.y,
                targetTransform.rotation.z + follow.rotationOffset.z
            };
        } else {
            transform.rotation = follow.rotationOffset;
        }

        if (follow.followScale) {
            transform.scale = targetTransform.scale;
        } else {
            transform.scale = follow.scale;
        }
    }
}
