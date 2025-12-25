/**
 * @file    EntityComponentSystem.cpp
 * @brief   EntityComponentSystem class implementation file
 * @details This file contains the implementation of the EntityComponentSystem class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#include "EntityComponentSystem.h"
#include "Components.h"
#include "GameObject.h"
#include "utilities/UUIDGenerator.h"

cbit::EntityComponentSystem::EntityComponentSystem() : _uiSystem(nullptr, _registry),
                                                       _meshRenderSystem(_registry) {
    // Constructor implementation can be added here if needed
}

void cbit::EntityComponentSystem::setWindow(GLFWwindow *window) {
    _window = window;
    _uiSystem.setWindow(window);
    _meshRenderSystem.setWindow(window);
}

cbit::EntityComponentSystem::~EntityComponentSystem() = default;

void cbit::EntityComponentSystem::update(const float deltaTime) {
    _uiSystem.update(deltaTime);
}

void cbit::EntityComponentSystem::render() {
    _meshRenderSystem.render();
    _uiSystem.render();
}

void cbit::EntityComponentSystem::cleanup() {
    _registry.clear();
}

cbit::GameObject cbit::EntityComponentSystem::createGameObject(const std::string &tag) {
    auto entity = GameObject(_registry.create(), this);
    entity.addComponent<TagComponent>(tag);
    entity.addComponent<IdComponent>(UUIDGenerator::generate());
    return entity;
}

void cbit::EntityComponentSystem::destroyGameObject(const GameObject gameObject) {
    _registry.destroy(gameObject.getEntity());
}

cbit::GameObject cbit::EntityComponentSystem::getGameObject(const std::string &tag) {
    const auto view = _registry.view<TagComponent>();
    for (auto entity: view) {
        auto &tagComponent = view.get<TagComponent>(entity);
        if (tagComponent.tag == tag) {
            return {entity, this};
        }
    }
    return {entt::null, this};
}
