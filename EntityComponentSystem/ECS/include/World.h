#ifndef WORLD_H
#define WORLD_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "Utility/TemplateHelper.h"

#include <vector>

#include <iostream>

namespace ECS
{

// User's Manual:
// 1. Through a world reference you will be able access the functionality of this Entity Component System
// 2. So the only way to access the ecs is via the world, which will make sure everything is allocated, managed, and deleted correctly
// 3. You can store your entityIds, component references however you please, no deletes of components or of systems allowed
// 4. Every component on an entity must be unique, meaning: Multiple components of the same type on a single entity is currently not supported
// 5. Every System must be unique, meaning : Multiple Systems of the same type / class being registered is currently not supported
// 6. Every Component class must have a public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_Component" );
// 7. Every System class must have public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_System" );


	class World
	{

		// DLL Notes:
		// Create a Create World Fucntion to return a new ECS::World()

		EntityManager* m_enityManager;

		SystemManager* m_systemManager;

		ComponentManager* m_componentManager;



	public:

		// Constructs ECS system
		World() :
			m_enityManager( new ECS::EntityManager() ),
			m_systemManager( new ECS::SystemManager() ),
			m_componentManager( new ECS::ComponentManager( m_enityManager, m_systemManager ) )
		{}

		// Cleans and deletes ecs system
		~World()
		{
			// Each Manager will handle the destruction of their items

			// Systems get deleted first, so when we remove components, they no longer
			if ( m_systemManager )
			{
				delete m_systemManager;
				m_systemManager = nullptr;
			}

			// Now we remove all components from the component manager
			if ( m_componentManager )
			{
				delete m_componentManager;
				m_componentManager = nullptr;
			}

			// Finally clear out all entities
			if ( m_enityManager )
			{
				delete m_enityManager;
				m_enityManager = nullptr;
			}
		}

		// Will create the number of entities passed, given that you do not exceed entity limits
		std::vector<EntityId> CreateEntities( uint64_t numberOfEntities )
		{
			std::vector<EntityId> createdEntities;
			EntityId currentEntityId;

			for ( int i = 0; i < numberOfEntities; i++ )
			{
				currentEntityId = m_enityManager->CreateEntity();

				if ( currentEntityId != 0 )
					// A valid Entity Id is a non-zero value
				{
					createdEntities.push_back( currentEntityId );
				}
				
			}

			return createdEntities;
		}


		// Will create 'n' number of entities with the passed components added to it, Returns vector of the entityIds
		template<class ... Components>
		std::vector<EntityId> CreateEntitiesWithComponents( uint64_t numberOfEntities )
		{
			std::cout << "Creating " << numberOfEntities << " entities..." << std::endl;
			std::vector<EntityId> createdEntities;
			EntityId currentEntityId = 0;
			for ( int i = 0; i < numberOfEntities; i++ )
			{
				currentEntityId = m_enityManager->CreateEntity();

				std::cout << "Current Iteration: " << i << std::endl;
				std::cout << "Current EntityId: " << currentEntityId << std::endl;

				if ( currentEntityId != 0 )
					// A valid Entity Id is a non-zero number, therefore add components to valid entities
				{

					AddNewComponentToEntity< 0, Components ... >( currentEntityId );

					createdEntities.push_back( currentEntityId );

				}

			}

			return createdEntities;

		}

		// Destroys Entity with the passed EntityId, removing all components in the process
		void DestroyEntity( EntityId entityId )
		{
			m_componentManager->RemoveAllComponents( entityId );
			m_enityManager->DestroyEntity( entityId );
		}

		// GetAllEntitiesWithComponents< ... Components>()
			// Returns a vector of entity ids, that have the components passed

		

		// Adds Component to entity with passed EntityId
		template<typename T, typename ... Args>
		T* AddComponentToEntity( EntityId entityId, Args&& ... args )
		{
			return m_componentManager->AddComponent<T, Args ...>( entityId, std::forward<Args>( args ) ... );
		}

		// Removes component from entity with passed EntityId, if Component exists on entity
		template<typename T>
		void RemoveComponentFromEntity( EntityId entityId )
		{
			m_componentManager->RemoveComponent<T>( entityId );
		}


		// Registers Systems, inside of system manager
		template<typename T>
		T* RegisterSystem()
		{
			return m_systemManager->RegisterSystem<T>();
		}

		// Unregisters system from system manager
		template<typename T>
		void UnregisterSystem()
		{
			m_systemManager->UnregisterSystem<T>();
		}

		// Registers Systems, inside of system manager
		template<typename T>
		T* GetSystem()
		{
			return m_systemManager->GetSystem<T>();
		}


		// Update World Systems
		void Update( float deltaTime )
		{
			m_systemManager->Update( deltaTime );
		}

	private:


		// Recursively adds components to the entity with the passed id
		template<size_t INDEX, typename ComponentClass, typename ... Components>
		void AddNewComponentToEntity( EntityId entityId )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<ComponentClass, Component>();

			std::cout << "Adding Component to Entity: " << entityId << std::endl;
			if ( m_componentManager->AddComponent<ComponentClass>( entityId ) != nullptr )
				// When we successfully add a component, carry on with the process
			{
				AddNewComponentToEntity< INDEX + 1, Components ... >( entityId );
			}


		}

		template<size_t INDEX>
		void AddNewComponentToEntity( EntityId entityId ) { std::cout << "Completed Adding Components to Entity: " << entityId << std::endl; }


	};

}



#endif // !WORLD_H

