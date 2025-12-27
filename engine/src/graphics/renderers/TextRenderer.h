/**
 * @file    TextRenderer.h
 * @brief   Header file for the TextRenderer class.
 * @details This file defines the TextRenderer class which is responsible for rendering text in OpenGL.
 *          It provides methods to load fonts, render text with specified positions and colors,
 *          and manage text rendering resources. The class uses FreeType for font loading and rendering,
 *          and it is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
 *          The TextRenderer class is intended to be used in applications that require text rendering capabilities,
 *          such as games, graphical user interfaces, and other applications that need to display text on the screen.
 *          It provides a simple interface for rendering text with various styles and sizes, and
 *          it manages the necessary resources for efficient text rendering.
 *          The class is designed to be extensible, allowing for future enhancements such as support for
 *          additional font formats, text styles, and rendering techniques.
 *          The TextRenderer class is part of a larger graphics rendering framework and is intended to be
 *          used in conjunction with other components such as ShaderProgram, Texture2D, and Mesh classes for rendering text in a scene.
 *          The class is designed to be used in a game engine or graphics application that uses OpenGL for rendering.
 *          The text can be rendered in various
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-28
 */

#ifndef CBIT_TEXTRENDERER_H
#define CBIT_TEXTRENDERER_H

#include <map>

#include "ShaderProgram.h"
#include "OpenGLInclude.h"
#include "glm/vec2.hpp"

namespace cbit {
    /**
     * @struct Character
     * @brief  Represents a single character glyph.
     */
    struct Character {
        GLuint textureID; // ID handle of glyph texture
        glm::ivec2 size; // Size of glyph
        glm::ivec2 bearing; // Offset from baseline to left/top of glyph
        GLuint advance; // Horizontal offset to advance to next glyph
    };

    /**
     * @class TextRenderer
     * @brief Renders text using OpenGL.
     * @details The TextRenderer class is responsible for rendering text in OpenGL applications.
     *          It uses FreeType for font loading and rendering, and it provides methods to render text
     *          with specified positions, sizes, and colors. The class manages the necessary OpenGL resources
     *          for efficient text rendering, including vertex buffers and texture atlases for glyphs.
     *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
     *          The TextRenderer class is intended to be used in applications that require text rendering capabilities,
     *          such as games, graphical user interfaces, and other applications that need to display text on the screen.
     *          It provides a simple interface for rendering text with various styles and sizes, and
     *          it manages the necessary resources for efficient text rendering.
     *          The class is designed to be extensible, allowing for future enhancements such as support for
     *          additional font formats, text styles, and rendering techniques.
     *          The TextRenderer class is part of a larger graphics rendering framework and is intended to be
     *          used in conjunction with other components such as ShaderProgram, Texture2D, and Mesh classes for rendering text in a scene.
     *          The class is designed to be used in a game engine or graphics application that uses OpenGL for rendering.
     *          The text can be rendered in various
     */
    class TextRenderer {
    public:
        std::map<GLchar, Character> Characters;
        ShaderProgram textShader;
        GLuint VAO, VBO, EBO;

        /**
         * @brief Initializes the TextRenderer with the specified screen dimensions.
         * @param screenWidth Width of the screen.
         * @param screenHeight Height of the screen.
         */
        TextRenderer(GLuint screenWidth, GLuint screenHeight);

        /**
         * @brief loads a font from the specified file path and size.
         * @param fontPath The path to the font file.
         * @param fontSize The size of the font to load.
         */
        void loadFont(const std::string &fontPath, GLuint fontSize);

        /**
         * @brief Renders the specified text at the given position with the specified scale and color.
         * @param text The text to render.
         * @param x The x-coordinate of the text position.
         * @param y The y-coordinate of the text position.
         * @param scale The scale factor for the text size.
         * @param color The color of the text as a glm::vec3 (RGB).
         */
        void renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

        /**
         * @brief Resizes the text rendering viewport to the specified width and height.
         * @param width The new width of the viewport.
         * @param height The new height of the viewport.
         */
        void resize(GLuint width, GLuint height);
    };
}

#endif //CBIT_TEXTRENDERER_H
