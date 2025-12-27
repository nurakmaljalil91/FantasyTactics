/**
 * @file    Texture2D.h
 * @brief   Header file for the Texture2D class.
 * @details This file defines the Texture2D class which is responsible for loading and managing 2D textures in OpenGL.
 *          It provides methods to load textures from files, bind and unbind textures, and manage texture parameters.
 *          The class uses the stb_image library for image loading and GLAD for OpenGL function loading.
 *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
 *          The Texture2D class
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-01
 */

#ifndef CBIT_TEXTURE_2D_H
#define CBIT_TEXTURE_2D_H

#include "OpenGLInclude.h"
#include <string>
#include <iostream>

namespace cbit {
    /**
     * @class Texture2D
     * @brief This class represents a 2D texture in OpenGL.
     * @details The Texture2D class is responsible for loading, binding, and managing 2D textures in OpenGL.
     *          It uses the stb_image library to load images from files and creates OpenGL texture objects.
     *          The class provides methods to load textures from files, bind and unbind textures, and manage texture parameters.
     *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
     */
    class Texture2D {
    public:
        /**
         * Default constructor.
         * Initializes an empty texture object. The texture will be created when loadTexture()
         */
        Texture2D();

        /**
         * Default destructor.
         * Cleans up the OpenGL texture object.
         */
        ~Texture2D();

        /**
         * Loads a texture from a file.
         * Loads texture from a file
         * http://nothings.org/stb_image.h
         * Creates mip maps if generateMipMaps is true.
         * @param filename file a path to the texture image.
         * @param generateMipMaps should mipmaps be generated?
         * @return is the texture loaded successfully?
         */
        bool loadTexture(const std::string &filename, bool generateMipMaps = true);

        /**
         * Returns the OpenGL texture ID.
         * @return GLuint representing the texture ID.
         */
        [[nodiscard]] GLuint getTexture() const { return _texture; }

        /**
         * Binds the texture to a specified texture unit.
         * @param texUnit The texture unit to bind the texture to (default is 0).
         * @details This method activates the specified texture unit and binds the texture to it.
         *          It is necessary to call this before rendering objects that use this texture.
         */
        void bind(GLuint texUnit = 0) const;

        /**
         * Unbinds the texture from the specified texture unit.
         * @param texUnit The texture unit to unbind the texture from (default is 0).
         * @details This method unbinds the texture from the specified texture unit, effectively disabling it.
         *          It is useful when you want to stop using a texture in later rendering calls.
         */
        static void unbind(GLuint texUnit = 0);

    private:
        GLuint _texture;
    };
}
#endif //CBIT_TEXTURE_2D_H
