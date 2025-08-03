/**
 * @file    EntityComponentSystem.h
 * @brief   Header file for the EntityComponentSystem class
 * @details This file contains the definition of the EntityComponentSystem class.
 * @author  Nur Akmal bin Jalil
 * @date    2025-08-03
 */

#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H
#include "UISystem.h"
#include "entt/entt.hpp"
#include "utilities/Logger.h"

class GameObject;

/**
 * @class EntityComponentSystem
 * @brief Represents the entity-component system for managing game objects and their components.
 * @details The EntityComponentSystem class is responsible for managing entities and their associated components.
 *          It provides methods to create, destroy, and query game objects, as well as to add and retrieve components.
 *          The class uses the entt library for efficient entity management and component storage.
 */
class EntityComponentSystem {
public:
    /**
     * Default constructor.
     * @details Initializes an empty EntityComponentSystem with no entities or components.
     */
    EntityComponentSystem();

    /**
     * Default destructor.
     * @details Cleans up resources used by the EntityComponentSystem.
     */
    ~EntityComponentSystem();

    /**
     * Update the entity-component system.
     * @details This method is called to update the ECS, allowing for processing of game logic and component updates.
     * @param deltaTime Time since the last update in seconds.
     * @details This method is called to update the ECS, allowing for processing of game logic
     */
    void update(float deltaTime);

    /**
     * Render the entity-component system.
     * @details This method is called to render all game objects and their components.
     *          It should be called after the update method to ensure that all game objects are rendered correctly.
     */
    void render();

    /**
     * Cleanup the entity-component system.
     * @details This method is called to clean up resources used by the ECS, such as destroying entities and components.
     *          It should be called when the ECS is no longer needed, such as when the game is exiting or switching scenes.
     */
    void cleanup();

    /**
     * Create a new game object with a unique tag.
     * @param tag the tag for the game object to be created.
     * @return GameObject representing the newly created game object.
     * @details This method creates a new game object with the specified tag and returns a GameObject instance
     *          that represents the newly created game object. The tag is used to identify the game object
     *          and can be used for querying or managing the game object later.
     *          The method internally creates a new entity in the ECS and associates it with the specified
     *          tag. The created game object can then be used to add components, query for components,
     *          and perform operations on the entity.
     *          If a game object with the same tag already exists, it will return the existing
     */
    GameObject createGameObject(const std::string &tag);

    /**
     * Destroy a game object.
     * @details This method destroys the specified game object, removing it from the ECS and cleaning up its resources.
     * @param gameObject GameObject to be destroyed.
     */
    void destroyGameObject(GameObject gameObject);

    /**
     * Get a game object by its tag.
     * @param tag The tag of the game object to retrieve.
     * @return GameObject representing the game object with the specified tag.
     * @details This method retrieves a game object by its tag. If no game object with the specified tag exists,
     *          it will return an invalid GameObject instance.
     */
    GameObject getGameObject(const std::string &tag);

    /**
     * Get all game objects with specified components.
     * @tparam Components  The types of components to filter the game objects by.
     * @return entt::view containing all game objects with the specified components.
     */
    template<typename... Components>
    auto getAllGameObjects() {
        return _registry.view<Components...>();
    }

    /**
     * Get game objects with specified components.
     * @tparam Components  The types of components to filter the game objects by.
     * @return entt::view containing all game objects with the specified components.
     * @details This method retrieves a view of all game objects that have the specified components.
     *          It can be used to iterate over game objects that share common components.
     */
    template<typename... Components>
    auto getGameObjectsWith() {
        return _registry.view<Components...>();
    }

    /**
     * Check if a game object is valid.
     * @param entity The entity to check for validity.
     * @return true if the game object is valid, false otherwise.
     * @details This method checks if the specified entity is a valid game object in the ECS.
     *          An entity is considered valid if it exists in the registry and has not been destroyed.
     */
    [[nodiscard]] bool validGameObject(const entt::entity entity) const {
        return _registry.valid(entity);
    }

    /**
     * Check if an entity has a specific component.
     * @tparam Components The types of components to check for.
     * @param entity The entity to check for the components.
     * @return true if the entity has all specified components, false otherwise.
     * @details This method checks if the specified entity has all of the given components.
     *          It can be used to determine if an entity is suitable for certain operations based on its components.
     */
    template<typename... Components>
    [[nodiscard]] bool hasComponent(const entt::entity entity) const {
        return _registry.all_of<Components...>(entity);
    }

    /**
     * Get components of an entity.
     * @tparam Components The types of components to retrieve.
     * @param entity The entity to retrieve the components from.
     * @return A tuple containing the components of the specified types for the entity.
     * @details This method retrieves the components of the specified types for the given entity.
     *          It can be used to access multiple components at once, allowing for efficient retrieval of component data.
     */
    template<typename... Components>
    [[nodiscard]] auto getComponents(const entt::entity entity) {
        return _registry.get<Components...>(entity);
    }

    /**
     * Get a specific component of an entity.
     * @tparam Component The type of the component to retrieve.
     * @param entity The entity to retrieve the component from.
     * @return A reference to the component of the specified type for the entity.
     */
    template<typename Component>
    [[nodiscard]] auto &getComponent(const entt::entity entity) {
        return _registry.get<Component>(entity);
    }

    /**
     * Add a component to an entity.
     * @tparam T The type of the component to be added.
     * @param entity The entity to which the component will be added.
     * @param args Arguments to construct the component.
     * @return A reference to the newly added component.
     * @details This method adds a new component of type T to the specified entity.
     *          If the component already exists, it will return the existing component instead of adding a new one.
     */
    template<typename T, typename... Args>
    T &addComponent(const entt::entity entity, Args &&... args) {
        // Check if the component already exists
        if (hasComponent<T>(entity)) {
            Logger::log()->warn("Component already added");
            return _registry.get<T>(entity);
        }
        return _registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    /**
     * Get the registry of the entity-component system.
     * @return A reference to the entt::registry used by the ECS.
     */
    [[nodiscard]] entt::registry &getRegistry() { return _registry; }

private:
    entt::registry _registry;
    friend class GameObject;

    UISystem _uiSystem{_registry};
};


#endif //ENTITYCOMPONENTSYSTEM_H
