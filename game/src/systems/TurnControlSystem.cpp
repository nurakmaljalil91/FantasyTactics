/**
 * @file    TurnControlSystem.cpp
 * @brief   Implementation of the TurnControlSystem class.
 * @details Selects which unit can currently respond to movement input.
 * @author  Nur Akmal bin Jalil
 * @date    2026-03-29
 */

#include "TurnControlSystem.h"

#include "core/Input.h"
#include "ecs/Components.h"

#include <algorithm>
#include <vector>

namespace {
    struct TurnEntry {
        entt::entity entity{entt::null};
        int slot = 0;
    };

    std::vector<TurnEntry> collectTurnEntries(entt::registry &registry) {
        const auto view = registry.view<TurnControlComponent>();
        std::vector<TurnEntry> entries;

        for (const auto entity: view) {
            const auto &turnControl = view.get<TurnControlComponent>(entity);
            entries.push_back(TurnEntry{entity, turnControl.slot});
        }

        std::sort(entries.begin(), entries.end(), [](const TurnEntry &lhs, const TurnEntry &rhs) {
            return lhs.slot < rhs.slot;
        });

        return entries;
    }

    int findActiveIndex(const std::vector<TurnEntry> &entries, entt::registry &registry) {
        for (int index = 0; index < static_cast<int>(entries.size()); ++index) {
            const auto &turnControl = registry.get<TurnControlComponent>(entries[index].entity);
            if (turnControl.canMove) {
                return index;
            }
        }

        return entries.empty() ? -1 : 0;
    }

    void setActiveIndex(
        entt::registry &registry,
        const std::vector<TurnEntry> &entries,
        const int activeIndex) {
        if (entries.empty()) {
            return;
        }

        const auto view = registry.view<TurnControlComponent>();
        for (const auto entity: view) {
            auto &turnControl = view.get<TurnControlComponent>(entity);
            turnControl.canMove = false;
        }

        registry.get<TurnControlComponent>(entries[activeIndex].entity).canMove = true;
    }

    std::string getActiveUnitLabel(entt::registry &registry, const std::vector<TurnEntry> &entries) {
        if (entries.empty()) {
            return "None";
        }

        const int activeIndex = findActiveIndex(entries, registry);
        if (activeIndex < 0) {
            return "None";
        }

        const auto &turnControl = registry.get<TurnControlComponent>(entries[activeIndex].entity);
        if (!turnControl.displayName.empty()) {
            return turnControl.displayName;
        }

        if (const auto *tag = registry.try_get<cbit::TagComponent>(entries[activeIndex].entity)) {
            return tag->tag;
        }

        return "Unknown";
    }

    void updateTurnStatusText(entt::registry &registry, const std::string &activeUnitLabel) {
        const auto textView = registry.view<TurnStatusTextComponent, cbit::UITextComponent>();
        for (const auto entity: textView) {
            const auto &status = textView.get<TurnStatusTextComponent>(entity);
            auto &text = textView.get<cbit::UITextComponent>(entity);
            text.text = status.label + activeUnitLabel;
        }
    }
}

void TurnControlSystem::update(entt::registry &registry, float deltaTime) {
    (void)deltaTime;

    auto entries = collectTurnEntries(registry);
    if (entries.empty()) {
        updateTurnStatusText(registry, "None");
        return;
    }

    int activeIndex = findActiveIndex(entries, registry);
    if (activeIndex < 0) {
        activeIndex = 0;
        setActiveIndex(registry, entries, activeIndex);
    }

    if (cbit::Input::isKeyPressed(cbit::Keyboard::H)) {
        activeIndex = (activeIndex - 1 + static_cast<int>(entries.size())) % static_cast<int>(entries.size());
        setActiveIndex(registry, entries, activeIndex);
    }

    if (cbit::Input::isKeyPressed(cbit::Keyboard::L)) {
        activeIndex = (activeIndex + 1) % static_cast<int>(entries.size());
        setActiveIndex(registry, entries, activeIndex);
    }

    updateTurnStatusText(registry, getActiveUnitLabel(registry, entries));
}
