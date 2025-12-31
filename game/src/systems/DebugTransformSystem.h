/**
 * @file    DebugTransformSystem.h
 * @brief   Debug transform follower system.
 * @details Spawns or updates a debug mesh to follow a target entity's transform.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-31
 */

#ifndef FANTASYTACTICS_DEBUGTRANSFORMSYSTEM_H
#define FANTASYTACTICS_DEBUGTRANSFORMSYSTEM_H

#include "ecs/ISystem.h"
#include "math/Vector3.h"
#include "entt/entt.hpp"

/**
 * @struct DebugFollowComponent
 * @brief  Debug the helper component to follow another entity's transform.
 */
struct DebugFollowComponent {
    entt::entity target{entt::null};
    cbit::Vector3 positionOffset{0.0f, 0.0f, 0.0f};
    cbit::Vector3 rotationOffset{0.0f, 0.0f, 0.0f};
    cbit::Vector3 scale{0.2f, 0.2f, 0.2f};
    bool followRotation = true;
    bool followScale = false;
    bool offsetInLocalSpace = false;
};

/**
 * @struct DebugTransformTextComponent
 * @brief  Updates a UITextComponent with transform values from a target entity.
 */
struct DebugTransformTextComponent {
    entt::entity target{entt::null};
};

class DebugTransformSystem final : public cbit::ISystem {
public:
    void update(entt::registry &registry, float deltaTime) override;
    void render(entt::registry &registry) override {;
        // No rendering needed for this system
    }
};

#endif //FANTASYTACTICS_DEBUGTRANSFORMSYSTEM_H
