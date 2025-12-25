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

cbit::SceneManager::SceneManager(): _currentScene(nullptr) {
}

cbit::SceneManager::~SceneManager() {
    cleanup();
}

void cbit::SceneManager::initialize(GLFWwindow *window) {
    _window = window;
}

void cbit::SceneManager::addScene(const std::string &name, std::shared_ptr<Scene> scene) {
    if (_scenes.find(name) != _scenes.end()) {
        Logger::log()->error("Scene '{}' already exists in SceneManager", name);
        return; // Scene with this name already exists
    }
    _scenes[name] = std::move(scene);
}

std::string cbit::SceneManager::getActiveScene() {
    for (const auto &[name, scene]: _scenes) {
        if (scene == _currentScene) {
            return name; // Return the name of the active scene
        }
    }
    return "";
}

void cbit::SceneManager::setActiveScene(const std::string &name) {
    const auto scenes = _scenes.find(name);
    if (scenes == _scenes.end()) {
        Logger::log()->error("Scene '{}' does not exist in SceneManager", name);
        _currentScene = nullptr;
        return; // Scene with this name does not exist
    }

    if (_currentScene) {
        _currentScene->cleanup(); // Clean up the current scene before switching
    }

    _currentScene = scenes->second;

    if (_currentScene) {
        _currentScene->setSceneManager(this);
        _currentScene->setWindow(_window);
        _currentScene->initialize(); // Initialize the new current scene
    }
}

void cbit::SceneManager::update(const float deltaTime) const {
    if (_currentScene) {
        _currentScene->update(deltaTime);
    }
}

void cbit::SceneManager::render(int windowWidth, int windowHeight) const {
    if (_currentScene) {
        _currentScene->setWindowSize(windowWidth, windowHeight);
        _currentScene->render();
    }
}

void cbit::SceneManager::cleanup() const {
    if (_currentScene) {
        _currentScene->cleanup();
    }
}
