/**
 * @file    MeshRenderSystem.h
 * @brief   MeshRenderSystem class for rendering mesh components.
 * @details MeshRenderSystem class is responsible for rendering mesh components in the entity-component system.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-25
 */

#ifndef CBIT_MESHRENDERSYSTEM_H
#define CBIT_MESHRENDERSYSTEM_H

#include <memory>
#include <string>
#include <unordered_map>
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
        ShaderProgram *_getShader(const std::string &vertexPath, const std::string &fragmentPath);

        GLFWwindow *_window{nullptr};
        entt::registry &_registry;
        ShaderProgram _shader;
        std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> _shaderCache;
    };
}
#endif //CBIT_MESHRENDERSYSTEM_H
