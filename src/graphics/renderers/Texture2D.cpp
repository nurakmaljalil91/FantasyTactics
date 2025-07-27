//
// Created by User on 1/1/2024.
//

#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>
#include "../../utilities/Logger.h"

// constructor
Texture2D::Texture2D() {
}

// destructor
Texture2D::~Texture2D() {
    glDeleteTextures(1, &_texture);
}

// Loads texture from a file
// http://nothings.org/stb_image.h
// Creates mip maps if generateMipMaps is true.
bool Texture2D::loadTexture(const std::string &filename, bool generateMipMaps) {
    int width, height, components;
    unsigned char *imageData = stbi_load(filename.c_str(), &width, &height, &components, 4);

    if (imageData == nullptr) {
        Logger::log()->error("Texture loading failed for texture: {}", filename);
        return false;
    }

    // flip image vertically
    int widthInBytes = width * 4;
    unsigned char *top = nullptr;
    unsigned char *bottom = nullptr;
    unsigned char temp = 0;
    int halfHeight = height / 2;

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
void Texture2D::bind(GLuint texUnit) {
    assert(texUnit >= 0 && texUnit <= 32);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

// Unbinds texture
void Texture2D::unbind(GLuint texUnit) {
    // assert(texUnit >= 0 && texUnit <= 31);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
