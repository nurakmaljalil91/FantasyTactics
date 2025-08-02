/**
 * @file    Scene.cpp
 * @brief   Scene class for managing game scenes.
 * @details The Scene class is a placeholder for managing game scenes in the application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-29
 */

#include "Scene.h"

void Scene::setWindowSize(int width, int height) {
    _windowHeight = height;
    _windowWidth = width;
}

int Scene::getWindowWidth() const {
    return _windowWidth;
}

int Scene::getWindowHeight() const {
    return _windowHeight;
}
