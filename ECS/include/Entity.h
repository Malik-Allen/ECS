#ifndef ENTITY_H
#define ENTITY_H

#include "ECS_Definitions.h"

#include <array>

namespace ECS
{

	struct Entity
	{
		
		Entity() : m_entityId( 0 ), m_componentCounter( 0 ), m_components() {}
		~Entity() {}	

		const EntityId& GetId() const { return m_entityId; }
		const uint64_t& GetComponentCount() const { return m_componentCounter; }
		const std::array<class Component*, MAX_COMPONENTS_PER_ENTITY>& GetComponents() const { return m_components; }

	private:

		friend class EntityManager;
		friend class ComponentManager;

		// Unique identifier for this entity
		EntityId			m_entityId;

		// Number of components on this entity
		uint64_t			m_componentCounter;


		std::array<Component*, MAX_COMPONENTS_PER_ENTITY> m_components;

	};

}


#endif // ENTITY_H