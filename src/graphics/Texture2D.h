//
// Created by User on 1/1/2024.
//

#ifndef FANTASYTACTICS_TEXTURE_2D_H
#define FANTASYTACTICS_TEXTURE_2D_H

#include "glad/glad.h"
#include <string>
#include <iostream>
#include <cassert>


#include "../utilities/logger.h"

class Texture2D {
public:
    Texture2D();

    ~Texture2D();

    bool loadTexture(const std::string &filename, bool generateMipMaps = true);

    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);

    //    void setWrap(GLint wrapModeHorizontal, GLint wrapModeVertical);
    //    void setMinFilter(GLint minFilter);
    //    void setMagFilter(GLint magFilter);

    //    int getWidth() const { return width; }
    //    int getHeight() const { return height; }
    //    int getNumComponents() const { return numComponents; }

private:
    Texture2D(const Texture2D &rhs);

    Texture2D &operator=(const Texture2D &rhs);


    //        std::string fileToString(const std::string& filename);
    //        void checkTextureErrors(GLuint texture);

    GLuint mTexture;
    //        int width;
    //        int height;
    //        int numComponents;
};


#endif //FANTASYTACTICS_TEXTURE_2D_H
