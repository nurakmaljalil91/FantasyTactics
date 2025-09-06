/**
 * @file     UISystem.cpp
 * @brief    UISystem class implementation file
 * @details  This file contains the implementation of the UISystem class, which is responsible for rendering user interface elements in the entity-component system.
 *           The UISystem uses the entt library for efficient entity management and component storage.
 *           It can be extended in the future to include methods for handling user input, managing UI components,
 *           and integrating with other systems such as event handling and animation.
 *           The UISystem can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
 *           The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *           It can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface management system.
 *           The UISystem can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#include "UISystem.h"
#include "Components.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "utilities/Logger.h"

UISystem::UISystem(GLFWwindow *window, entt::registry &registry) : _window(window), _registry(registry),
                                                                   _textRenderer(1200, 800) {
    // Constructor implementation can be added here if needed
    _uiShader.loadShaders("resources/shaders/ui.vert", "resources/shaders/ui.frag");
    _uiColorShader.loadShaders("resources/shaders/color_ui.vert", "resources/shaders/color_ui.frag");
    _textRenderer.loadFont("resources/fonts/Amble.ttf", 50);

    if (_window) {
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(_window, &framebufferWidth, &framebufferHeight);
        if (framebufferWidth <= 0) framebufferWidth = 1;
        if (framebufferHeight <= 0) framebufferHeight = 1;
        _textRenderer.resize(framebufferWidth, framebufferHeight);
    }
}

void UISystem::setWindow(GLFWwindow *window) {
    _window = window;
    if (_window) {
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(_window, &framebufferWidth, &framebufferHeight);
        if (framebufferWidth <= 0) framebufferWidth = 1;
        if (framebufferHeight <= 0) framebufferHeight = 1;
        _textRenderer.resize(framebufferWidth, framebufferHeight);
    }
}

void UISystem::update(float deltaTime) {
    _syncSize();

    // Scale from window space to framebuffer space (handle HiDPI/retina)
    const float scaleX = static_cast<float>(_framebufferWidth) / static_cast<float>(_windowWidth);
    const float scaleY = static_cast<float>(_framebufferHeight) / static_cast<float>(_windowHeight);

    // Cursor in window coordinates (top-left is (0,0))
    double mouseX, mouseY;
    glfwGetCursorPos(_window, &mouseX, &mouseY);
    _mouseX = mouseX;
    _mouseY = mouseY;

    // Convert to framebuffer coordinates (top-left is (0,0))
    _mouseX = mouseX * scaleX;
    _mouseY = (_windowHeight - _mouseY) * scaleY;

    const bool mouseDown = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    const bool mousePressedThisFrame = mouseDown && !_mouseDownLastFrame;
    const bool mouseReleasedThisFrame = !mouseDown && _mouseDownLastFrame;

    const auto view = _registry.view<TransformComponent, RectangleComponent, ButtonComponent>();
    for (const auto entity: view) {
        auto &transform = view.get<TransformComponent>(entity);
        auto &rectangle = view.get<RectangleComponent>(entity);
        auto &button = view.get<ButtonComponent>(entity);

        const bool wasHovered = button.isHovered;
        const bool nowHovered = _hitTest(transform, rectangle, _mouseX, _mouseY);

        // Hover transition
        button.isHovered = nowHovered;
        if (!wasHovered && nowHovered && button.onHoverEnter) {
            button.onHoverEnter(entity); // Call hover enter callback
        } else if (wasHovered && !nowHovered && button.onHoverExit) {
            button.onHoverExit(entity); // Call hover exit callback
        }

        // Press/Release state
        // Arm on press that begins inside.
        if (mousePressedThisFrame) {
            button.isPressedInside = nowHovered;
        }

        // Visual "down" while holding: only if press started inside and still hovered
        button.isPressed = mouseDown && button.isPressedInside && nowHovered;

        // Fire on release if the press started inside (release anywhere)
        if (mouseReleasedThisFrame) {
            if (button.isPressedInside) {
                if (button.onClick) button.onClick(entity);
            }
            button.isPressed = false;
            button.isPressedInside = false;
        }
    }

    _mouseDownLastFrame = mouseDown;
}

void UISystem::render() {
    _syncSize();

    const glm::mat4 orthoProjection = glm::ortho(
        0.0f, static_cast<float>(_framebufferWidth), // left → right
        0.0f, static_cast<float>(_framebufferHeight), // bottom → top
        -1.0f, 1.0f
    );

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Textured Quad
    _uiShader.use();
    _uiShader.setUniform("uProjection", orthoProjection);

    for (const auto entities: _registry.view<TransformComponent, TextureComponent>()) {
        auto &transform = _registry.get<TransformComponent>(entities);
        auto &[path] = _registry.get<TextureComponent>(entities);

        // _drawColorQuad(transform, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Draw a white quad for now
        _drawQuad(path, transform);
    }

    _uiColorShader.use();
    _uiColorShader.setUniform("uProjection", orthoProjection);

    for (const auto entities: _registry.view<TransformComponent, RectangleComponent, ButtonComponent>()) {
        const auto &transform = _registry.get<TransformComponent>(entities);
        auto &[rectangleWidth, rectangleHeight] = _registry.get<RectangleComponent>(entities);
        const auto &button = _registry.get<ButtonComponent>(entities);

        TransformComponent sized = transform;
        if (rectangleWidth > 0) {
            sized.scale.x = static_cast<float>(rectangleWidth);
        } else {
            sized.scale.x = transform.scale.x; // Use original scale if width is not set
        }
        if (rectangleHeight > 0) {
            sized.scale.y = static_cast<float>(rectangleHeight);
        } else {
            sized.scale.y = transform.scale.y; // Use original scale if height is not set
        }

        // check if it hovers by mouse
        glm::vec4 color = button.color;
        if (button.isHovered) color = glm::vec4(0.9f, 0.9f, 1.0f, 1.0f);
        if (button.isPressed) color = glm::vec4(0.7f, 0.7f, 1.0f, 1.0f);

        _drawColorQuad(sized, color);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void UISystem::_syncSize() {
    if (!_window) return;
    glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
    glfwGetFramebufferSize(_window, &_framebufferWidth, &_framebufferHeight);
    _windowWidth = std::max(_windowWidth, 1);
    _windowHeight = std::max(_windowHeight, 1);
    _framebufferWidth = std::max(_framebufferWidth, 1);
    _framebufferHeight = std::max(_framebufferHeight, 1);

    glViewport(0, 0, _framebufferWidth, _framebufferHeight);
    _textRenderer.resize(_framebufferWidth, _framebufferHeight);
}

bool UISystem::_hitTest(const TransformComponent &transformComponent, const RectangleComponent &rectangleComponent,
                        const double uiX, const double uiY) {
    const double x0 = transformComponent.position.x;
    const double y0 = transformComponent.position.y;
    const double x1 = x0 + (rectangleComponent.width > 0
                                ? static_cast<float>(rectangleComponent.width)
                                : transformComponent.scale.x);
    const double y1 = y0 + (rectangleComponent.height > 0
                                ? static_cast<float>(rectangleComponent.height)
                                : transformComponent.scale.y);
    return uiX >= x0 && uiX <= x1 && uiY >= y0 && uiY <= y1;
}

void UISystem::_drawQuad(std::string &texturePath, const TransformComponent &transform) {
    auto [it, inserted] = _textures.try_emplace(texturePath); // default‐constructs Texture
    if (inserted) {
        if (!it->second.loadTexture(texturePath)) {
            Logger::log()->error("Failed to load texture: {}", texturePath);
            _textures.erase(it);
            return;
        }
    }
    // now it->second is valid
    it->second.bind();

    _uiShader.use();

    glActiveTexture(GL_TEXTURE0);
    _uiShader.setUniform("uTexture", 0);

    const glm::vec2 position{
        transform.position.x,
        transform.position.y
    };

    const glm::vec2 size{
        transform.scale.x,
        transform.scale.y
    };

    _uiShader.setUniform("uPosition", position);

    _uiShader.setUniform("uSize", size);

    _quad2D.draw();
}

void UISystem::_drawColorQuad(const TransformComponent &transform, const glm::vec4 &color) {
    _uiColorShader.use();
    // _uiColorShader.setUniform("uProjection", glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f));

    const glm::vec2 position(transform.position.x, transform.position.y);
    const glm::vec2 size(transform.scale.x, transform.scale.y);
    _uiColorShader.setUniform("uPosition", position);
    _uiColorShader.setUniform("uSize", size);
    _uiColorShader.setUniform("uColor", color);

    // draw the unit quad scaled/translated
    _quad2D.draw();
}
