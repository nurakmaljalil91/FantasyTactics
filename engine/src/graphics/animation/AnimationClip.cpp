/**
 * @file    AnimationClip.cpp
 * @brief   Implementation of AnimationClip for loading skeletal animation data.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#include "AnimationClip.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "utilities/Logger.h"

cbit::AnimationClip::AnimationClip(const std::string &name, const std::string &path) {
    loadFromFile(path, name);
}

bool cbit::AnimationClip::loadFromFile(const std::string &path, const std::string &nameOverride) {
    Assimp::Importer importer;
    const unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;

    const aiScene *scene = importer.ReadFile(path, flags);
    if (!scene || scene->mNumAnimations == 0) {
        Logger::log()->error("Assimp failed to load animation {}: {}", path, importer.GetErrorString());
        return false;
    }

    const aiAnimation *animation = scene->mAnimations[0];
    _duration = static_cast<float>(animation->mDuration);
    _ticksPerSecond = animation->mTicksPerSecond != 0.0 ? static_cast<float>(animation->mTicksPerSecond) : 25.0f;
    _name = nameOverride.empty() ? animation->mName.C_Str() : nameOverride;

    _channels.clear();
    _channels.reserve(animation->mNumChannels);

    for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
        const aiNodeAnim *channel = animation->mChannels[i];
        const std::string channelName = channel->mNodeName.C_Str();
        AnimationChannel animChannel;

        animChannel.positions.reserve(channel->mNumPositionKeys);
        for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j) {
            const auto &key = channel->mPositionKeys[j];
            KeyPosition keyPos;
            keyPos.position = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
            keyPos.timeStamp = static_cast<float>(key.mTime);
            animChannel.positions.push_back(keyPos);
        }

        animChannel.rotations.reserve(channel->mNumRotationKeys);
        for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j) {
            const auto &key = channel->mRotationKeys[j];
            KeyRotation keyRot;
            keyRot.rotation = glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
            keyRot.timeStamp = static_cast<float>(key.mTime);
            animChannel.rotations.push_back(keyRot);
        }

        animChannel.scales.reserve(channel->mNumScalingKeys);
        for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j) {
            const auto &key = channel->mScalingKeys[j];
            KeyScale keyScale;
            keyScale.scale = glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
            keyScale.timeStamp = static_cast<float>(key.mTime);
            animChannel.scales.push_back(keyScale);
        }

        if (!animChannel.scales.empty()) {
            animChannel.baseScale = animChannel.scales.front().scale;
        }

        _channels[channelName] = std::move(animChannel);
    }

    return true;
}
