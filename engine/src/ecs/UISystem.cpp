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

cbit::UISystem::UISystem(GLFWwindow *window, entt::registry &registry) : _window(window), _registry(registry),
                                                                         _textRenderer(1200, 800) {
    // Constructor implementation can be added here if needed
    _uiShader.loadShaders("resources/shaders/ui.vert", "resources/shaders/ui.frag");
    _uiColorShader.loadShaders("resources/shaders/color_ui.vert", "resources/shaders/color_ui.frag");
    _textRenderer.loadFont("resources/fonts/Amble.ttf", 50);

    _syncSize();
}

void cbit::UISystem::setWindow(GLFWwindow *window) {
    _window = window;
    _syncSize();
}

void cbit::UISystem::update(float deltaTime) {
    (void) deltaTime; // Currently unused
    _syncSize();

    if (!_window) return;
    _dispatchPointerEvents();
}

void cbit::UISystem::render() {
    _syncSize();

    const glm::mat4 projectionMatrix = _orthoMatrix();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _renderImages(projectionMatrix);
    _renderColorRectangles(projectionMatrix);
    _renderText();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void cbit::UISystem::_syncSize() {
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

glm::mat4 cbit::UISystem::_orthoMatrix() const {
    return glm::ortho(
        0.0f, static_cast<float>(_framebufferWidth), // left → right
        0.0f, static_cast<float>(_framebufferHeight), // bottom → top
        -1.0f, 1.0f
    );
}

cbit::UISystem::UIRectangle cbit::UISystem::_computeRectangle(entt::entity entity) const {
    UIRectangle rectangle{};

    // Size resolution priority: UIAnchor.sizePixel -> RectangleComponent -> TransformComponent.scale
    glm::vec2 size{0.0f, 0.0f};

    if (const auto *anchorComponent = _registry.try_get<UIAnchorComponent>(entity)) {
        if (anchorComponent->sizePixel.toGLM().x > 0.0f) {
            size.x = anchorComponent->sizePixel.toGLM().x;
        }
        if (anchorComponent->sizePixel.toGLM().y > 0.0f) {
            size.y = anchorComponent->sizePixel.toGLM().y;
        }
    }

    if ((size.x <= 0.0f || size.y <= 0.0f) && _registry.all_of<RectangleComponent>(entity)) {
        const auto &rectangleComponent = _registry.get<RectangleComponent>(entity);
        if (size.x <= 0.0f && rectangleComponent.width > 0) {
            size.x = static_cast<float>(rectangleComponent.width);
        }
        if (size.y <= 0.0f && rectangleComponent.height > 0) {
            size.y = static_cast<float>(rectangleComponent.height);
        }
    }

    if ((size.x <= 0.0f || size.y <= 0.0f) && _registry.all_of<TransformComponent>(entity)) {
        const auto &transformComponent = _registry.get<TransformComponent>(entity);
        if (size.x <= 0.0f) {
            size.x = transformComponent.scale.toGLM().x;
        }
        if (size.y <= 0.0f) {
            size.y = transformComponent.scale.toGLM().y;
        }
    }

    // Fallback for something non-zero
    size.x = std::max(size.x, 1.0f);
    size.y = std::max(size.y, 1.0f);

    glm::vec2 position{0.0f, 0.0f};

    if (auto *anchorComponent = _registry.try_get<UIAnchorComponent>(entity)) {
        // Determine anchor base position
        switch (anchorComponent->anchor) {
            case UIAnchor::TopLeft:
                position.x = 0.0f;
                position.y = static_cast<float>(_framebufferHeight) - size.y;
                break;
            case UIAnchor::TopCenter:
                position.x = (static_cast<float>(_framebufferWidth) - size.x) / 2.0f;
                position.y = static_cast<float>(_framebufferHeight) - size.y;
                break;
            case UIAnchor::TopRight:
                position.x = static_cast<float>(_framebufferWidth) - size.x;
                position.y = static_cast<float>(_framebufferHeight) - size.y;
                break;
            case UIAnchor::CenterLeft:
                position.x = 0.0f;
                position.y = (static_cast<float>(_framebufferHeight) - size.y) / 2.0f;
                break;
            case UIAnchor::Center:
                position.x = (static_cast<float>(_framebufferWidth) - size.x) / 2.0f;
                position.y = (static_cast<float>(_framebufferHeight) - size.y) / 2.0f;
                break;
            case UIAnchor::CenterRight:
                position.x = static_cast<float>(_framebufferWidth) - size.x;
                position.y = (static_cast<float>(_framebufferHeight) - size.y) / 2.0f;
                break;
            case UIAnchor::BottomLeft:
                position.x = 0.0f;
                position.y = 0.0f;
                break;
            case UIAnchor::BottomCenter:
                position.x = (static_cast<float>(_framebufferWidth) - size.x) / 2.0f;
                position.y = 0.0f;
                break;
            case UIAnchor::BottomRight:
                position.x = static_cast<float>(_framebufferWidth) - size.x;
                position.y = 0.0f;
                break;
        }

        // Apply offset.
        //
        // NOTE: offsetPixel is interpreted in the same window/framebuffer coordinate
        // system used to compute 'position'. In this system, the origin is at the
        // bottom-left of the framebuffer and the Y axis increases upwards. This
        // means that a positive offsetPixel.y will always move the UI element
        // visually upwards from its anchor, regardless of which anchor is used.
        position += anchorComponent->offsetPixel.toGLM();
    } else if (_registry.all_of<TransformComponent>(entity)) {
        const auto &transformComponent = _registry.get<TransformComponent>(entity);
        position.x = transformComponent.position.toGLM().x;
        position.y = transformComponent.position.toGLM().y;
    }

    rectangle.x = position.x;
    rectangle.y = position.y;
    rectangle.width = size.x;
    rectangle.height = size.y;
    return rectangle;
}

bool cbit::UISystem::_hit(const UIRectangle &rectangle, double uiX, double uiY) const {
    return uiX >= rectangle.x && uiX <= (rectangle.x + rectangle.width) &&
           uiY >= rectangle.y && uiY <= (rectangle.y + rectangle.height);
}

void cbit::UISystem::_dispatchPointerEvents() {
    // Mouse conversion: window coordinates (top-left origin) -> framebuffer coordinates (bottom-left origin)
    const float scaleX = static_cast<float>(_framebufferWidth) / static_cast<float>(_windowWidth);
    const float scaleY = static_cast<float>(_framebufferHeight) / static_cast<float>(_windowHeight);

    double mouseX, mouseY;

    glfwGetCursorPos(_window, &mouseX, &mouseY);

    _mouseX = mouseX * scaleX;
    _mouseY = (_windowHeight - mouseY) * scaleY;

    const bool mouseDown = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    const bool mousePressedThisFrame = mouseDown && !_mouseDownLastFrame;
    const bool mouseReleasedThisFrame = !mouseDown && _mouseDownLastFrame;

    // Collect interactable UI entities with z-index, sort descending so top-most gets events first
    struct Item {
        entt::entity entity;
        int z;
    };

    std::vector<Item> interactableEntities;

    auto view = _registry.view<UIAnchorComponent>();
    interactableEntities.reserve(view.size());

    for (const auto &entity: view) {
        const auto &anchorComponent = view.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible || !anchorComponent.interactable) {
            continue;
        }
        if (_registry.any_of<UIPointerCallbacksComponent, ButtonComponent>(entity)) {
            interactableEntities.push_back({entity, anchorComponent.zIndex});
        }
    }

    std::sort(interactableEntities.begin(), interactableEntities.end(),
              [](const Item &lhs, const Item &rhs) { return lhs.z > rhs.z; });

    bool consumed = false;
    for (const auto &item: interactableEntities) {
        const auto entity = item.entity;

        auto &anchorComponent = _registry.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible || !anchorComponent.interactable) { continue; }

        const UIRectangle rectangle = _computeRectangle(entity);
        const bool nowHovered = _hit(rectangle, _mouseX, _mouseY);

        // Ensure pointer state exists
        if (!_registry.any_of<UIPointerState>(entity)) {
            _registry.emplace<UIPointerState>(entity);
        }

        auto &statePointer = _registry.get<UIPointerState>(entity);

        // Get callbacks (ButtonComponent reuses its onClick; UIPointerCallbacks is more general)
        UIPointerCallbacksComponent *callbacks = _registry.try_get<UIPointerCallbacksComponent>(entity);

        // Back-compatibility: if entity has ButtonComponent, use its callbacks
        ButtonComponent *button = _registry.try_get<ButtonComponent>(entity);

        UIPointerCallbacksComponent mapped{};

        if (!callbacks && button) {
            mapped.onHoverEnter = button->onHoverEnter;
            mapped.onHoverExit = button->onHoverExit;
            mapped.onClick = button->onClick;
            callbacks = &mapped;
        }

        const bool wasHovered = statePointer.hovered;
        // Hover transition
        statePointer.hovered = (!consumed) && nowHovered;

        if (!wasHovered && statePointer.hovered && callbacks && callbacks->onHoverEnter) {
            callbacks->onHoverEnter(entity); // Call hover enter callback
        } else if (wasHovered && !statePointer.hovered && callbacks && callbacks->onHoverExit) {
            callbacks->onHoverExit(entity); // Call hover exit callback
        }

        // If already consumed by a higher z-index element, skip further processing
        if (!consumed && mousePressedThisFrame) {
            statePointer.pressedInside = statePointer.hovered;
            statePointer.pressed = statePointer.pressedInside;
            if (statePointer.pressedInside && callbacks && callbacks->onPress) {
                callbacks->onPress(entity);
            }
        }

        // Keep pressed visual if press began inside; do not require hover to maintain pressed state
        if (mouseDown) {
            statePointer.pressed = statePointer.pressedInside;
        }

        // Consume only for hover/press targeting (simple rule: first hovered element consumes)
        if (!consumed && nowHovered) {
            consumed = true;
        }

        if (mouseReleasedThisFrame) {
            const bool clicked = statePointer.pressedInside && statePointer.hovered;
            statePointer.pressed = false;
            statePointer.pressedInside = false;

            if (clicked && callbacks && callbacks->onClick) {
                callbacks->onClick(entity);
            } else if (callbacks && callbacks->onRelease) {
                callbacks->onRelease(entity);
            }
        }

        // Keep old ButtonComponent flags in sync
        if (button) {
            button->isHovered = statePointer.hovered;
            button->isPressed = statePointer.pressed;
            button->isPressedInside = statePointer.pressedInside;
        }
    }

    // Update mouse button state for next frame
    _mouseDownLastFrame = mouseDown;
}

