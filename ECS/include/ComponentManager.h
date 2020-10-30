#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "Utility/TemplateHelper.h"
#include "Component.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include <array>
#include <vector>
#include <map>

namespace ECS
{

	// Manager for all Components Created by this Manager
	class ComponentManager
	{
		std::array<Component*, MAX_COMPONENTS> m_components;
		std::vector<Component*> m_trash;

		uint64_t m_componentCounter;

		EntityManager* m_entityManager;

		using ComponentMap = std::map<uint64_t, std::vector<Component*>>;

		ComponentMap m_componentMap;

		SystemManager* m_systemManager;


	public:

		ComponentManager(EntityManager* entityManager, SystemManager* systemManager) : 
			m_components(),
			m_componentCounter( 0 ), 
			m_entityManager(entityManager) ,
			m_systemManager( systemManager )
		{}

		~ComponentManager() {}


		// Add Component to Entity with the Id, returns created Component, returns fall if entity does not exist
		template<typename T, typename ... Args>
		T* AddComponent( EntityId entityId, Args&& ... args )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, Component>();

			if ( m_componentCounter == MAX_COMPONENTS )	// We are at capacity, return 
			{
				// Some Debug
				return nullptr;
			}

			Entity* entity = m_entityManager->m_entities[entityId];
			if ( entity == nullptr )	// Entity does not exist
			{
				// Some Debug
				return nullptr;
			}

			if ( entity->m_componentCounter == MAX_COMPONENTS_PER_ENTITY )	// This entity is at its capacity
			{
				// Some debug
				return nullptr;
			}

			// Component Classes can support different constructors, 0 -> n number of paramters in their constructor
			T* component = new T( std::forward<Args>( args ) ... );

			if ( component == nullptr )	// Could not create component
			{
				// Some Debug
				return nullptr;
			}

			component->m_ownerId = entityId;
			component->m_componentId = entity->m_componentCounter;
			++entity->m_componentCounter;

			m_components[m_componentCounter] = component;
			++m_componentCounter;

			entity->m_components[component->m_componentId] = component;

			m_componentMap[component->m_componentType].push_back( component );	// Also add a reference of this component to the ComponentMap

			m_systemManager->OnEntitySignatureChanged( *entity );


			return component;

		}


		// Removes Component with the passed ComponentId
		template<typename T>
		void RemoveComponent( EntityId entityId )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, Component>();

			Entity* entity = m_entityManager->m_entities[entityId];
			if ( entity == nullptr )	// Entity does not exist
			{
				// Some Debug
				return;
			}

			T* temp = new T();	// This is created to obtain a id from this component type

			if ( temp == nullptr )
			{
				// Some Debug
				return;
			}

			// Loop through the components of the same type
			Component* component = nullptr;
			for ( auto c = m_componentMap[temp->m_componentType].begin(); c != m_componentMap[temp->m_componentType].end(); c++ )
			{
				component = *c;

				if ( component->m_ownerId == entityId )		// When we find the first component with the same owner, we will remove it
				{

					m_componentMap[temp->m_componentType].erase( c );	// Erase it from the component map, so it can no longer be referenced

					ComponentId componentId = component->m_componentId;

					uint64_t lastComponentId = --m_entityManager->m_entities[entityId]->m_componentCounter;	// Save this for the swapping later


					// Assign the component index of the component we are about to delete to the last component on this entity
					m_entityManager->m_entities[entityId]->m_components[componentId] = m_entityManager->m_entities[entityId]->m_components[lastComponentId];

					// If this component is a valid component, then we give it a new component id
					if ( m_entityManager->m_entities[entityId]->m_components[componentId] != nullptr )
					{
						m_entityManager->m_entities[entityId]->m_components[componentId]->m_componentId = componentId;
					}

					// Making sure we clean up what we left behind
					m_entityManager->m_entities[entityId]->m_components[lastComponentId] = nullptr;

					// Finally delete the component
					// TODO: Add it to scheduled garbage collection
					delete component, component = nullptr;
					--m_componentCounter;
					break;
				}
			}

			delete temp, temp = nullptr;

		}


		
		

	};

}


#endif // COMPONENTMANAGER_H

