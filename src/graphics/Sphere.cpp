//
// Created by User on 26/3/2025.
//

#include "Sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // for pi constants
#include <cmath>

Sphere::Sphere(float radius, int stacks, int slices)
{
    // We’ll create (stacks+1)*(slices+1) "grid" of vertices,
    // then form 2 triangles per grid cell.

    // Helper lambda for generating one vertex (position, normal, texcoords)
    auto makeVertex = [&](int stackIdx, int sliceIdx)
    {
        // Range: stackIdx in [0..stacks], sliceIdx in [0..slices]
        float theta = glm::pi<float>() * (float)stackIdx / (float)stacks;          // 0..pi
        float phi   = glm::two_pi<float>() * (float)sliceIdx / (float)slices;      // 0..2*pi

        // spherical coordinates (assuming "theta" is polar angle from +y down,
        //  and "phi" is angle around the y-axis from +z toward +x, for example).
        // A common alternative is "theta = latitude, phi = longitude" – up to you.
        // Here, let's do a more typical approach:
        //    x = sin(theta)*cos(phi)
        //    y = cos(theta)
        //    z = sin(theta)*sin(phi)
        // so the "poles" are at theta=0 (top) and theta=pi (bottom).

        float x = radius * sin(theta) * cos(phi);
        float y = radius * cos(theta);
        float z = radius * sin(theta) * sin(phi);

        Vertex v;
        v.position = glm::vec3(x, y, z);

        // For a sphere, the normal is (position / radius) if radius != 0
        v.normal = glm::normalize(glm::vec3(x, y, z));

        // A simple spherical UV map:
        //   u = phi / (2*pi)
        //   v = 1 - (theta / pi)
        float u = phi / glm::two_pi<float>();
        float w = 1.0f - (theta / glm::pi<float>()); // (use 'w' to avoid naming conflict with v.position)
        v.texCoords = glm::vec2(u, w);

        return v;
    };

    // Generate all vertices in a 2D grid [stacks+1] x [slices+1]
    // We’ll store them in a helper 2D array, or push directly into triangles.
    // For each cell, we create 2 triangles:
    //   (i,j), (i+1,j), (i,j+1)  and  (i+1,j), (i+1,j+1), (i,j+1)
    // with wrap-around in the slice (longitude) direction.

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            // current slice j, next slice j+1 (wrapped)
            int jNext = (j + 1) % slices;
            // The four corners of this patch:
            Vertex v0 = makeVertex(i,   j);
            Vertex v1 = makeVertex(i+1, j);
            Vertex v2 = makeVertex(i,   jNext);
            Vertex v3 = makeVertex(i+1, jNext);

            // Triangle 1
            mVertices.push_back(v0);
            mVertices.push_back(v1);
            mVertices.push_back(v2);

            // Triangle 2
            mVertices.push_back(v1);
            mVertices.push_back(v3);
            mVertices.push_back(v2);
        }
    }

    // Upload to GPU
    initializeBuffers();

    // Mark loaded
    mLoaded = true;
}