void cbit::UISystem::_renderColorRectangles(const glm::mat4 &projectionMatrix) {
    _uiColorShader.use();
    _uiColorShader.setUniform("uProjection", projectionMatrix);

    // Draw sorted by z-index ascending (back to front)
    struct Item {
        entt::entity entity;
        int z;
    };

    std::vector<Item> colorRectangleEntities;
    auto view = _registry.view<UIAnchorComponent, UIColorRectangleComponent>();
    colorRectangleEntities.reserve(view.size_hint());

    for (const auto &entity: view) {
        const auto &anchorComponent = view.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible) {
            continue;
        }
        colorRectangleEntities.push_back({entity, anchorComponent.zIndex});
    }

    std::sort(colorRectangleEntities.begin(), colorRectangleEntities.end(),
              [](const Item &lhs, const Item &rhs) { return lhs.z < rhs.z; });

    for (const auto &item: colorRectangleEntities) {
        const auto entity = item.entity;
        const auto &anchorComponent = _registry.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible) { continue; }

        const auto &visualComponent = _registry.get<UIColorRectangleComponent>(entity);
        const UIRectangle rectangle = _computeRectangle(entity);

        glm::vec4 color = visualComponent.color;

        if (auto *statePointer = _registry.try_get<UIPointerState>(entity)) {
            if (statePointer->hovered) {
                color = visualComponent.hoverColor;
            }
            if (statePointer->pressed) {
                color = visualComponent.activeColor;
            }
        } else if (auto *button = _registry.try_get<ButtonComponent>(entity)) {
            if (button->isHovered) {
                color = visualComponent.hoverColor;
            }
            if (button->isPressed) {
                color = visualComponent.activeColor;
            }
        }

        _drawColorQuad(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color);
    }
}

