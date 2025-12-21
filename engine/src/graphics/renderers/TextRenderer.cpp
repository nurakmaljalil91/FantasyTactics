/**
 * @file    TextRenderer.cpp
 * @brief   Implementation of the TextRenderer class for rendering text in OpenGL.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-28
 */

#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "glm/ext/matrix_clip_space.hpp"

TextRenderer::TextRenderer(const GLuint screenWidth, const GLuint screenHeight) : VAO(0), VBO(0), EBO(0) {
    textShader.loadShaders("resources/shaders/text.vert", "resources/shaders/text.frag");
    textShader.use();

    const glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f,
                                            static_cast<GLfloat>(screenHeight));

    textShader.setUniform("projection", projection);

    // setup VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 6 vertices, 4 bytes per vertex (x, y, z, w)
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<void *>(nullptr));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
}

void TextRenderer::loadFont(const std::string &fontPath, GLuint fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        Logger::log()->error("Could not initialize FreeType Library");
        return;
    }
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        Logger::log()->error("Failed to load font: {}", fontPath);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            Logger::log()->error("Failed to load Glyph: {}", c);
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::renderText(const std::string &text, GLfloat x, const GLfloat y, const GLfloat scale,
                              const glm::vec3 color) {
    textShader.use();
    textShader.setUniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (auto c: text) {
        auto it = Characters.find(c);
        if (it == Characters.end()) {
            continue;
        }


        auto &[textureID, size, bearing, advance] = Characters[c];
        const GLfloat xPosition = x + bearing.x * scale;
        const GLfloat yPosition = y + (bearing.y - size.y) * scale;
        const GLfloat width = size.x * scale;
        const GLfloat height = size.y * scale;

        // update VBO for each character quad
        const GLfloat vertices[6][4] = {
            {xPosition, yPosition + height, 0.0f, 0.0f},
            {xPosition, yPosition, 0.0f, 1.0f},
            {xPosition + width, yPosition, 1.0f, 1.0f},

            {xPosition, yPosition + height, 0.0f, 0.0f},
            {xPosition + width, yPosition, 1.0f, 1.0f},
            {xPosition + width, yPosition + height, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (advance >> 6) * scale; // advance is in 1/64 pixels
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::resize(GLuint width, GLuint height) {
    // update projection matrix
    const glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f,
                                            static_cast<GLfloat>(height));
    textShader.use();
    textShader.setUniform("projection", projection);
}
