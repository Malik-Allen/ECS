#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"

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

		EntityManager() : m_entityCounter( 0 ) {}

		~EntityManager() { DestroyAllEntities(); }

		// Creates an Entity, returns an EntityId, returns 0 if the entity was not created
		EntityId CreateEntity()
		{
			if (m_entityCounter > MAX_ENTITIES) {
				// Some Debug
				return 0;
			}

			Entity* entity = new Entity();

			if ( entity == nullptr )
			{
				// Some Debug
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
				// Some debug
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
			Entity* entity = nullptr;
			for (uint64_t i = 0; i < m_entityCounter; i++)
			{
				entity = m_entities[i];

				// Entity does not exist, returning
				if (entity == nullptr)
				{
					// Some debug
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