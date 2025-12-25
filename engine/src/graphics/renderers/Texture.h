/**
 * @file    Texture.h
 * @brief   Header file for the Texture class.
 * @details This file contains the definition of the Texture class, which is used to represent
 *          a texture in the game. The Texture class is responsible for loading and binding textures
 *          to the rendering pipeline.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGLInclude.h"
#include <string>

namespace cbit {
    /**
     * @class Texture
     * @brief Represents a texture in the game.
     * @details The Texture class is responsible for loading, binding, and managing textures in OpenGL.
     *          It provides methods to load textures from files, bind and unbind textures, and manage
     *          texture parameters. The class is designed to be used in a graphics rendering context where
     *          OpenGL is used for rendering 2D and 3D graphics. It can be integrated with other components such as
     *          shaders and renderers to create a complete rendering pipeline.
     *          The class uses the stb_image library for image loading and GLAD for OpenGL function loading.
     *          It is designed to be efficient and easy to use, allowing developers to quickly integrate
     */
    class Texture {
    public:
        /**
         * Default constructor.
         * Initializes an empty texture object. The texture will be created when loadTexture() is called
         */
        Texture();

        /**
         * Default destructor.
         * Cleans up the OpenGL texture object.
         */
        ~Texture();

        // disable copy
        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        // enable move
        Texture(Texture &&o) noexcept
            : _textureID(o._textureID) { o._textureID = 0; }

        Texture &operator=(Texture &&o) noexcept {
            if (this != &o) {
                if (_textureID)
                    glDeleteTextures(1, &_textureID);
                _textureID = o._textureID;
                o._textureID = 0;
            }
            return *this;
        }

        /**
         * Loads a texture from a file.
         * @param path The file path to the texture image.
         * @return True if the texture was loaded successfully, false otherwise.
         * @details This method uses stb_image to load the texture image and creates an OpenGL texture object.
         *          It sets the texture parameters such as wrapping and filtering options.
         */
        bool loadTexture(const std::string &path);

        /**
         * Returns the OpenGL texture ID.
         * @return GLuint representing the texture ID.
         * @details This method returns the OpenGL texture ID associated with this texture object.
         */
        [[nodiscard]] GLuint getID() const { return _textureID; }

        /**
         * Binds the texture to the current OpenGL context.
         * @details This method binds the texture to the current OpenGL context, making it active for rendering.
         *          It should be called before rendering any objects that use this texture.
         */
        void bind() const;

    private:
        GLuint _textureID;
    };
}
#endif //TEXTURE_H
