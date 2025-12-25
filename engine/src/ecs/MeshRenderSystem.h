/**
 * @file    MeshRenderSystem.h
 * @brief   MeshRenderSystem class for rendering mesh components.
 * @details MeshRenderSystem class is responsible for rendering mesh components in the entity-component system.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef FANTASYTACTICS_MESHRENDERSYSTEM_H
#define FANTASYTACTICS_MESHRENDERSYSTEM_H
#include "entt/entt.hpp"
#include "graphics/renderers/ShaderProgram.h"

namespace cbit {
    /**
     * @class MeshRenderSystem
     * @brief System for rendering mesh components.
     */
    class MeshRenderSystem {
    public:
        /**
         * @brief Constructs a MeshRenderSystem instance with the given registry.
         * @param registry The entt registry to be used for managing entities and components.
         */
        explicit MeshRenderSystem(entt::registry &registry);

        void setWindow(GLFWwindow *window);

        /**
         * @brief Renders all mesh components.
         * @details This method iterates through all entities with mesh components and renders them.
         */
        void render();

    private:
        GLFWwindow *_window{nullptr};
        entt::registry &_registry;
        ShaderProgram _shader;
    };
}
#endif //FANTASYTACTICS_MESHRENDERSYSTEM_H
