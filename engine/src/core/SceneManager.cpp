/**
 * @file    SceneManager.cpp
 * @brief   SceneManager class for managing game scenes.
 * @details SceneManager class that manages multiple scenes in the application.
 *          It allows adding scenes, updating them, and rendering them in the correct order.
 *          The class is designed to be used in a game engine or graphics application that uses Open
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "SceneManager.h"

void SceneManager::addScene(const std::shared_ptr<Scene> &scene) {
    _scenes.push_back(scene);
}

void SceneManager::update(const float deltaTime) const {
    if (_scenes.empty()) {
        return; // No scenes to update
    }
    for (const auto &scene: _scenes) {
        scene->update(deltaTime);
    }
}

void SceneManager::render() const {
    if (_scenes.empty()) {
        return; // No scenes to update
    }
    for (const auto &scene: _scenes) {
        scene->render();
    }
}
