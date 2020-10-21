#include "ECS/ECS.h"

#include <iostream>
#include <string>

#include <vector>


class Object : public ECS::Entity {

public:

	Object() {}
	~Object() {}

};

class AudioComponent : public ECS::Component {

public:

	AudioComponent() : ECS::Component( Component_Type::AudioComponent ) {}
	~AudioComponent() {}

	void Play() {
		std::cout << "The World Is Yours!" << std::endl;
	}

};

class PhysicsComponent : public ECS::Component
{

public:

	PhysicsComponent() : ECS::Component( Component_Type::PhysicsComponent ) {}
	~PhysicsComponent() {}

	void Run()
	{
		std::cout << "Every Action has a equal and opposite reaction!" << std::endl;
	}

};

class AudioSystem : public ECS::System<AudioComponent> {

	using BaseType = System<AudioComponent>;

public:

	AudioSystem(ECS::EntityManager* entityManager) : System(entityManager) {}
	~AudioSystem() {}

	virtual void Update(float deltaTime) override {

		for (auto& component : m_components) {

			component->Play();
		}

	}

	
};


int main(int args, char* argv[]) {

	
	ECS::EntityManager* entityManager = new ECS::EntityManager();	
	ECS::ComponentManager* componentManager = new ECS::ComponentManager();
	ECS::SystemManager* systemManager = new ECS::SystemManager();

	entityManager->AssignComponentManager( componentManager );
	componentManager->AssignSystemManager( systemManager );
	systemManager->AssignEntityManager( entityManager );

	AudioSystem* audioSystem = systemManager->AddSystem<AudioSystem>();

	ECS::EntityId entityId = entityManager->CreateEntity<Object>();

	std::cout << "Current EntityId: " << entityId << std::endl;

	AudioComponent* audio = entityManager->GetEntity(entityId)->AddComponent<AudioComponent>();

	std::cout << "Component Id: " << audio->GetComponentId() << " Component OwnerId: " << audio->GetOwnerId() << std::endl;

	

	
	PhysicsComponent* physics = entityManager->GetEntity( entityId )->AddComponent<PhysicsComponent>();
	std::cout << "Component Id: " << physics->GetComponentId() << " Component OwnerId: " << physics->GetOwnerId() << std::endl;

	entityManager->GetEntity( entityId )->RemoveComponent<PhysicsComponent>( physics->GetComponentId() );

	/*if ( dynamic_cast<PhysicsComponent*>(audio) )
	{
		audio->Play();
	}
	else
	{
		physics->Run();
	}*/

	systemManager->Update( 0.0f );
	systemManager->Update( 0.0f );


	entityManager->DestroyEntity(entityId);



	delete entityManager, entityManager = nullptr;

	delete componentManager, componentManager = nullptr;


	return 0;

}