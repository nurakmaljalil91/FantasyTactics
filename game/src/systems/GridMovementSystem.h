/**
 * @file    GridMovementSystem.h
 * @brief   Header file for the GridMovementSystem class
 * @details This file contains the definition of the GridMovementSystem class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef FANTASYTACTICS_GRIDMOVEMENTSYSTEM_H
#define FANTASYTACTICS_GRIDMOVEMENTSYSTEM_H
#include <utility>

#include "ecs/ISystem.h"
#include "math/Vector3.h"

/**
 * @struct GridMovementComponent
 * @brief  Grid movement component for tile-based movement.
 */
struct GridMovementComponent {
    int x = 0;
    int z = 0;
    float moveCooldown = 0.0f;
};

class GridMovementSystem final : public cbit::ISystem {
public:
    using GridToWorldFunction = std::function<cbit::Vector3(int x, int z)>;

    explicit GridMovementSystem(GridToWorldFunction gridToWorldFunction, const int gridWidth, const int gridHeight)
        : _gridToWorldFunction(std::move(gridToWorldFunction)),
          _gridWidth(gridWidth),
          _gridHeight(gridHeight) {
    }

    void update(entt::registry &registry, float deltaTime) override;

    void render(entt::registry &registry) override {;
        // No rendering needed for this system
    }

private:
    GridToWorldFunction _gridToWorldFunction;
    int _gridWidth = 0;
    int _gridHeight = 0;
};


#endif //FANTASYTACTICS_GRIDMOVEMENTSYSTEM_H
