/**
 * @file    GameObject.h
 * @brief   GameObject class header file
 * @details This file contains the definition of the GameObject class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef CBIT_GAMEOBJECT_H
#define CBIT_GAMEOBJECT_H

#include <entt/entt.hpp>

#include "EntityComponentSystem.h"
#include "utilities/Logger.h"

namespace cbit {
    /**
     * @class GameObject
     * @brief Represents a game object in the entity-component system.
     * @details The GameObject class is a wrapper around an entity in the ECS (Entity-Component-System) framework.
     *          It provides a way to manage and interact with entities in the game engine.
     *          The class is designed to be used in conjunction with the EntityComponentSystem class,
     *          which manages the registry of entities and their components.
     *          The GameObject class can be used to create, destroy, and manipulate entities in the game world.
     *          It can also be extended to include methods for adding and removing components, querying
     *          components, and performing operations on the entity.
     *          The class is designed to be used in a game engine or graphics application that uses OpenGL or other graphics libraries for rendering.
     *          The GameObject class can be integrated with other components such as EntityComponentSystem, SceneManager, and Renderer to create a complete game object management system.
     */
    class GameObject {
    public:
        /**
         * Default constructor.
         * @details Initializes an empty GameObject with no associated entity or system.
         */
        GameObject() = default;

        /**
         * Constructor that initializes the GameObject with an entity and a system.
         * @param entity The entity associated with this GameObject.
         * @param system Pointer to the EntityComponentSystem managing this GameObject.
         * @details This constructor sets the entity and system for the GameObject, allowing it to interact with the ECS.
         */
        GameObject(const entt::entity entity, EntityComponentSystem *system) : _entity(entity), _system(system) {
        }

        /**
         * Default destructor.
         * @details Cleans up resources associated with the GameObject.
         */
        ~GameObject() = default;


        /**
         * Add a component to the GameObject.
         * @tparam T The type of the component to be added.
         * @tparam Args Args The types of the arguments to be passed to the component's constructor.
         * @return A reference to the GameObject itself, allowing for method chaining.
         */
        template<typename T, typename... Args>
        GameObject &addComponent(Args &&... args) {
            // Check if the component already exists
            if (hasComponent<T>()) {
                Logger::log()->warn("Component already added");
            } else {
                _system->_registry.emplace<T>(_entity, std::forward<Args>(args)...);
            }
            return *this;
        }

        /**
         * Get a component from the GameObject.
         * @tparam T The type of the component to be retrieved.
         * @return A reference to the component.
         * @details This method retrieves the component of type T from the GameObject's entity.
         *          If the component does not exist, it will throw an exception.
         */
        template<typename T>
        T &getComponent() {
            return _system->_registry.get<T>(_entity);
        }

        /**
         * Check if the GameObject has a component of type T.
         * @tparam T The type of the component to check for.
         * @return true if the component exists, false otherwise.
         * @details This method checks if the GameObject's entity has a component of type T.
         */
        template<typename T>
        bool hasComponent() {
            return _system->_registry.any_of<T>(_entity);
        }

        /**
         * Remove a component from the GameObject.
         * @tparam T The type of the component to be removed.
         * @details This method removes the component of type T from the GameObject's entity.
         *          If the component does not exist, it will do nothing.
         */
        template<typename T>
        void removeComponent() {
            _system->_registry.remove<T>(_entity);
        }

        /**
         * Destroy the GameObject.
         * @details This method destroys the GameObject's entity in the ECS, effectively removing it from the game world.
         *          After calling this method, the GameObject should not be used anymore.
         */
        void destroy() {
            _system->_registry.destroy(_entity);
        }

        /**
         * Get the entity associated with this GameObject.
         * @return The entt::entity representing this GameObject.
         * @details This method returns the entity associated with the GameObject, allowing access to its components and properties.
         */
        [[nodiscard]] entt::entity getEntity() const {
            return _entity;
        }

        /**
         * Set the entity for this GameObject.
         * @param entity The entt::entity to be associated with this GameObject.
         * @details This method sets the entity for the GameObject, allowing it to interact with the ECS.
         */
        // Note: This should be used with caution, as it can lead to inconsistencies if the entity is already managed by the system.
        // It is generally better to create a new GameObject with the desired entity and system.
        // This method is provided for advanced use cases where the entity needs to be changed dynamically.
        // Use with caution!
        void setEntity(entt::entity entity) {
            _entity = entity;
        }

        /**
         * Set the EntityComponentSystem for this GameObject.
         * @param system Pointer to the EntityComponentSystem managing this GameObject.
         * @details This method sets the system for the GameObject, allowing it to interact with the ECS.
         */
        void setSystem(EntityComponentSystem *system) {
            _system = system;
        }

        /**
         * Get the EntityComponentSystem managing this GameObject.
         * @return Pointer to the EntityComponentSystem managing this GameObject.
         * @details This method returns the system associated with the GameObject, allowing access to the ECS registry and operations.
         */
        [[nodiscard]] EntityComponentSystem *getSystem() const {
            return _system;
        }

        /**
         * Equality operator for comparing two GameObjects.
         * @param other The other GameObject to compare with.
         * @return true if both GameObjects have the same entity, false otherwise.
         * @details This operator checks if two GameObjects are equal by comparing their associated entities.
         */
        bool operator==(const GameObject &other) const {
            return _entity == other._entity;
        }

    private:
        entt::entity _entity{entt::null};
        EntityComponentSystem *_system = nullptr;
    };
}

#endif //CBIT_GAMEOBJECT_H
