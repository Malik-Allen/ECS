#ifndef WORLD_H
#define WORLD_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

#include "Utility/TemplateHelper.h"
#include "Utility/Debug.h"

#include <vector>

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

		template<typename ... T >
		friend struct Parser;

	public:

		// Constructs ECS system
		World() :
			m_enityManager( new ECS::EntityManager() ),
			m_systemManager( new ECS::SystemManager() ),
			m_componentManager( new ECS::ComponentManager( m_enityManager, m_systemManager ) )
		{
			DEBUG_LOG( LOG::INFO, "World created... " );
			CONSOLE_LOG( LOG::INFO, "World created... " );
		}

		// Cleans and deletes ecs system
		~World()
		{
			// Each Manager will handle the destruction of their items
			DEBUG_LOG( LOG::INFO, "Destroying world... " );
			CONSOLE_LOG( LOG::INFO, "Destroying world... " );

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

			DEBUG_LOG( LOG::INFO, "Destroying world... COMPLETE" );
			CONSOLE_LOG( LOG::INFO, "Destroying world... COMPLETE" );
		}

		// Will create the number of entities passed, given that you do not exceed entity limits
		std::vector<EntityId> CreateEntities( uint64_t numberOfEntities )
		{
			DEBUG_LOG( LOG::INFO, "Creating: " + std::to_string( numberOfEntities ) + " entities" );
			CONSOLE_LOG( LOG::INFO, "Creating: " + std::to_string( numberOfEntities ) + " entities" );

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

			DEBUG_LOG( LOG::INFO, "Succeffully created: " + std::to_string( createdEntities.size() ) + " entities" );
			CONSOLE_LOG( LOG::INFO, "Succeffully created: " + std::to_string( createdEntities.size() ) + " entities" );

			return createdEntities;
		}


		// Will create 'n' number of entities with the passed components added to it, Returns vector of the entityIds
		template<class ... Components>
		std::vector<EntityId> CreateEntitiesWithComponents( uint64_t numberOfEntities )
		{

			DEBUG_LOG( LOG::INFO, "Creating: " + std::to_string( numberOfEntities ) + " entities" );
			CONSOLE_LOG( LOG::INFO, "Creating: " + std::to_string( numberOfEntities ) + " entities" );

			std::vector<EntityId> createdEntities;
			EntityId currentEntityId = 0;
			for ( int i = 0; i < numberOfEntities; i++ )
			{
				currentEntityId = m_enityManager->CreateEntity();

				if ( currentEntityId != 0 )
					// A valid Entity Id is a non-zero number, therefore add components to valid entities
				{

					AddNewComponentToEntity< 0, Components ... >( currentEntityId );

					createdEntities.push_back( currentEntityId );

				}

			}

			DEBUG_LOG( LOG::INFO, "Succeffully created: " + std::to_string( createdEntities.size() ) + " entities" );
			CONSOLE_LOG( LOG::INFO, "Succeffully created: " + std::to_string( createdEntities.size() ) + " entities" );

			return createdEntities;

		}

		// Destroys Entity with the passed EntityId, removing all components in the process
		void DestroyEntity( EntityId entityId )
		{
			DEBUG_LOG( LOG::INFO, "Destroying entity with id: " + std::to_string( entityId ) );
			CONSOLE_LOG( LOG::INFO, "Destroying entity with id: " + std::to_string( entityId ) );

			m_componentManager->RemoveAllComponents( entityId );
			m_enityManager->DestroyEntity( entityId );
		}

		// GetAllEntitiesWithComponents< ... Components>()
			// Returns a vector of entity ids, that have the components passed



		// Adds Component to entity with passed EntityId
		template<typename T, typename ... Args>
		T* AddComponentToEntity( EntityId entityId, Args&& ... args )
		{
			DEBUG_LOG( LOG::INFO, "Adding component with id: " + std::to_string( T::ID ) + " to entity with id: " + std::to_string( entityId ) );
			CONSOLE_LOG( LOG::INFO, "Adding component with id: " + std::to_string( T::ID ) + " to entity with id: " + std::to_string( entityId ) );
			return m_componentManager->AddComponent<T, Args ...>( entityId, std::forward<Args>( args ) ... );
		}

		// Removes component from entity with passed EntityId, if Component exists on entity
		template<typename T>
		void RemoveComponentFromEntity( EntityId entityId )
		{
			DEBUG_LOG( LOG::INFO, "Removing component with id: " + std::to_string( T::ID ) + " from entity with id: " + std::to_string( entityId ) );
			CONSOLE_LOG( LOG::INFO, "Removing component with id: " + std::to_string( T::ID ) + " from entity with id: " + std::to_string( entityId ) );
			m_componentManager->RemoveComponent<T>( entityId );
		}


		// Registers Systems, inside of system manager
		template<typename T>
		T* RegisterSystem()
		{
			DEBUG_LOG( LOG::INFO, "Registering system with id: " + std::to_string( T::ID ) );
			CONSOLE_LOG( LOG::INFO, "Registering system with id: " + std::to_string( T::ID ) );
			return m_systemManager->RegisterSystem<T>();
		}

		// Deregisters system from system manager
		template<typename T>
		void DeregisterSystem()
		{
			DEBUG_LOG( LOG::INFO, "Deregistering system with id: " + std::to_string( T::ID ) );
			CONSOLE_LOG( LOG::INFO, "Deregistering system with id: " + std::to_string( T::ID ) );
			m_systemManager->DeregisterSystem<T>();
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

			DEBUG_LOG( LOG::INFO, "Adding component: " + std::to_string( ComponentClass::ID ) + " to entity with id: " + std::to_string( entityId ) + " Index: " + std::to_string( INDEX ) );
			CONSOLE_LOG( LOG::INFO, "Adding component: " + std::to_string( ComponentClass::ID ) + " to entity with id: " + std::to_string( entityId ) + " Index: " + std::to_string( INDEX ) );

			if ( m_componentManager->AddComponent<ComponentClass>( entityId ) != nullptr )
				// When we successfully add a component, carry on with the process
			{
				AddNewComponentToEntity< INDEX + 1, Components ... >( entityId );
			}


		}

		template<size_t INDEX>
		void AddNewComponentToEntity( EntityId entityId )
		{
			DEBUG_LOG( LOG::INFO, "Adding component: COMPLETE: entity with id: " + std::to_string( entityId ) + " Index: " + std::to_string( INDEX ) );
			CONSOLE_LOG( LOG::INFO, "Adding component: COMPLETE: entity with id: " + std::to_string( entityId ) + " Index: " + std::to_string( INDEX ) );
		}


	};

}



#endif // !WORLD_H