void cbit::UISystem::_renderImages(const glm::mat4 &projectionMatrix) {
    _uiShader.use();
    _uiShader.setUniform("uProjection", projectionMatrix);

    struct Item {
        entt::entity entity;
        int z;
    };
    std::vector<Item> imageEntities;
    auto view = _registry.view<UIAnchorComponent, UIImageComponent>();
    imageEntities.reserve(view.size_hint());
    for (const auto &entity: view) {
        const auto &anchorComponent = view.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible) {
            continue;
        }
        imageEntities.push_back({entity, anchorComponent.zIndex});
    }
    std::sort(imageEntities.begin(), imageEntities.end(),
              [](const Item &lhs, const Item &rhs) { return lhs.z < rhs.z; });

    for (const auto &item: imageEntities) {
        const auto entity = item.entity;
        const auto &anchorComponent = _registry.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible) { continue; }
        const auto &imageComponent = _registry.get<UIImageComponent>(entity);
        const UIRectangle rectangle = _computeRectangle(entity);
        _drawImageQuad(imageComponent.path, rectangle.x, rectangle.y, rectangle.width, rectangle.height,
                       imageComponent.tintColor);
    }
}

void cbit::UISystem::_renderText() {
    // TextRenderer likely draw in screen space; assume bottom-left origin
    // If it uses top-left origin, need to convert y coordinate
    struct Item {
        entt::entity entity;
        int z;
    };
    std::vector<Item> textEntities;
    auto view = _registry.view<UIAnchorComponent, UITextComponent>();
    textEntities.reserve(view.size_hint());
    for (const auto &entity: view) {
        const auto &anchorComponent = view.get<UIAnchorComponent>(entity);
        if (!anchorComponent.visible) {
            continue;
        }
        textEntities.push_back({entity, anchorComponent.zIndex});
    }
    std::sort(textEntities.begin(), textEntities.end(),
              [](const Item &lhs, const Item &rhs) { return lhs.z < rhs.z; });

    for (const auto &item: textEntities) {
        const auto entity = item.entity;
        const auto &anchorComponent = _registry.get<UIAnchorComponent>(entity);

        const auto &textComponent = _registry.get<UITextComponent>(entity);
        const UIRectangle rectangle = _computeRectangle(entity);

        // Approximate centering without font metrics:
        // - Estimate text width using a fixed per-character advance
        // - Place baseline slightly below the vertical midpoint
        const float scale = textComponent.fontSize;
        const float approxAdvancePx = 30.0f * scale; // tune this constant for your font size 50
        const float approxTextWidth = approxAdvancePx * static_cast<float>(textComponent.text.size());

        const float x = rectangle.x + (rectangle.width - approxTextWidth) * 0.5f;
        const float y = rectangle.y + (rectangle.height * 0.5f) - (14.0f * scale);

        const glm::vec3 rgb(textComponent.color.r, textComponent.color.g, textComponent.color.b);
        _textRenderer.renderText(
            textComponent.text,
            x,
            y,
            textComponent.fontSize,
            rgb
        );
    }
}

