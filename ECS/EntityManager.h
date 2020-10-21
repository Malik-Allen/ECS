#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "IEntity.h"
#include "ComponentManager.h"

#include <string>
#include <map>

namespace ECS {

    // The Entity Manager Contains a map of Entities and a reference to a Component Manager
    class EntityManager {


        // ComponentManager that is assigned to any Entity created with this manager
        ComponentManager*               m_componentManager;

        // TODO: Make this a hash_map for speed!
        // Map of Entities, where the Entity Id is the key and the Entity pointer as the value
        std::map<EntityId, IEntity*>    m_entities;

        // TODO: Make an linked-listed of Entities that are pending a clean up, Create a System to clear out these Entities on a schedule

        // Always incremented, never decremented
        uint32_t                        m_entityCounter;

    public:

        EntityManager() : 
            m_componentManager(nullptr), m_entityCounter(0) {}
        
        ~EntityManager() { DestroyAllEntities(); }

        void AssignComponentManager( ComponentManager* componentManager ) { m_componentManager = componentManager; }


        // Creates an Entity of the passed Entity SubClass, returns an EntityId
        template<typename T>
        EntityId CreateEntity() 
        {
            
            IEntity* entity = dynamic_cast<IEntity*>(new T());

            if (!entity)
                return 0;

            entity->m_entityId = ++m_entityCounter;
            entity->m_componentManager = m_componentManager;

            m_entities[m_entityCounter] =  entity;

            return m_entityCounter;

        }


        // Destroys the Entity with the passed EntityId
        void DestroyEntity(EntityId entityId) 
        {
            
            IEntity* entity = GetEntity(entityId);      // Finds the Entity with this EntityId in the list of Entities

            if (!entity)
                return;

            entity->m_componentManager->RemoveAllComponents();  // Removes all Components from this Entity before deletion

            m_entities.erase(entityId);

            // TODO: Push this Entity to the linked-list of pending destroyed entities,  // Mass removal of all entities

            delete entity, entity = nullptr;

        }


        // Returns an Entity inside of this Entity Manager with the passed EntityId, return nullptr if Entity does not exist
        IEntity* GetEntity(EntityId entityId)
        {

            // TODO: make this like an array access instead of a find function, when this is a hash_map no need for find function
            if (m_entities.find(entityId) != m_entities.end()) {
                return m_entities[entityId];
            }
            return nullptr;

        }


        // Destroys all of the Entities in this EntityManager, removing all their components in the process
        bool DestroyAllEntities() 
        {

            for (auto entity : m_entities) {

                entity.second->m_componentManager->RemoveAllComponents();

                delete entity.second, entity.second = nullptr;
            }

            return m_entities.empty();

        }


    };

}


#endif // ENTITYMANAGER_H