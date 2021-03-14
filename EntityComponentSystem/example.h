#include "ECS/ECS.h"
#include "ECS/include/Utility/Debug.h"



class AudioComponent : public ECS::Component {

public:
	static constexpr uint32_t ID = GENERATE_ID( "AudioComponent" );

	AudioComponent() : Component(ID) {}
	~AudioComponent() {}

	

};

class RenderComponent : public ECS::Component {

public:
	static constexpr uint32_t ID = GENERATE_ID("RenderComponent");
	// RenderComponent() : Component( ID ), receivedTime( 0.0f ) {}
	RenderComponent(float timeStamp) : Component(ID), receivedTime(timeStamp) {}
	~RenderComponent() {}

	float receivedTime;
};


class PhysicsComponent : public ECS::Component
{
public:
	static constexpr uint64_t ID = GENERATE_ID( "PhysicsComponent" );

	PhysicsComponent() : Component(ID) {}
	~PhysicsComponent() {}

};


class PhysicsSystem : public ECS::System<PhysicsComponent, AudioComponent>
{

public:

	static constexpr uint64_t ID = GENERATE_ID("PhysicsSystem");

	PhysicsSystem() : System(ID) {}
	~PhysicsSystem() {}

	virtual void Update( float deltaTime )
	{

		for (const auto& c : m_components) {

			PhysicsComponent* physicsComponent = std::get<PhysicsComponent*>(c);

			if (physicsComponent) {
				std::cout << "The world id yours!" << std::endl;
			}
		}

	}

};

class AudioSystem : public ECS::System<AudioComponent, RenderComponent>
{

public:

	static constexpr uint64_t ID = GENERATE_ID("AudioSystem");

	AudioSystem() : System(ID) {}
	~AudioSystem() {}

	virtual void Update(float deltaTime)
	{

		for (const auto& c : m_components) {

			AudioComponent* AUDIOCOMPONENT = std::get<AudioComponent*>(c);

			if (AUDIOCOMPONENT) {
				std::cout << "Hello World!" << std::endl;
			}

			RenderComponent* RENDERCOMPONENT = std::get<RenderComponent*>( c );

			std::cout << "Received Time: " << RENDERCOMPONENT->receivedTime << std::endl;
		}

	}

};


static int run_example() {

	DEBUG_INIT();

	ECS::EntityManager* entityManager = new ECS::EntityManager();	
	ECS::SystemManager* systemManager = new ECS::SystemManager();
	ECS::ComponentManager* componentManager = new ECS::ComponentManager(entityManager, systemManager);

	systemManager->RegisterSystem<AudioSystem>();
	systemManager->RegisterSystem<PhysicsSystem>();

	for (int i = 1; i <= 1000; i++) {
		entityManager->CreateEntity();
		// std::cout << "New Entity with Id: " <<  << std::endl;

	}

	for (int i = 1; i <= 1000; i++) {

		componentManager->AddComponent<AudioComponent>(i);
		componentManager->AddComponent<PhysicsComponent>(i);
		componentManager->AddComponent<RenderComponent>( i, 15.0f );
			
	}
	systemManager->DeregisterSystem<AudioSystem>();
	systemManager->Update(0.0f);

	for (int i = 1; i <= 1000; i++)
	{
		componentManager->RemoveComponent<PhysicsComponent>(i);
		
		componentManager->RemoveAllComponents(i);

	}
	std::cout << "Yelling!" << std::endl;

	systemManager->Update(0.0f);

	for (int i = 1; i <= 1000; i++) {

		componentManager->RemoveAllComponents(i);
		entityManager->DestroyEntity(i);

	}

	std::cout << "Testing!" << std::endl;

	

	systemManager->Update(1.0f);

	std::cout << "Complete!" << std::endl;

	ECS::World* world1 = new ECS::World();

	world1->RegisterSystem<AudioSystem>();

	std::vector<ECS::EntityId> worldEntities = world1->CreateEntitiesWithComponents<PhysicsComponent, AudioComponent>(25);

	for ( size_t i = 0; i < worldEntities.size(); i++ )
	{

		world1->AddComponentToEntity<RenderComponent>( worldEntities[i], 30.0f );

	}

	world1->RemoveComponentFromEntity<RenderComponent>(25);

	world1->Update(0.0f);

	AudioSystem* a = world1->GetSystem<AudioSystem>();
	a->Update( 10.0f );

	std::cout << "Completed World Process!" << std::endl;

	delete world1;
	world1 = nullptr;

	return 0;

	auto parser = ECS::Parser<AudioComponent, PhysicsComponent>( world1 );

	std::cout << "Test Complete!" << std::endl;

	auto list = parser.GetComponents();

	for ( auto& e : list )
	{
		AudioComponent* a = std::get<AudioComponent*>( e );
		PhysicsComponent* p = std::get<PhysicsComponent*>( e );

		if ( a )
		{
			std::cout << "Found Audio Component" << std::endl;
		}

		if ( p )
		{
			std::cout << "Found Physics Component" << std::endl;
		}
	}

	// componentManager->AddComponent<RenderComponent>( id );
	// componentManager->AddComponent<PhysicsComponent>(id);
	// componentManager->AddComponent<AudioComponent>(id);

	// 

	// componentManager->RemoveComponent<PhysicsComponent>(id);
	
	// systemManager->Update(0.0f);

	// physicsComponent = componentManager->AddComponent<PhysicsComponent>( id );

	// componentManager->AddComponent<AudioComponent>(id);
	// physicsComponent = componentManager->AddComponent<PhysicsComponent>( id );
	

	// return 0;
	// componentManager->RemoveComponent<PhysicsComponent>( id );
	// componentManager->RemoveComponent<PhysicsComponent>( id );
	// componentManager->RemoveComponent<PhysicsComponent>( id );

	


	
	


	return 0;

}