bool cbit::UISystem::_hitTest(const TransformComponent &transformComponent,
                              const RectangleComponent &rectangleComponent,
                              const double uiX, const double uiY) {
    const double x0 = transformComponent.position.toGLM().x;
    const double y0 = transformComponent.position.toGLM().y;
    const double x1 = x0 + (rectangleComponent.width > 0
                                ? static_cast<float>(rectangleComponent.width)
                                : transformComponent.scale.toGLM().x);
    const double y1 = y0 + (rectangleComponent.height > 0
                                ? static_cast<float>(rectangleComponent.height)
                                : transformComponent.scale.toGLM().y);
    return uiX >= x0 && uiX <= x1 && uiY >= y0 && uiY <= y1;
}

void cbit::UISystem::_drawQuad(std::string &texturePath, const TransformComponent &transform) {
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
        transform.position.toGLM().x,
        transform.position.toGLM().y
    };

    const glm::vec2 size{
        transform.scale.toGLM().x,
        transform.scale.toGLM().y
    };

    _uiShader.setUniform("uPosition", position);

    _uiShader.setUniform("uSize", size);

    _quad2D.draw();
}

void cbit::UISystem::_drawColorQuad(float x, float y, float w, float h, const glm::vec4 &color) {
    _uiColorShader.use();
    // _uiColorShader.setUniform("uProjection", glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f));

    const glm::vec2 position(x, y);
    const glm::vec2 size(w, h);
    _uiColorShader.setUniform("uPosition", position);
    _uiColorShader.setUniform("uSize", size);
    _uiColorShader.setUniform("uColor", color);

    // draw the unit quad scaled/translated
    _quad2D.draw();
}

void cbit::UISystem::_drawImageQuad(const std::string &texturePath, float x, float y, float w, float h,
                                    const glm::vec4 &tint) {
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
    const glm::vec2 position(x, y);
    const glm::vec2 size(w, h);
    _uiShader.setUniform("uPosition", position);
    _uiShader.setUniform("uSize", size);
    // if ui shader supports tint
    if (_uiShader.hasUniform("uTint")) {
        _uiShader.setUniform("uTint", tint);
    }
    _quad2D.draw();
}
