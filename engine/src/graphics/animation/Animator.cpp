/**
 * @file    Animator.cpp
 * @brief   Implementation of Animator for evaluating skeletal animations.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#include "Animator.h"

#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace {
    constexpr float kDefaultTicksPerSecond = 25.0f;
}

void cbit::Animator::update(const AnimationClip *clip,
                            const SkinnedMesh &mesh,
                            const float deltaTime,
                            const bool loop,
                            const float speed) {
    if (!clip || mesh.getRootNodeIndex() < 0) {
        return;
    }

    if (_finalBoneMatrices.size() != mesh.getBoneCount()) {
        _finalBoneMatrices.assign(mesh.getBoneCount(), glm::mat4(1.0f));
    }

    const float ticksPerSecond = clip->getTicksPerSecond() > 0.0f ? clip->getTicksPerSecond() : kDefaultTicksPerSecond;
    _currentTime += deltaTime * ticksPerSecond * speed;
    _finished = false;

    const float duration = clip->getDuration();
    if (duration > 0.0f) {
        if (loop) {
            _currentTime = std::fmod(_currentTime, duration);
        } else if (_currentTime >= duration) {
            _currentTime = duration;
            _finished = true;
        }
    }

    _calculateBoneTransform(mesh.getRootNodeIndex(), glm::mat4(1.0f), *clip, mesh);
}

void cbit::Animator::reset() {
    _currentTime = 0.0f;
    _finished = false;
}

void cbit::Animator::_calculateBoneTransform(const int nodeIndex,
                                             const glm::mat4 &parentTransform,
                                             const AnimationClip &clip,
                                             const SkinnedMesh &mesh) {
    const auto &nodes = mesh.getSkeletonNodes();
    if (nodeIndex < 0 || nodeIndex >= static_cast<int>(nodes.size())) {
        return;
    }

    const auto &node = nodes[nodeIndex];
    glm::mat4 nodeTransform = node.transform;

    const auto &channels = clip.getChannels();
    const auto channelIt = channels.find(node.name);
    if (channelIt != channels.end()) {
        nodeTransform = _interpolateChannel(channelIt->second, _currentTime);
    }

    const glm::mat4 globalTransform = parentTransform * nodeTransform;

    const auto &boneMap = mesh.getBoneMap();
    const auto boneIt = boneMap.find(node.name);
    if (boneIt != boneMap.end()) {
        const int boneIndex = boneIt->second;
        const auto &boneInfo = mesh.getBoneInfo();
        if (boneIndex >= 0 && boneIndex < static_cast<int>(boneInfo.size())) {
            _finalBoneMatrices[boneIndex] =
                mesh.getGlobalInverseTransform() * globalTransform * boneInfo[boneIndex].offset;
        }
    }

    for (const int childIndex: node.children) {
        _calculateBoneTransform(childIndex, globalTransform, clip, mesh);
    }
}

glm::mat4 cbit::Animator::_interpolateChannel(const AnimationChannel &channel, const float animationTime) const {
    const glm::vec3 translation = _interpolatePosition(channel, animationTime);
    const glm::quat rotation = _interpolateRotation(channel, animationTime);
    const glm::vec3 scale = _interpolateScale(channel, animationTime);

    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
    const glm::mat4 rotationMatrix = glm::toMat4(rotation);
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 cbit::Animator::_interpolatePosition(const AnimationChannel &channel, const float animationTime) const {
    if (channel.positions.empty()) {
        return glm::vec3(0.0f);
    }
    if (channel.positions.size() == 1) {
        return channel.positions[0].position;
    }

    const int index = _getPositionIndex(channel, animationTime);
    const int nextIndex = std::min(index + 1, static_cast<int>(channel.positions.size() - 1));
    const auto &current = channel.positions[index];
    const auto &next = channel.positions[nextIndex];
    const float delta = next.timeStamp - current.timeStamp;
    const float factor = delta > 0.0f ? (animationTime - current.timeStamp) / delta : 0.0f;
    return glm::mix(current.position, next.position, factor);
}

glm::vec3 cbit::Animator::_interpolateScale(const AnimationChannel &channel, const float animationTime) const {
    if (channel.scales.empty()) {
        return glm::vec3(1.0f);
    }
    if (channel.scales.size() == 1) {
        const glm::vec3 baseScale = channel.baseScale;
        return glm::vec3(
            baseScale.x != 0.0f ? channel.scales[0].scale.x / baseScale.x : channel.scales[0].scale.x,
            baseScale.y != 0.0f ? channel.scales[0].scale.y / baseScale.y : channel.scales[0].scale.y,
            baseScale.z != 0.0f ? channel.scales[0].scale.z / baseScale.z : channel.scales[0].scale.z);
    }

    const int index = _getScaleIndex(channel, animationTime);
    const int nextIndex = std::min(index + 1, static_cast<int>(channel.scales.size() - 1));
    const auto &current = channel.scales[index];
    const auto &next = channel.scales[nextIndex];
    const float delta = next.timeStamp - current.timeStamp;
    const float factor = delta > 0.0f ? (animationTime - current.timeStamp) / delta : 0.0f;
    const glm::vec3 interpolated = glm::mix(current.scale, next.scale, factor);
    const glm::vec3 baseScale = channel.baseScale;
    return glm::vec3(
        baseScale.x != 0.0f ? interpolated.x / baseScale.x : interpolated.x,
        baseScale.y != 0.0f ? interpolated.y / baseScale.y : interpolated.y,
        baseScale.z != 0.0f ? interpolated.z / baseScale.z : interpolated.z);
}

glm::quat cbit::Animator::_interpolateRotation(const AnimationChannel &channel, const float animationTime) const {
    if (channel.rotations.empty()) {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }
    if (channel.rotations.size() == 1) {
        return glm::normalize(channel.rotations[0].rotation);
    }

    const int index = _getRotationIndex(channel, animationTime);
    const int nextIndex = std::min(index + 1, static_cast<int>(channel.rotations.size() - 1));
    const auto &current = channel.rotations[index];
    const auto &next = channel.rotations[nextIndex];
    const float delta = next.timeStamp - current.timeStamp;
    const float factor = delta > 0.0f ? (animationTime - current.timeStamp) / delta : 0.0f;
    return glm::normalize(glm::slerp(current.rotation, next.rotation, factor));
}

int cbit::Animator::_getPositionIndex(const AnimationChannel &channel, const float animationTime) const {
    for (size_t i = 0; i + 1 < channel.positions.size(); ++i) {
        if (animationTime < channel.positions[i + 1].timeStamp) {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(channel.positions.size() - 1);
}

int cbit::Animator::_getRotationIndex(const AnimationChannel &channel, const float animationTime) const {
    for (size_t i = 0; i + 1 < channel.rotations.size(); ++i) {
        if (animationTime < channel.rotations[i + 1].timeStamp) {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(channel.rotations.size() - 1);
}

int cbit::Animator::_getScaleIndex(const AnimationChannel &channel, const float animationTime) const {
    for (size_t i = 0; i + 1 < channel.scales.size(); ++i) {
        if (animationTime < channel.scales[i + 1].timeStamp) {
            return static_cast<int>(i);
        }
    }
    return static_cast<int>(channel.scales.size() - 1);
}
