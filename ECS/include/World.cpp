#include "World.h"

namespace ECS
{

	World::World() :
		m_enityManager(new ECS::EntityManager()),
		m_systemManager(new ECS::SystemManager()),
		m_componentManager(new ECS::ComponentManager(m_enityManager, m_systemManager))
	{}

	World::~World()
	{
		// Each Manager will handle the destruction of their items

		// Systems get deleted first, so when we remove components, they no longer
		if (m_systemManager)
		{
			delete m_systemManager;
			m_systemManager = nullptr;
		}

		// Now we remove all components from the component manager
		if (m_componentManager)
		{
			delete m_componentManager;
			m_componentManager = nullptr;
		}

		// Finally clear out all entities
		if (m_enityManager)
		{
			delete m_enityManager;
			m_enityManager = nullptr;
		}
	}


}