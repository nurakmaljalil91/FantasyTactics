/**
 * @file    Texture2D.cpp
 * @brief   Implementation of the Texture2D class for managing 2D textures in OpenGL.
 * @details The Texture2D class is responsible for loading, binding, and managing 2D textures in OpenGL.
 *          It uses the stb_image library to load images from files and creates OpenGL texture objects.
 *          The class provides methods to load textures from files, bind and unbind textures, and manage texture parameters.
 *          It is designed to be used in a graphics rendering context where OpenGL is used for rendering 2D and 3D graphics.
 *          The class handles texture loading, flipping images vertically, setting texture parameters such as wrapping and filtering,
 *          and generating mipmaps if requested.
 *          It also provides methods to bind the texture to a specific texture unit and unbind it when no longer needed.
 *          The class is designed to be efficient and easy to use, allowing developers to quickly integrate 2D textures into their OpenGL applications.
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-01
 */

#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>
#include "../../utilities/Logger.h"

// constructor
cbit::Texture2D::Texture2D() : _texture(0) {
}

// destructor
cbit::Texture2D::~Texture2D() {
    glDeleteTextures(1, &_texture);
}

bool cbit::Texture2D::loadTexture(const std::string &filename, const bool generateMipMaps) {
    int width, height, components;
    unsigned char *imageData = stbi_load(filename.c_str(), &width, &height, &components, 4);

    if (imageData == nullptr) {
        Logger::log()->error("Texture loading failed for texture: {}", filename);
        return false;
    }

    // flip image vertically
    const int widthInBytes = width * 4;
    unsigned char *top = nullptr;
    unsigned char *bottom = nullptr;
    unsigned char temp = 0;
    const int halfHeight = height / 2;

    for (int row = 0; row < halfHeight; row++) {
        top = imageData + row * widthInBytes;
        bottom = imageData + (height - row - 1) * widthInBytes;

        for (int col = 0; col < widthInBytes; col++) {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    // all upcoming GL_TEXTURE_2D operations will affect our texture object (mTexture)

    // set texture parameters
    // GL_CLAMP_TO_EDGE
    // GL_REPEAT
    // GL_MIRRORED_REPEAT
    // GL_CLAMP_TO_BORDER
    // GL_LINEAR
    // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_CLAMP_TO_EDGE

    // set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // enable anisotropic filtering
    //    float aniso = 0.0f;
    //    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

    // load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, imageData);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // free memory
    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture when done so we don't accidentally mess up our texture

    return true;
}

// Binds texture to texture unit
void cbit::Texture2D::bind(const GLuint texUnit) const {
    assert(texUnit >= 0 && texUnit <= 32);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

// Unbinds texture
void cbit::Texture2D::unbind(const GLuint texUnit) {
    // assert(texUnit >= 0 && texUnit <= 31);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
