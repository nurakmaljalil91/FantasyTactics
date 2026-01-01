/**
 * @file    SkinnedMesh.h
 * @brief   Skinned mesh class for loading and rendering animated 3D models.
 * @details Loads mesh geometry, bone weights, and skeleton hierarchy using Assimp.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#ifndef CBIT_SKINNEDMESH_H
#define CBIT_SKINNEDMESH_H

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "OpenGLInclude.h"

namespace cbit {
    constexpr int kMaxBoneInfluences = 4;
    constexpr int kMaxBones = 128;

    struct SkinnedVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::ivec4 boneIds{0};
        glm::vec4 weights{0.0f};
    };

    struct BoneInfo {
        glm::mat4 offset{1.0f};
    };

    struct SkeletonNode {
        std::string name;
        glm::mat4 transform{1.0f};
        std::vector<int> children;
    };

    /**
     * @class SkinnedMesh
     * @brief Represents a skinned mesh with bone weights and a skeleton hierarchy.
     */
    class SkinnedMesh {
    public:
        SkinnedMesh();
        ~SkinnedMesh();

        /**
         * @brief   Loads a model file supported by Assimp (e.g., FBX) with skinning data.
         * @param   filename
         * @return  true if the model was loaded successfully.
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief   Renders the skinned mesh using OpenGL.
         */
        void draw() const;

        [[nodiscard]] const std::unordered_map<std::string, int> &getBoneMap() const { return _boneMap; }
        [[nodiscard]] const std::vector<BoneInfo> &getBoneInfo() const { return _boneInfo; }
        [[nodiscard]] const std::vector<SkeletonNode> &getSkeletonNodes() const { return _nodes; }
        [[nodiscard]] int getRootNodeIndex() const { return _rootNodeIndex; }
        [[nodiscard]] const glm::mat4 &getGlobalInverseTransform() const { return _globalInverseTransform; }
        [[nodiscard]] size_t getBoneCount() const { return _boneInfo.size(); }
        [[nodiscard]] size_t getVertexCount() const { return _vertices.size(); }
        [[nodiscard]] size_t getIndexCount() const { return _indices.size(); }

    private:
        void _initializeBuffers();
        void _setVertexBoneData(SkinnedVertex &vertex, int boneId, float weight);
        int _buildSkeletonNodes(const aiNode *node);

        std::vector<SkinnedVertex> _vertices;
        std::vector<unsigned int> _indices;
        std::unordered_map<std::string, int> _boneMap;
        std::vector<BoneInfo> _boneInfo;
        std::vector<SkeletonNode> _nodes;
        glm::mat4 _globalInverseTransform{1.0f};
        int _rootNodeIndex = -1;

        bool _loaded = false;
        GLuint _vao = 0;
        GLuint _vbo = 0;
        GLuint _ebo = 0;
    };
}

#endif //CBIT_SKINNEDMESH_H
