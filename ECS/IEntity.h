#ifndef IENTITY_H
#define IENTITY_H



#include <string>

namespace ECS {

    typedef std::string EntityId;

    class ComponentManager;

    // Entity , contains a component manager, an id and an active state
    class IEntity {

        friend class EntityManager;
        

    protected:

        // Component manager, assigned on create in entity manager and manages the components of this entity
        ComponentManager*           m_componentManager;    

        // Id that represents this entity within its container
        EntityId                    m_entityId;                          

        // State of this entity, is active or not  
        bool                        m_isActive;                              

    public:

        IEntity() :
            m_componentManager(nullptr),
            m_entityId(""),
            m_isActive(true)
        {}

        virtual ~IEntity() {}

        // Adds a component to this entity, returns the component on success, returns nullptr if fails
        template<typename T>
        T* AddComponent() 
        {

            // Component Manager's Add Component
            return this->m_componentManager->AddComponent<T>(m_entityId);

        }

        // Removes the component with the component Id
        template<typename T>
        void RemoveComponent(EntityId componentId) 
        {

            this->m_componentManager->RemoveComponent<T>(componentId);

        }


        // Sets the active state of this entity
        void SetEnabled(bool isActive) 
        { 

            if (isActive != m_isActive) {
                isActive ? OnEnabled() : OnDisabled();    // Current implementation have the fucntionality of onEnable and onDisable happen for before the active status has changed  
            }

            m_isActive = isActive; 
            
        }


        // Returns the active state of this entity
        bool IsEnabled() const { return m_isActive; }


        // Called when this entity is enabled
        virtual void OnEnabled() = 0;

        // Called when this entity is deisabled
        virtual void OnDisabled() = 0;

        auto GetComponents() const { return m_componentManager->GetComponents();}


        // TODO:    IMPLEMENTATION

        // Comparsion Operators
            // In order to compare two entities based on thei entity ids

    };

}



#endif // IENTITY_H