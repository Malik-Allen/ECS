#ifndef WORLD_H
#define WORLD_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ECS {

	// User's Manual:
	// 1. Through a world reference you will be able to call the below functionality
	// 2. So the only way to access the ecs is via the world, which will make sure everything is allocated, managed, and deleted correctly
	// 3. You can store your entityIds, component references how ever you please, no deletes of components or of systems allowed
	// 4. Every component on an entity must be unique, meaning: Multiple components of the same type on a single entity is currently not supported
	// 5. Every System must be unique, meaning: Multiple Systems of the same type/class being registered is currently not supported
	// 6. Every Component class must have a public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_Component" );
	// 7. Every System class must have public static constexpr uint32_t ID = GENERATE_ID( "EXAMPLE_System" );

	class World 
	{

		// DLL Notes:
		// Create a Create World Fucntion to return a new ECS::World()

		// EntityManager*		m_enityManager;

		// ComponentManager*	m_componentManager;

		// SystemManager*		m_systemManager;

	public:

		// Constructs ECS system
		World() {}

		// Cleans and deletes ecs system
		~World() {}		// OnDestroy()

		// OnCreate() 
			// Create Entity, Component and System Managers
			// Called in constructor

		// OnDestroy()
			// Destroys all entities, components and systems inside of this world's managers
			// Calling clean up and finallly deleting memory



		// CreateEntityWithComponents<... Components>(int numberOfEntities)
			// Will create 'n' number of entities with the passed components added to it
			// (Potentially) At the cost of no constructor parameters for individual components... 
			// Returns vector of the entityIds

		// GetAllEntitiesWithComponents< ... Components>()
			// Returns a vector of entity ids, that have the components passed

		// DestroyEntity(EntityId Id)
			// Destroys entity with that id



		// AddComponentToEntity<class Component>(EntityId, ComponentConstrutor Parameters)
			// Adds component to entity with passed id
			// Returns the Reference to the component

		// RemoveComponentFromEntity<class Component>(EntityId)
			// Removes component from the entity with the passed id




		// RegisterSystem<class System>()
			// Registers system inside of system manager
			// Returns a pointer to the system that was created

		// UnRegisterSystem<class System>()
			// Unregisters system inside of system manager if it exists

		// Update(float delta)
			// Updates all systems within the world


	};

}



#endif // !WORLD_H

