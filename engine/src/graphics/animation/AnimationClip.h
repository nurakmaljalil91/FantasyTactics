/**
 * @file    AnimationClip.h
 * @brief   Animation clip data for skeletal animations.
 * @details Loads animation channels from Assimp-supported files.
 * @author  Nur Akmal bin Jalil
 * @date    2026-01-01
 */

#ifndef CBIT_ANIMATIONCLIP_H
#define CBIT_ANIMATIONCLIP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace cbit {
    struct KeyPosition {
        glm::vec3 position{0.0f};
        float timeStamp = 0.0f;
    };

    struct KeyRotation {
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        float timeStamp = 0.0f;
    };

    struct KeyScale {
        glm::vec3 scale{1.0f};
        float timeStamp = 0.0f;
    };

    struct AnimationChannel {
        std::vector<KeyPosition> positions;
        std::vector<KeyRotation> rotations;
        std::vector<KeyScale> scales;
        glm::vec3 baseScale{1.0f};
    };

    class AnimationClip {
    public:
        AnimationClip() = default;
        AnimationClip(const std::string &name, const std::string &path);

        bool loadFromFile(const std::string &path, const std::string &nameOverride = "");

        [[nodiscard]] const std::string &getName() const { return _name; }
        [[nodiscard]] float getDuration() const { return _duration; }
        [[nodiscard]] float getTicksPerSecond() const { return _ticksPerSecond; }
        [[nodiscard]] const std::unordered_map<std::string, AnimationChannel> &getChannels() const {
            return _channels;
        }

    private:
        std::string _name;
        float _duration = 0.0f;
        float _ticksPerSecond = 25.0f;
        std::unordered_map<std::string, AnimationChannel> _channels;
    };
}

#endif //CBIT_ANIMATIONCLIP_H
