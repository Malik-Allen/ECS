#include "SystemManager.h"

#include "ISystem.h"

namespace ECS {


	SystemManager::SystemManager(EntityManager* entityManager) : m_entityManager(entityManager) {}

	SystemManager::~SystemManager() {}

	
	// Updates all Active Systems on this System Manager
	void SystemManager::Update(float deltaTime) 
	{
	
		for (ISystem* system : m_activeSystems) {
			system->Update(deltaTime);
		}
	
	}

}
