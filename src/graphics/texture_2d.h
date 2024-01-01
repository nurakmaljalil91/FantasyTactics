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

class texture_2d {
public:

    texture_2d(std::shared_ptr<spdlog::logger> pLogger);
    ~texture_2d();

    bool loadTexture(const std::string& filename, bool generateMipMaps = true);
    void bind(GLuint texUnit = 0);

//    void setWrap(GLint wrapModeHorizontal, GLint wrapModeVertical);
//    void setMinFilter(GLint minFilter);
//    void setMagFilter(GLint magFilter);

//    int getWidth() const { return width; }
//    int getHeight() const { return height; }
//    int getNumComponents() const { return numComponents; }

private:
        texture_2d(const texture_2d& rhs) {}
        texture_2d& operator=(const texture_2d& rhs) {}

        std::shared_ptr<spdlog::logger> logger;
//        std::string fileToString(const std::string& filename);
//        void checkTextureErrors(GLuint texture);

        GLuint texture;
//        int width;
//        int height;
//        int numComponents;
};


#endif //FANTASYTACTICS_TEXTURE_2D_H
