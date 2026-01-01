/**
 * @file    SkinnedMesh.cpp
 * @brief   Implementation of the SkinnedMesh class for loading skinned models.
 * @details Loads skinned meshes and skeleton hierarchy using Assimp.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#include "SkinnedMesh.h"
#include <cstddef>
#include <cfloat>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <utility>
#include <glm/gtc/matrix_inverse.hpp>
#include "utilities/Logger.h"

namespace {
    glm::mat4 toGlmMatrix(const aiMatrix4x4 &matrix) {
    glm::mat4 result;
    result[0][0] = matrix.a1;
    result[1][0] = matrix.a2;
    result[2][0] = matrix.a3;
    result[3][0] = matrix.a4;
    result[0][1] = matrix.b1;
    result[1][1] = matrix.b2;
    result[2][1] = matrix.b3;
    result[3][1] = matrix.b4;
    result[0][2] = matrix.c1;
    result[1][2] = matrix.c2;
    result[2][2] = matrix.c3;
    result[3][2] = matrix.c4;
    result[0][3] = matrix.d1;
    result[1][3] = matrix.d2;
    result[2][3] = matrix.d3;
    result[3][3] = matrix.d4;
        return result;
    }

}

cbit::SkinnedMesh::SkinnedMesh() = default;

cbit::SkinnedMesh::~SkinnedMesh() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

bool cbit::SkinnedMesh::loadFromFile(const std::string &filename) {
    Assimp::Importer importer;
    const unsigned int flags = aiProcess_Triangulate
                               | aiProcess_JoinIdenticalVertices
                               | aiProcess_GenNormals;

    const aiScene *scene = importer.ReadFile(filename, flags);
    if (!scene || !scene->HasMeshes()) {
        Logger::log()->error("Assimp failed to load skinned model {}: {}", filename, importer.GetErrorString());
        return false;
    }

    if (scene->mNumMeshes > 1) {
        Logger::log()->warn("Assimp model {} has {} meshes; loading first mesh only.", filename, scene->mNumMeshes);
    }

    _vertices.clear();
    _indices.clear();
    _boneMap.clear();
    _boneInfo.clear();
    _nodes.clear();
    _rootNodeIndex = -1;

    const aiMesh *mesh = scene->mMeshes[0];
    _vertices.resize(mesh->mNumVertices);

    glm::vec3 minPos(FLT_MAX);
    glm::vec3 maxPos(-FLT_MAX);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        SkinnedVertex vertex{};
        const auto &position = mesh->mVertices[i];
        vertex.position = glm::vec3(position.x, position.y, position.z);

        if (mesh->HasNormals()) {
            const auto &normal = mesh->mNormals[i];
            vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
        }

        if (mesh->HasTextureCoords(0)) {
            const auto &uv = mesh->mTextureCoords[0][i];
            vertex.textureCoordinates = glm::vec2(uv.x, uv.y);
        }

        _vertices[i] = vertex;

        minPos = glm::min(minPos, vertex.position);
        maxPos = glm::max(maxPos, vertex.position);
    }

    _indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace &face = mesh->mFaces[i];
        if (face.mNumIndices != 3) {
            continue;
        }
        _indices.push_back(face.mIndices[0]);
        _indices.push_back(face.mIndices[1]);
        _indices.push_back(face.mIndices[2]);
    }

    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
        const aiBone *bone = mesh->mBones[boneIndex];
        const std::string boneName = bone->mName.C_Str();

        int boneId = 0;
        const auto existing = _boneMap.find(boneName);
        if (existing == _boneMap.end()) {
            if (_boneInfo.size() >= kMaxBones) {
                Logger::log()->warn("Skinned mesh {} exceeds max bones ({}). Skipping bone {}.",
                                    filename, kMaxBones, boneName);
                continue;
            }
            boneId = static_cast<int>(_boneInfo.size());
            _boneMap[boneName] = boneId;
            BoneInfo info;
            info.offset = toGlmMatrix(bone->mOffsetMatrix);
            _boneInfo.push_back(info);
        } else {
            boneId = existing->second;
        }

        for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
            const aiVertexWeight &weight = bone->mWeights[weightIndex];
            if (weight.mVertexId < _vertices.size()) {
                _setVertexBoneData(_vertices[weight.mVertexId], boneId, weight.mWeight);
            }
        }
    }

    for (auto &vertex: _vertices) {
        const float totalWeight = vertex.weights.x + vertex.weights.y + vertex.weights.z + vertex.weights.w;
        if (totalWeight > 0.0f) {
            vertex.weights /= totalWeight;
        }
    }

    if (scene->mRootNode) {
        _globalInverseTransform = glm::inverse(toGlmMatrix(scene->mRootNode->mTransformation));
        _rootNodeIndex = _buildSkeletonNodes(scene->mRootNode);
    }

    Logger::log()->info("Skinned mesh {} bounds min=({}, {}, {}) max=({}, {}, {})",
                        filename,
                        minPos.x, minPos.y, minPos.z,
                        maxPos.x, maxPos.y, maxPos.z);

    _initializeBuffers();
    _loaded = true;
    return true;
}

void cbit::SkinnedMesh::draw() const {
    if (!_loaded) return;
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void cbit::SkinnedMesh::_initializeBuffers() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(_vertices.size() * sizeof(SkinnedVertex)), _vertices.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(_indices.size() * sizeof(unsigned int)), _indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                          reinterpret_cast<GLvoid *>(offsetof(SkinnedVertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                          reinterpret_cast<GLvoid *>(offsetof(SkinnedVertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                          reinterpret_cast<GLvoid *>(offsetof(SkinnedVertex, textureCoordinates)));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 4, GL_INT, sizeof(SkinnedVertex),
                           reinterpret_cast<GLvoid *>(offsetof(SkinnedVertex, boneIds)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(SkinnedVertex),
                          reinterpret_cast<GLvoid *>(offsetof(SkinnedVertex, weights)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
}

void cbit::SkinnedMesh::_setVertexBoneData(SkinnedVertex &vertex, const int boneId, const float weight) {
    if (boneId < 0 || boneId >= kMaxBones) {
        return;
    }
    for (int i = 0; i < kMaxBoneInfluences; ++i) {
        if (vertex.weights[i] == 0.0f) {
            vertex.boneIds[i] = boneId;
            vertex.weights[i] = weight;
            return;
        }
    }
}

int cbit::SkinnedMesh::_buildSkeletonNodes(const aiNode *node) {
    SkeletonNode newNode;
    newNode.name = node->mName.C_Str();
    newNode.transform = toGlmMatrix(node->mTransformation);
    const int nodeIndex = static_cast<int>(_nodes.size());
    _nodes.push_back(std::move(newNode));
    _nodes[nodeIndex].children.reserve(node->mNumChildren);

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        int childIndex = _buildSkeletonNodes(node->mChildren[i]);
        _nodes[nodeIndex].children.push_back(childIndex);
    }

    return nodeIndex;
}
