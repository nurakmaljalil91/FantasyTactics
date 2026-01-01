/**
 * @file    Animator.h
 * @brief   Animator class for evaluating skeletal animations.
 * @details Computes final bone matrices for the current animation clip.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#ifndef CBIT_ANIMATOR_H
#define CBIT_ANIMATOR_H

#include <vector>
#include <glm/glm.hpp>
#include "AnimationClip.h"
#include "graphics/meshes/SkinnedMesh.h"

namespace cbit {
    class Animator {
    public:
        Animator() = default;

        void update(const AnimationClip *clip, const SkinnedMesh &mesh, float deltaTime, bool loop, float speed);
        void reset();
        [[nodiscard]] bool isFinished() const { return _finished; }
        [[nodiscard]] const std::vector<glm::mat4> &getFinalBoneMatrices() const { return _finalBoneMatrices; }

    private:
        void _calculateBoneTransform(int nodeIndex,
                                     const glm::mat4 &parentTransform,
                                     const AnimationClip &clip,
                                     const SkinnedMesh &mesh);

        glm::mat4 _interpolateChannel(const AnimationChannel &channel, float animationTime) const;
        glm::vec3 _interpolatePosition(const AnimationChannel &channel, float animationTime) const;
        glm::vec3 _interpolateScale(const AnimationChannel &channel, float animationTime) const;
        glm::quat _interpolateRotation(const AnimationChannel &channel, float animationTime) const;

        int _getPositionIndex(const AnimationChannel &channel, float animationTime) const;
        int _getRotationIndex(const AnimationChannel &channel, float animationTime) const;
        int _getScaleIndex(const AnimationChannel &channel, float animationTime) const;

        float _currentTime = 0.0f;
        bool _finished = false;
        std::vector<glm::mat4> _finalBoneMatrices;
    };
}

#endif //CBIT_ANIMATOR_H
