/**
 * @file    UISystem.h
 * @brief   UISystem class for managing user interface rendering in the entity-component system.
 * @details The UISystem class is responsible for rendering user interface elements in the entity-component system.
 *          It uses the entt library for efficient entity management and component storage.
 *          The UISystem can be extended in the future to include methods for handling user input,
 *          managing UI components, and integrating with other systems such as event handling and animation.
 *          It can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
 *          The UISystem can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface
 *          management system. It can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef CBIT_UISYSTEM_H
#define CBIT_UISYSTEM_H

#include "Components.h"
#include "entt/entt.hpp"
#include "graphics/renderers/ShaderProgram.h"
#include "graphics/renderers/TextRenderer.h"
#include "graphics/renderers/Texture.h"
#include "graphics/ui/Quad2D.h"


namespace cbit {
    /**
     * @class UISystem
     * @brief Represents the user interface system for rendering UI elements in the entity-component system.
     * @details The UISystem class is responsible for rendering user interface elements in the entity-component system.
     *          It uses the entt library for efficient entity management and component storage.
     *          The UISystem can be extended in the future to include methods for handling user input,
     *          managing UI components, and integrating with other systems such as event handling and animation.
     *          It can be used to organize UI elements, handle user interactions, and manage rendering and updates for the user interface.
     *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
     *          The UISystem can be integrated with other components such as SceneManager, GameObject, and Renderer to create a complete user interface
     *          management system. It can be used to create menus, HUDs, and other UI elements that enhance the user experience in the game or application.
     * @note This class is part of the entity-component system (ECS) architecture and is designed to work with the entt library for
     *       efficient entity management and component storage.
     */
    class UISystem {
    public:
        /**
         * @brief Constructs a UISystem instance with the given registry.
         * @param window The GLFW window to be used for rendering the user interface.
         * @param registry The entt registry to be used for managing entities and components.
         */
        explicit UISystem(GLFWwindow *window, entt::registry &registry);

        /**
         * Set window for the UISystem.
         * @details This method sets the GLFW window that will be used by the UISystem for rendering and handling user input.
         *          It is typically called during the initialization phase of the application.
         *          The UISystem will use this window to render UI elements and handle mouse events.
         * @note This method should be called before any rendering or input handling operations are performed.
         * @param window
         */
        void setWindow(GLFWwindow *window);

        /**
         * @brief Updates the UISystem.
         * @details This method is called to update the UISystem, allowing it to process any necessary logic or state changes.
         *          Currently, it does not perform any operations but can be extended in the future.
         * @param deltaTime The time elapsed since the last update, used for time-based calculations.
         */
        void update(float deltaTime);

        /**
         * @brief Renders the user interface elements.
         * @details This method iterates through all entities with TransformComponent and TextureComponent,
         *          and draws the corresponding UI elements using the specified shaders.
         */
        void render();

    private:
        struct UIRectangle {
            float x, y, width, height; // framebuffer space; origin bottom-left
        };

        GLFWwindow *_window{nullptr};
        entt::registry &_registry;
        ShaderProgram _uiShader;
        ShaderProgram _uiColorShader;
        TextRenderer _textRenderer;
        Quad2D _quad2D;
        std::unordered_map<std::string, Texture> _textures;

        // sizes a window to keep in sync
        int _windowWidth = 1, _windowHeight = 1;
        int _framebufferWidth = 1, _framebufferHeight = 1;

        // Input state
        bool _mouseDownLastFrame = false; // Track if the mouse was down in the last frame
        double _mouseX = 0.0; // Mouse X position
        double _mouseY = 0.0; // Mouse Y position

        /**
         * @brief Synchronizes the size of the UISystem with the current window size.
         * @details This method checks the current size of the GLFW window and updates the internal
         *          framebuffer size accordingly. It ensures that the UISystem's rendering area matches
         *          the window size, which is important for proper rendering of UI elements.
         *          If the window size has changed, it also updates the TextRenderer's size to match
         *          the new framebuffer dimensions.
         * @note This method should be called whenever the window size may have changed, such as
         *       during the update phase of the application.
         */
        void _syncSize();

        /**
         * @brief Generates an orthographic projection matrix for UI rendering.
         * @details This method creates an orthographic projection matrix based on the current
         *          framebuffer dimensions. The matrix is used to transform UI elements from
         *          screen space to normalized device coordinates for rendering.
         * @return A glm::mat4 representing the orthographic projection matrix.
         */
        glm::mat4 _orthoMatrix() const;

        /**
         * @brief Computes the UIRectangle for the given entity based on its TransformComponent and RectangleComponent.
         * @param entity The entity for which to compute the UIRectangle.
         * @return A UIRectangle representing the position and size of the UI element in framebuffer space.
         */
        UIRectangle _computeRectangle(entt::entity entity) const;

        /**
         * @brief Checks if a point (uiX, uiY) hits the given UIRectangle.
         * @param rectangle The UIRectangle to test against.
         * @param uiX The X coordinate of the point to test.
         * @param uiY The Y coordinate of the point to test.
         * @return True if the point hits the rectangle, false otherwise.
         */
        bool _hit(const UIRectangle &rectangle, double uiX, double uiY) const;

        /**
         * @brief Dispatches pointer events to UI elements based on mouse input.
         * @details This method checks the current mouse position and button states,
         *          and dispatches events such as hover enter, hover exit, and click
         *          to the appropriate UI elements based on hit testing.
         */
        void _dispatchPointerEvents();

        /** @brief Renders colored rectangles for UI elements.
         * @param projectionMatrix The orthographic projection matrix for UI rendering.
         */
        void _renderColorRectangles(const glm::mat4 &projectionMatrix);

        /** @brief Renders images for UI elements.
         * @param projectionMatrix The orthographic projection matrix for UI rendering.
         */
        void _renderImages(const glm::mat4 &projectionMatrix);

        /** @brief Renders text for UI elements.
         * @details This method iterates through all entities with TextComponent and TransformComponent,
         */
        void _renderText();

        /**
         * @brief Checks if a point (uiX, uiY) hits the rectangle defined by the TransformComponent and RectangleComponent.
         * @param transformComponent The TransformComponent containing position, rotation, and scale.
         * @param rectangleComponent The RectangleComponent defining the rectangle's size.
         * @param uiX The X coordinate of the point to test.
         * @param uiY The Y coordinate of the point to test.
         * @return True if the point hits the rectangle, false otherwise.
         */
        static bool _hitTest(const TransformComponent &transformComponent, const RectangleComponent &rectangleComponent,
                             double uiX, double uiY);

        /**
         * Draws a quad with the specified texture and transform.
         * @param texturePath The path to the texture to be used for the quad.
         * @param transform The transform component containing position, rotation, and scale.
         */
        void _drawQuad(std::string &texturePath, const TransformComponent &transform);

        /**
         * Draws a colored quad with the specified transform and color.
         * @param x The x-coordinate of the bottom-left corner of the quad.
         * @param y The y-coordinate of the bottom-left corner of the quad.
         * @param w The width of the quad.
         * @param h The height of the quad.
         * @param color The color to be used for the quad.
         */
        void _drawColorQuad(float x, float y, float w, float h, const glm::vec4 &color);

        /**
         * Draws an image quad with the specified texture, position, size, and tint color.
         * @param texturePath The path to the texture to be used for the image quad.
         * @param x The x-coordinate of the bottom-left corner of the image quad.
         * @param y The y-coordinate of the bottom-left corner of the image quad.
         * @param w The width of the image quad.
         * @param h The height of the image quad.
         * @param tint The tint color to be applied to the image quad.
         */
        void _drawImageQuad(const std::string &texturePath, float x, float y, float w, float h, const glm::vec4 &tint);
    };
}

#endif //CBIT_UISYSTEM_H
