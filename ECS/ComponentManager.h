#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "IComponent.h"

#include "SystemManager.h"

#include <string>
#include <map>
#include <tuple>
#include <vector>

namespace ECS {



    // Manager for all Components Created by this Manager
    class ComponentManager {

        using ComponentMap = std::map< ComponentType, std::map<EntityId, std::map<ComponentId, IComponent*> > >;

        ComponentMap                        m_components;

        // Always incremented, never decremented
        uint32_t                            m_componentCounter;

        SystemManager*                      m_systemManager;

    public:

        ComponentManager() : m_componentCounter(0), m_systemManager(nullptr) {}

        ~ComponentManager() {}

        void AssignSystemManager( SystemManager* systemManager ) { m_systemManager = systemManager; }


        // Adds a Component this Component Manager, assigning its Owning EntityId to the EntityId passed
        template<typename T>
        T* AddComponent(EntityId entityId) 
        {

            IComponent* component = dynamic_cast<IComponent*>(new T());     // Dynamic cast will return nullptr, if the passed type is not a child of the Component Base Class

            if (!component)
                return nullptr;


            component->m_ownerId = entityId;
            component->m_componentId = ++m_componentCounter;

            
            m_components[component->m_type][entityId][component->m_componentId] = component;

            m_systemManager->OnComponentAdded( component );
           
            return dynamic_cast<T*>(component);

        }


        // Removes Component with the passed ComponentId
        template<typename T>
        void RemoveComponent(EntityId entityId, ComponentId componentId)
        {

            // Find the component in the map of components

            IComponent* component = GetComponent<T>(entityId, componentId);

            if (!component)
                return;

            m_components[component->m_type][entityId].erase( componentId );

            delete component, component = nullptr;

        }



        // Clears out all components on this component manager
        bool RemoveAllComponents()
        {
            IComponent* temp = nullptr;
            for ( auto component : m_components )
            {

                for ( auto comp : component.second )
                {

                    for ( auto c : comp.second )
                    {

                        if ( c.second != nullptr)
                        {

                            temp = m_components[component.first][comp.first][c.first];
                            delete c.second;
                            c.second = nullptr;

                        }

                    }

                    comp.second.clear();
                    
                }

                component.second.clear();

            }



            return m_components.empty();

        }


        // Returns the Component with the Component Id inside this Component Manager
        template<typename T>
        IComponent* GetComponent( EntityId entityId, ComponentId componentId )
        {

            // First create a temporary Icomponent object using this type
            IComponent* temp = dynamic_cast<IComponent*>(new T());

            if ( !temp )
                return nullptr;

            
            
            if (m_components.find( temp->m_type ) != m_components.end()) {

                if ( m_components[temp->m_type].find( entityId ) != m_components[temp->m_type].end() )
                {
                    if ( m_components[temp->m_type][entityId].find( componentId ) != m_components[temp->m_type][entityId].end() )
                    {
                        return m_components[temp->m_type][entityId][componentId];
                    }

                }
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

