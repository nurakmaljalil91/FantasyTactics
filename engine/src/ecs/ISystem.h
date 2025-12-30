/**
 * @file    ISystem.h
 * @brief   Header file for the ISystem interface.
 * @details This file contains the definition of the ISystem interface which serves as a base for all systems in the entity-component system.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-3o
 */

#ifndef CBIT_ISYSTEM_H
#define CBIT_ISYSTEM_H
#include "entt/entt.hpp"

namespace cbit {
    /**
     * @class ISystem
     * @brief Interface for systems in the entity-component system.
     * @details The ISystem interface serves as a base for all systems in the entity-component system.
     *          It provides a common interface for updating and rendering systems.
     */
    class ISystem {
    public:
        /**
         * Default destructor.
         * @details Cleans up resources associated with the system.
         */
        virtual ~ISystem() = default;

        /**
         * Update the system.
         * @param registry
         * @param deltaTime Time since the last update in seconds.
         * @details This method is called to update the system logic and perform any necessary updates.
         */
        virtual void update(entt::registry &registry, float deltaTime) = 0;

        /**
         * Render the system.
         * @param registry
         * @details This method is called to render the system, drawing all relevant components.
         */
        virtual void render(entt::registry &registry) {
        };
    };
}

#endif //CBIT_ISYSTEM_H
