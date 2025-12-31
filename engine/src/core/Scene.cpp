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
    glClearColor(
        _backgroundColor.glmVec4.r,
        _backgroundColor.glmVec4.g,
        _backgroundColor.glmVec4.b,
        _backgroundColor.glmVec4.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _world.render();
}

void cbit::Scene::cleanup() {
    _world.cleanup();
}

void cbit::Scene::setWindow(GLFWwindow *window) {
    _window = window;
    _world.setWindow(window);
}

void cbit::Scene::setBackgroundColor(const Color &color) {
    _backgroundColor = color;
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

cbit::EntityComponentSystem &cbit::Scene::getWorld() {
    return _world;
}
