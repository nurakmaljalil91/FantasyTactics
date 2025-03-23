//
// Created by User on 11/1/2024.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../utilities/logger.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh();

    ~Mesh();

    bool LoadObj(const std::string &filename);

    void Draw();

protected:
    void InitializeBuffers();

    std::vector<Vertex> mVertices;
private:

    bool mLoaded;
    GLuint mVAO;
    GLuint mVBO;
    // GLuint mIBO;
};


#endif //MESH_H
