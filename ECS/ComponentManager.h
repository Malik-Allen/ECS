#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "IComponent.h"

#include <string>
#include <map>

namespace ECS {

    // Manager for all Components Created by this Manager
    class ComponentManager {

        class EntityManager*                m_entityManager;

        // TODO: Think about what would be most efficient container for the Component Pointers
        std::map<ComponentId, IComponent*>   m_components;

    public:

        ComponentManager() : m_entityManager(nullptr) {}

        ~ComponentManager() { RemoveAllComponents(); }


        // Adds a Component this Component Manager, assigning its Owning EntityId to the EntityId passed
        template<typename T>
        T* AddComponent(EntityId entityId) 
        {

            IComponent* component = dynamic_cast<IComponent*>(new T());     // Dynamic cast will return nullptr, if the passed type is not a child of the Component Base Class

            if (!component)
                return nullptr;

            component->m_ownerId = entityId;

            // TODO: Component Id will be the hash value for the component
            ComponentId componentId = entityId + "- Component - " + std::to_string(m_components.size() + 1);
            component->m_componentId = componentId;

            m_components[componentId] = component;

            return dynamic_cast<T*>(component);

        }


        // Removes Component with the passed ComponentId
        void RemoveComponent(ComponentId componentId)
        {

            // Find the component in the map of components

            IComponent* component = GetComponent(componentId);

            if (!component)
                return;

            delete component, component = nullptr;

            m_components.erase(componentId);

        }



        // Clears out all components on this component manager
        bool RemoveAllComponents()
        {

            for (auto component : m_components) {

                delete component.second, component.second = nullptr;    // Deleting the pointer

            }

            return m_components.empty();

        }


        // Returns the Component with the Component Id inside this Component Manager
        IComponent* GetComponent(ComponentId componentId) 
        {
            // TODO: Change to hash_map
            if (m_components.find(componentId) != m_components.end()) {
                return m_components[componentId];
            }
            return nullptr;

        }

        auto GetComponents() const
        {
            return m_components;
        }

    };

}


#endif // COMPONENTMANAGER_H

