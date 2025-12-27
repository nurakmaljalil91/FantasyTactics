/**
 * @file    Mesh.cpp
 * @brief   Implementation of the Mesh class for loading and rendering 3D models.
 * @details Mesh class that can load Wavefront OBJ files and render them using OpenGL.
 *          This implementation is simplified and assumes the OBJ file contains only triangles
 * @author  Nur Akmal bin Jalil
 * @date    2024-01-11
 */

#include "Mesh.h"
#include <sstream>
#include <fstream>
#include <numeric>
#include "utilities/Logger.h"

/**
 * @brief  Splits string according to some substring and returns it as a vector.
 * @param  stringData
 * @param  delimiter
 * @return returns a vector of strings, each element is a substring of the original string
 */
std::vector<std::string> splitString(std::string stringData, const std::string &delimiter) {
    std::vector<std::string> results;
    while (true) {
        const int pos = static_cast<int>(stringData.find(delimiter));
        if (pos == -1) {
            results.push_back(stringData);
            break;
        }
        results.push_back(stringData.substr(0, pos));
        stringData = stringData.substr(pos + 1, stringData.size() - pos - 1);
    }
    return results;
}

cbit::Mesh::Mesh()
    : loaded(false),
      VAO(0),
      VBO(0),
      EBO(0) {
}

cbit::Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool cbit::Mesh::loadObj(const std::string &filename) {
    if (filename.find(".obj") != std::string::npos) {
        std::vector<glm::vec3> tempNormals;
        std::vector<glm::vec2> tempUVs;
        std::vector<glm::vec3> tempVertices;
        std::vector<unsigned int> normalIndices;
        std::vector<unsigned int> uvIndices;
        std::vector<unsigned int> vertexIndices;
        std::ifstream fin(filename, std::ios::in);
        if (!fin) {
            Logger::log()->error("Unable to open file {}", filename);
            return false;
        }

        Logger::log()->info("Loading OBJ file {}...", filename);

        std::string lineBuffer;

        while (std::getline(fin, lineBuffer)) {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v") {
                glm::vec3 vertex;
                int dim = 0;
                while (dim < 3 && ss >> vertex[dim]) {
                    dim++;
                }
                tempVertices.push_back(vertex);
            } else if (cmd == "vt") {
                glm::vec2 uv;
                int dim = 0;
                while (dim < 2 && ss >> uv[dim])
                    dim++;

                tempUVs.push_back(uv);
            } else if (cmd == "vn") {
                glm::vec3 normal;
                int dim = 0;
                while (dim < 3 && ss >> normal[dim])
                    dim++;
                normal = glm::normalize(normal);
                tempNormals.push_back(normal);
            } else if (cmd == "f") {
                std::string faceData;
                int vertexIndex, uvIndex, normalIndex;

                while (ss >> faceData) {
                    std::vector<std::string> data = splitString(faceData, "/");

                    if (!data[0].empty()) {
                        sscanf_s(data[0].c_str(), "%d", &vertexIndex);
                        vertexIndices.push_back(vertexIndex);
                    }

                    if (!data.empty()) {
                        // Is face format v//vn?  If data[1] is empty string, then
                        // this vertex has no texture coordinate
                        if (!data[1].empty()) {
                            sscanf_s(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }

                    if (data.size() >= 2) {
                        // Does this vertex have normal?
                        if (!data[2].empty()) {
                            sscanf_s(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
            }
        }

        // close file
        fin.close();

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            Vertex meshVertex{};

            // Get the attributes using the indices

            if (!tempVertices.empty()) {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                meshVertex.position = vertex;
            }

            if (!tempNormals.empty()) {
                glm::vec3 normal = tempNormals[normalIndices[i] - 1];
                meshVertex.normal = normal;
            }

            if (!tempUVs.empty()) {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                meshVertex.textureCoordinates = uv;
            }

            vertices.push_back(meshVertex);
        }

        indices.resize(vertices.size());
        std::iota(indices.begin(), indices.end(), 0u);

        // Create and initialize the buffers
        initializeBuffers();

        return loaded = true;
    }

    return false;
}

void cbit::Mesh::initializeBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<GLvoid *>(nullptr));
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}

void cbit::Mesh::draw() {
    if (!loaded) return;

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
