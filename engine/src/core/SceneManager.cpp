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
#include "utilities/Logger.h"

SceneManager::SceneManager(): _currentScene(nullptr) {
}

void SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    if (_scenes.find(name) != _scenes.end()) {
        Logger::log()->error("Scene '{}' already exists in SceneManager", name);
        return; // Scene with this name already exists
    }
    _scenes[name] = std::move(scene);
}

std::string SceneManager::getActiveScene() {
    for (const auto &[name, scene]: _scenes) {
        if (scene == _currentScene) {
            return name; // Return the name of the active scene
        }
    }
    return "";
}

void SceneManager::setActiveScene(const std::string &name) {
    if (_scenes.find(name) != _scenes.end()) {
        _currentScene = _scenes[name];
        _currentScene->initialize();
    } else {
        Logger::log()->error("Scene '{}' not found in SceneManager", name);
        _currentScene = nullptr;
    }
}

void SceneManager::update(const float deltaTime) const {
    if (_currentScene) {
        _currentScene->update(deltaTime);
    }
}

void SceneManager::render() const {
    if (_currentScene) {
        _currentScene->render();
    }
}
