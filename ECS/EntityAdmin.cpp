#include "EntityAdmin.h"



void ECS::EntityAdmin::Update( float deltaTime )
{
	for ( ISystem* s : m_systems )
	{
		s->Update( deltaTime );
	}
}

void ECS::EntityAdmin::OnCreate()
{
	m_components = new std::vector<Component*>();
}

void ECS::EntityAdmin::OnDestroy()
{
}

ECS::EntityId ECS::EntityAdmin::CreateEntity()
{

	Entity* entity = new Entity();

	if ( !entity )
	{
		// Some Debug
		return 0;
	}

	entity->m_entityId = ++m_entityCounter;

	m_entities[entity->m_entityId] = entity;


	


	return m_entityCounter;

}

void ECS::EntityAdmin::DestroyEntity(EntityId entityId)
{
	Entity* entityToDestroy = nullptr;
	if ( m_entities[entityId] != nullptr )
	{
		entityToDestroy = m_entities[entityId];

		m_entities[entityId] = m_entities[m_entityCounter];	// Set the emptied location to the last index of this map

		m_entities[m_entityCounter] = nullptr;

		--m_entityCounter;

		if ( entityToDestroy )
		{
			// Destroy all its components along with it
			delete entityToDestroy, entityToDestroy = nullptr;
		}
	}
	else
	{
		// Some Debug Message
	}

}

void ECS::EntityAdmin::DestroyAllEntities()
{

	for ( uint64_t i = 0; i < m_entityCounter; ++i )
	{
		if ( m_entities[i] != nullptr )
		{
			delete m_entities[i], m_entities[i] = nullptr;
		}
	}

	m_entities.clear();

}

void ECS::EntityAdmin::RemoveComponent( EntityId entity, ComponentId componentId )
{
}
