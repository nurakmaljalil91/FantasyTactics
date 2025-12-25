/**
 * @file    Scene.cpp
 * @brief   Scene class for managing game scenes.
 * @details The Scene class is a placeholder for managing game scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "Scene.h"

void cbit::Scene::setWindowSize(const int width, const int height) {
    _windowHeight = height;
    _windowWidth = width;
}

void cbit::Scene::update(float deltaTime) {
    _world.update(deltaTime);
}

void cbit::Scene::render() {
    _world.render();
}

void cbit::Scene::cleanup() {
    _world.cleanup();
}

void cbit::Scene::setWindow(GLFWwindow *window) {
    _window = window;
    _world.setWindow(window);
}

GLFWwindow *cbit::Scene::getWindow() const {
    return _window;
}

int cbit::Scene::getWindowWidth() const {
    return _windowWidth;
}

int cbit::Scene::getWindowHeight() const {
    return _windowHeight;
}

EntityComponentSystem &cbit::Scene::getWorld() {
    return _world;
}
