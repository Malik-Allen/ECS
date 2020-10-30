#include "ECS/ECS.h"



class AudioComponent : public ECS::Component {

	static constexpr uint32_t ID = COMPILE_TIME_CRC32_STR( "AudioComponent" );
public:

	AudioComponent() : Component(ID) {}
	~AudioComponent() {}

	

};

class PhysicsComponent : public ECS::Component
{
	static constexpr uint64_t ID = COMPILE_TIME_CRC32_STR( "PhysicsComponent" );
public:

	PhysicsComponent() : Component(ID) {}
	~PhysicsComponent() {}

};


class PhysicsSystem : public ECS::System<PhysicsComponent, AudioComponent, PhysicsComponent>
{

public:

	PhysicsSystem() {}
	~PhysicsSystem() {}

	virtual void Update( float deltaTime )
	{

	}

};



int main(int args, char* argv[]) {

	
	ECS::EntityManager* entityManager = new ECS::EntityManager();	
	ECS::SystemManager* systemManager = new ECS::SystemManager();
	ECS::ComponentManager* componentManager = new ECS::ComponentManager(entityManager, systemManager);

	ECS::EntityId id = entityManager->CreateEntity();

	systemManager->RegisterSystem<PhysicsSystem>();

	PhysicsComponent* physicsComponent = componentManager->AddComponent<PhysicsComponent>( id );

	return 0;

	physicsComponent = componentManager->AddComponent<PhysicsComponent>( id );

	// std::cout << physicsComponent->m_componentType << std::endl;

	physicsComponent = componentManager->AddComponent<PhysicsComponent>( id );

	componentManager->RemoveComponent<PhysicsComponent>( id );
	componentManager->RemoveComponent<PhysicsComponent>( id );
	componentManager->RemoveComponent<PhysicsComponent>( id );

	componentManager->AddComponent<AudioComponent>( id );
	for ( int i = 0; i < 23; i++ )
	{
		componentManager->AddComponent<PhysicsComponent>( id );
	}

	for ( int i = 0; i < 12; ++i )
	{
		componentManager->RemoveComponent<PhysicsComponent>( id );
	}


	componentManager->RemoveComponent<AudioComponent>( id );
	// componentManager->RemoveComponent<PhysicsComponent>( id );


	return 0;

}