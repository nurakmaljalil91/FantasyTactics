//
// Created by User on 25/3/2025.
//

#include "Circle.h"

Circle::Circle(float radius, int segments) {

    // Reserve enough space: 1 center vertex + (segments + 1) ring vertices
    // (the extra +1 vertex repeats the first ring vertex so the fan closes)
    mVertices.reserve(segments + 2);

    // 1) Add the center vertex
    Vertex center;
    center.position = glm::vec3(0.0f, 0.0f, 0.0f);
    center.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    center.texCoords = glm::vec2(0.5f, 0.5f);
    mVertices.push_back(center);

    // 2) Create ring vertices around the circle
    float delta = 2.0f * static_cast<float>(M_PI) / segments;
    for (int i = 0; i <= segments; i++) {
        float angle = i * delta;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        Vertex v;
        v.position = glm::vec3(x, y, 0.0f);
        v.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        // Simple UV mapping: center at (0.5, 0.5), radius 0.5
        v.texCoords = glm::vec2(0.5f + 0.5f * std::cos(angle),
                                0.5f + 0.5f * std::sin(angle));
        mVertices.push_back(v);
    }

    // 3) Upload to GPU
    initializeBuffers();

    // 4) Mark as loaded so Mesh::draw() won’t skip it
    mLoaded = true;
}


// Override draw():
void Circle::draw()
{
    if (!mLoaded) return;

    glBindVertexArray(mVAO);
    // For a filled circle, use GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(mVertices.size()));
    glBindVertexArray(0);
}