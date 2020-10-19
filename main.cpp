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

	AudioComponent() {}
	~AudioComponent() {}

	void Play() {
		std::cout << "The World Is Yours!" << std::endl;
	}

};

class AudioSystem : public ECS::System<AudioComponent> {

	using BaseType = System<AudioComponent>;

public:

	AudioSystem(ECS::EntityManager* entityManager) : System(entityManager) {}
	~AudioSystem() {}

	virtual void Update(float deltaTime) override {

		for (auto& component : m_components) {

			AudioComponent* pAudio = std::get<AudioComponent*>(component);

			pAudio->Play();
		}

	}

	
};


int main(int args, char* argv[]) {

	

	ECS::ComponentManager* componentManager = new ECS::ComponentManager();

	ECS::EntityManager* entityManager = new ECS::EntityManager(componentManager);	

	ECS::SystemManager* systemManager = new ECS::SystemManager(entityManager);

	ECS::EntityId entityId = entityManager->CreateEntity<Object>();

	std::cout << "Current EntityId: " << entityId << std::endl;

	AudioComponent* audio = entityManager->GetEntity(entityId)->AddComponent<AudioComponent>();

	std::cout << "Component Id: " << audio->GetComponentId() << " Component OwnerId: " << audio->GetOwnerId() << std::endl;

	AudioSystem* audioSystem = systemManager->AddSystem<AudioSystem>();

	

	entityManager->DestroyEntity(entityId);

	delete componentManager, componentManager = nullptr;

	delete entityManager, entityManager = nullptr;

	return 0;

}