/**
 * @file    AnimationSystem.h
 * @brief   Animation system for updating skeletal animations.
 * @details Evaluates AnimatorComponents and updates final bone matrices.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#ifndef CBIT_ANIMATIONSYSTEM_H
#define CBIT_ANIMATIONSYSTEM_H

#include "ISystem.h"

namespace cbit {
    class AnimationSystem final : public ISystem {
    public:
        void update(entt::registry &registry, float deltaTime) override;
    };
}

#endif //CBIT_ANIMATIONSYSTEM_H
