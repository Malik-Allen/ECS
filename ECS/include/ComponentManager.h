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
		// All Components created by this component manager
		std::array<Component*, MAX_COMPONENTS> m_components;

		// TODO:
		// This vector will be a list of components marked as ready to be destroyed
			// Scehduled destruction will take place
		std::vector<Component*> m_trash;

		// The number of components on this component manager
		uint64_t				m_componentCounter;

		// Entity Manager reference
		EntityManager*			m_entityManager;

		using ComponentMap = std::map< uint64_t /*Component Type*/, std::vector<Component*> /*Components*/ >;

		// Map of Components sorted by the component type, where the component type is a key to a list of components of that type
		ComponentMap			m_componentMap;

		// System Manager reference
		SystemManager*			m_systemManager;


	public:

		ComponentManager(EntityManager* entityManager, SystemManager* systemManager) : 
			m_components(),
			m_componentCounter( 0 ), 
			m_entityManager(entityManager) ,
			m_systemManager( systemManager )
		{}

		~ComponentManager() { RemoveAllComponentsOnManager(); }


		// Add Component to Entity with the Id, returns created Component, returns fall if entity does not exist
		template<typename T, typename ... Args>
		T* AddComponent( EntityId entityId, Args&& ... args )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, Component>();

			if ( m_componentCounter >= MAX_COMPONENTS )	// We are at capacity, return 
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

			if ( entity->m_componentCounter >= MAX_COMPONENTS_PER_ENTITY )	// This entity is at its capacity
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
			entity->m_components[component->m_componentId] = component;

			component->m_componentManagerId = this->m_componentCounter;
			m_components[component->m_componentManagerId] = component;
			++this->m_componentCounter;

			// Also add a reference of this component to the ComponentMap
			m_componentMap[T::ID].push_back( component );	

			if (m_systemManager) {
				// This entity's signature has now changed update the system manager's systems
				m_systemManager->OnEntitySignatureChanged(*entity);
			}
			
			
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

			// Loop through the components of the same type, the id of the component type is used as the key to a vector of components 
			Component* component = nullptr;
			for ( auto c = m_componentMap[T::ID].begin(); c != m_componentMap[T::ID].end(); c++ )	// Iterator through vector of components from the same type
			{
				component = *c;	

				if ( component->m_ownerId == entityId )		// When we find the first component with the same owner, we will remove it
				{

					// Erase it from the component map, so it can no longer be referenced
					m_componentMap[T::ID].erase( c );	

					ComponentId componentId = component->m_componentId;

					// Save this for the swapping later
					uint64_t lastComponentId = --m_entityManager->m_entities[entityId]->m_componentCounter;	

					// Assign the component index of the component we are about to delete to the last component on this entity
					m_entityManager->m_entities[entityId]->m_components[componentId] = m_entityManager->m_entities[entityId]->m_components[lastComponentId];

					// If this component is a valid component, then we give it a new component id
					if ( m_entityManager->m_entities[entityId]->m_components[componentId] != nullptr )
					{
						m_entityManager->m_entities[entityId]->m_components[componentId]->m_componentId = componentId;
					}

					// Making sure we clean up what we left behind
					m_entityManager->m_entities[entityId]->m_components[lastComponentId] = nullptr;

					// Now we will perform the similar operation for the Component Manager's Array of Components
					// Just using local function variables that already exist
					componentId = component->m_componentManagerId;
					lastComponentId = --this->m_componentCounter;

					// Assign the last component in the array to the removed component's index
					m_components[componentId] = m_components[lastComponentId];
					m_components[lastComponentId] = nullptr;

					if (m_components[componentId] != nullptr) 
					{
						m_components[componentId]->m_componentManagerId = componentId;
					}

					// Update systems, now that we have removed a component from this entity
					m_systemManager->OnEntitySignatureChanged(*entity);

					// Finally delete the component
					// TODO: Add it to scheduled garbage collection
					delete component, component = nullptr;

					break;
				}
			}

		}

		// Removes all the components from the entity with the passed id
		void RemoveAllComponents(EntityId entityId) 
		{

			Entity* entity = m_entityManager->m_entities[entityId];
			if (entity == nullptr)	// Entity does not exist
			{
				// Some Debug
				return;
			}


			for (auto* c : entity->GetComponents()) 
			{
				if (c != nullptr) 
				{

					RemoveComponent(*entity, c->m_componentType);

				}
				else // The moment we find a null component, we now we are at the end of the array, no need to continue
				{
					break;
				}
			}


		}


	private:

		void RemoveAllComponentsOnManager()
		{
			for (size_t i = 0; i < m_components.size(); i++)
			{

				if (m_components[i] != nullptr)
				{
					delete m_components[i];
					m_components[i] = nullptr;
				}

			}

			m_componentMap.clear();
		}


		// Utility function for the ComponentManager to remove components given their component type
		void RemoveComponent(const Entity& entity, const uint64_t& componentType) 
		{

			// Loop through the components of the same type, the id of the component type is used as the key to a vector of components 
			Component* component = nullptr;
			for (auto c = m_componentMap[componentType].begin(); c != m_componentMap[componentType].end(); c++)	// Iterator through vector of components from the same type
			{
				component = *c;

				if (component->m_ownerId == entity.m_entityId)		// When we find the first component with the same owner, we will remove it
				{

					// Erase it from the component map, so it can no longer be referenced
					m_componentMap[componentType].erase(c);

					ComponentId componentId = component->m_componentId;

					// Removing the reference from the list of components on the passed entity
					m_entityManager->m_entities[entity.m_entityId]->m_components[componentId] = nullptr;
					--m_entityManager->m_entities[entity.m_entityId]->m_componentCounter;

					// Now we will perform the similar operation for the Component Manager's Array of Components
					// Just using local function variables that already exist
					componentId = component->m_componentManagerId;
					ComponentId lastComponentId = --this->m_componentCounter;

					// Assign the last component in the array to the removed component's index
					m_components[componentId] = m_components[lastComponentId];
					m_components[lastComponentId] = nullptr;

					if (m_components[componentId] != nullptr)
					{
						m_components[componentId]->m_componentManagerId = componentId;
					}

					if (m_systemManager) {
						// Update systems, now that we have removed a component from this entity
						m_systemManager->OnEntitySignatureChanged(entity);
					}
					

					// Finally delete the component
					// TODO: Add it to scheduled garbage collection
					delete component, component = nullptr;

					break;
				}
			}


		}
		
		

	};

}


#endif // COMPONENTMANAGER_H

