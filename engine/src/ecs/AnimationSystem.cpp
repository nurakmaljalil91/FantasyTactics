/**
 * @file    AnimationSystem.cpp
 * @brief   Animation system implementation.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#include "AnimationSystem.h"

#include "Components.h"
#include "utilities/Logger.h"
#include "glm/glm.hpp"

void cbit::AnimationSystem::update(entt::registry &registry, const float deltaTime) {
    const auto view = registry.view<SkinnedMeshComponent, AnimatorComponent>();

    for (const auto entity: view) {
        const auto &[skinnedMesh, animatorComponent] = view.get<SkinnedMeshComponent, AnimatorComponent>(entity);

        if (animatorComponent.activeClip.empty()) {
            continue;
        }

        if (animatorComponent.previousClip != animatorComponent.activeClip) {
            animatorComponent.animator.reset();
            animatorComponent.previousClip = animatorComponent.activeClip;
            if (!animatorComponent.activeClip.empty()) {
                const auto logClipIt = animatorComponent.clips.find(animatorComponent.activeClip);
                if (logClipIt != animatorComponent.clips.end()) {
                    const auto &clip = logClipIt->second;
                    Logger::log()->info("Animation clip '{}' duration={} tps={} channels={}",
                                        clip.getName(),
                                        clip.getDuration(),
                                        clip.getTicksPerSecond(),
                                        clip.getChannels().size());
                    if (!clip.getChannels().empty()) {
                        const auto &entry = *clip.getChannels().begin();
                        const auto &channel = entry.second;
                        const float posStart = channel.positions.empty() ? 0.0f : channel.positions.front().timeStamp;
                        const float posEnd = channel.positions.empty() ? 0.0f : channel.positions.back().timeStamp;
                        const float rotStart = channel.rotations.empty() ? 0.0f : channel.rotations.front().timeStamp;
                        const float rotEnd = channel.rotations.empty() ? 0.0f : channel.rotations.back().timeStamp;
                        const float scaleStart = channel.scales.empty() ? 0.0f : channel.scales.front().timeStamp;
                        const float scaleEnd = channel.scales.empty() ? 0.0f : channel.scales.back().timeStamp;
                        Logger::log()->info(
                            "Channel '{}' keys pos={} rot={} scale={} posTime=[{},{}] rotTime=[{},{}] scaleTime=[{},{}]",
                            entry.first,
                            channel.positions.size(),
                            channel.rotations.size(),
                            channel.scales.size(),
                            posStart, posEnd,
                            rotStart, rotEnd,
                            scaleStart, scaleEnd);
                    }

                    const auto &boneMap = skinnedMesh.mesh.getBoneMap();
                    Logger::log()->info("Skinned mesh bones count={}", boneMap.size());
                    int bonePrinted = 0;
                    for (const auto &pair: boneMap) {
                        Logger::log()->info("Bone name[{}]: {}", bonePrinted, pair.first);
                        if (++bonePrinted >= 10) {
                            break;
                        }
                    }

                    int channelPrinted = 0;
                    for (const auto &pair: clip.getChannels()) {
                        Logger::log()->info("Channel name[{}]: {}", channelPrinted, pair.first);
                        if (++channelPrinted >= 10) {
                            break;
                        }
                    }

                    for (const auto &pair: clip.getChannels()) {
                        const auto &channel = pair.second;
                        if (channel.rotations.size() >= 2) {
                            const auto &first = channel.rotations.front().rotation;
                            const auto &second = channel.rotations[1].rotation;
                            const float delta = glm::length(glm::vec4(first.x - second.x,
                                                                       first.y - second.y,
                                                                       first.z - second.z,
                                                                       first.w - second.w));
                            Logger::log()->info("Channel '{}' rotation delta between key0 and key1: {}",
                                                pair.first, delta);
                            break;
                        }
                    }

                    size_t movingChannels = 0;
                    for (const auto &pair: clip.getChannels()) {
                        const auto &channel = pair.second;
                        float positionDelta = 0.0f;
                        float rotationDelta = 0.0f;
                        float scaleDelta = 0.0f;
                        if (channel.positions.size() >= 2) {
                            const auto &first = channel.positions.front().position;
                            const auto &second = channel.positions[1].position;
                            positionDelta = glm::length(second - first);
                        }
                        if (channel.rotations.size() >= 2) {
                            const auto &first = channel.rotations.front().rotation;
                            const auto &second = channel.rotations[1].rotation;
                            rotationDelta = glm::length(glm::vec4(first.x - second.x,
                                                                  first.y - second.y,
                                                                  first.z - second.z,
                                                                  first.w - second.w));
                        }
                        if (channel.scales.size() >= 2) {
                            const auto &first = channel.scales.front().scale;
                            const auto &second = channel.scales[1].scale;
                            scaleDelta = glm::length(second - first);
                        }

                        if (positionDelta > 0.0001f || rotationDelta > 0.0001f || scaleDelta > 0.0001f) {
                            movingChannels++;
                        }
                    }

                    Logger::log()->info("Animation channels with non-zero delta in first two keys: {}/{}",
                                        movingChannels, clip.getChannels().size());

                    std::unordered_set<std::string> nodeNames;
                    nodeNames.reserve(skinnedMesh.mesh.getSkeletonNodes().size());
                    for (const auto &node: skinnedMesh.mesh.getSkeletonNodes()) {
                        nodeNames.insert(node.name);
                    }

                    size_t matchedChannels = 0;
                    for (const auto &pair: clip.getChannels()) {
                        if (nodeNames.find(pair.first) != nodeNames.end()) {
                            matchedChannels++;
                        }
                    }
                    Logger::log()->info("Animation channels matched to skeleton nodes: {}/{}",
                                        matchedChannels, clip.getChannels().size());
                }
            }
        }

        const auto clipIt = animatorComponent.clips.find(animatorComponent.activeClip);
        if (clipIt == animatorComponent.clips.end()) {
            Logger::log()->warn("Missing animation clip '{}' on entity.", animatorComponent.activeClip);
            continue;
        }

        animatorComponent.animator.update(
            &clipIt->second,
            skinnedMesh.mesh,
            deltaTime,
            animatorComponent.loop,
            animatorComponent.playbackSpeed);
    }
}
