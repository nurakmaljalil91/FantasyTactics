/**
 * @file    Quad2D.cpp
 * @brief   Implement the Quad2D
 * @author  Nur Akmal bin Jalil
 * @date    2025-07-28
 */

// UIQuad.cpp
#include "Quad2D.h"

// 6 vertices × (2 pos + 2 uv)
static constexpr float quadVertices[] = {
    //  pos      uv
    0, 1, 0, 1,
    1, 0, 1, 0,
    0, 0, 0, 0,

    0, 1, 0, 1,
    1, 1, 1, 1,
    1, 0, 1, 0,
};

Quad2D::Quad2D() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,GL_STATIC_DRAW);
    // aPos → loc0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 4 * sizeof(float), static_cast<void *>(nullptr));
    // aTexCoord → loc1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glBindVertexArray(0);
}

Quad2D::~Quad2D() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Quad2D::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
