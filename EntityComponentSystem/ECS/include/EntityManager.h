#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include "Utility/Debug.h"

#include <map>

namespace ECS
{

	class EntityManager
	{
		template<typename ... T >
		friend struct Parser;

		friend class ComponentManager;

		// The entities created by this Entity Manager
		std::map<EntityId, Entity*>    m_entities;

		// The number of entities in this entity manager
		uint64_t					m_entityCounter;

		// TODO: 
		// Make an list of Entities that are pending a clean up, Create a System to clear out these Entities on a schedule
			// Instead of deleting them right away

		

	public:

		EntityManager() : m_entityCounter( 0 ) 
		{
			DEBUG_LOG( LOG::INFO, "Entity Manager created... " );
			CONSOLE_LOG( LOG::INFO, "Entity Manager created... " );
		}

		~EntityManager() { DestroyAllEntities(); }

		// Creates an Entity, returns an EntityId, returns 0 if the entity was not created
		EntityId CreateEntity()
		{

			/* '>' check works for entity counter, because the 0 entity id is reserved for an invalid entity id, and an entity id is incremented as its assigned*/
			if (m_entityCounter > MAX_ENTITIES) {
				DEBUG_LOG( LOG::WARNING, "Failed to create entity: Max entity count reached" );
				CONSOLE_LOG( LOG::WARNING, "Failed to create entity: Max entity count reached" );
				return 0;
			}

			Entity* entity = new Entity();

			if ( entity == nullptr )
			{
				DEBUG_LOG( LOG::WARNING, "Failed to create entity: Entity is nullptr" );
				CONSOLE_LOG( LOG::WARNING, "Failed to create entity: Entity is nullptr" );
				return 0;
			}

			entity->m_entityId = ++m_entityCounter;
			m_entities[m_entityCounter] = entity;

			return m_entityCounter;

		}


		// Destroys the Entity with the passed entityId
		void DestroyEntity( EntityId entityId )
		{
			Entity* entity = m_entities[entityId];

			// Entity does not exist, returning
			if ( entity == nullptr )
			{
				DEBUG_LOG( LOG::WARNING, "Failed to destroy entity: Entity Id: " + std::to_string(entityId) + ", entity does not exist");
				CONSOLE_LOG( LOG::WARNING, "Failed to destroy entity: Entity Id: " + std::to_string( entityId ) + ", entity does not exist" );
				return;
			}

			m_entities.erase( entityId );

			delete entity, entity = nullptr;

			--m_entityCounter;

		}

	private:

		// Warning, does not destroy all components on the entities
		void DestroyAllEntities()
		{
			DEBUG_LOG( LOG::INFO, "Destroying all entities... " );
			CONSOLE_LOG( LOG::INFO, "Destroying all entities... " );
			Entity* entity = nullptr;
			for (uint64_t i = 0; i < m_entityCounter; i++)
			{
				entity = m_entities[i];

				// Entity does not exist, returning
				if (entity == nullptr)
				{
					DEBUG_LOG( LOG::WARNING, "Failed to destroy entity: Entity Id: " + std::to_string( i ) + ", reached end of valid entities to destroy... ending destroy..." );
					CONSOLE_LOG( LOG::WARNING, "Failed to destroy entity: Entity Id: " + std::to_string( i ) + ", reached end of valid entities to destroy... ending destroy..." );
					return;
				}


				delete entity, entity = nullptr;
			}
			m_entities.clear();
			m_entityCounter = 0;

		}


	};

}


#endif // ENTITYMANAGER_H