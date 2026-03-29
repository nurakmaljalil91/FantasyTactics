/**
 * @file    TurnControlSystem.h
 * @brief   Header file for the TurnControlSystem class.
 * @details Selects which unit is currently allowed to move.
 * @author  Nur Akmal bin Jalil
 * @date    2026-03-29
 */

#ifndef FANTASYTACTICS_TURNCONTROLSYSTEM_H
#define FANTASYTACTICS_TURNCONTROLSYSTEM_H

#include <string>

#include "ecs/ISystem.h"

/**
 * @struct TurnControlComponent
 * @brief  Stores turn-order data and whether the unit can currently move.
 */
struct TurnControlComponent {
    int slot = 0;
    std::string displayName;
    bool canMove = false;
};

/**
 * @struct TurnStatusTextComponent
 * @brief  Marks a UI text entity that should display the active unit name.
 */
struct TurnStatusTextComponent {
    std::string label = "Current Turn: ";
};

class TurnControlSystem final : public cbit::ISystem {
public:
    void update(entt::registry &registry, float deltaTime) override;

    void render(entt::registry &registry) override {
        // No rendering needed for this system.
    }
};

#endif // FANTASYTACTICS_TURNCONTROLSYSTEM_H
