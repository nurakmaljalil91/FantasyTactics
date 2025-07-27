/**
 * @file    Circle.cpp
 * @brief   Circle class for rendering a filled circle mesh.
 * @details Initializes a circle mesh with a specified radius and number of segments.
 *          The circle is centered at the origin and lies on the XY plane.
 * @author  Nur Akmal bin Jalil
 * @date    2025-03-25
 */
#include "Circle.h"

Circle::Circle(float radius, int segments) {
    // Reserve enough space: 1 center vertex + (segments + 1) ring vertices
    // (the extra +1 vertex repeats the first ring vertex so the fan closes)
    vertices.reserve(segments + 2);

    // 1) Add the center vertex
    Vertex center;
    center.position = glm::vec3(0.0f, 0.0f, 0.0f);
    center.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    center.texCoords = glm::vec2(0.5f, 0.5f);
    vertices.push_back(center);

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
        vertices.push_back(v);
    }

    // 3) Upload to GPU
    initializeBuffers();

    // 4) Mark as loaded so Mesh::draw() won’t skip it
    loaded = true;
}

Circle::~Circle() {
}


// Override draw():
void Circle::draw() {
    if (!loaded) return;

    glBindVertexArray(VAO);
    // For a filled circle, use GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);
}
