#include "SystemManager.h"

#include "ISystem.h"

namespace ECS {


	SystemManager::SystemManager() : m_entityManager(nullptr) {}

	SystemManager::~SystemManager() {}

	
	// Updates all Active Systems on this System Manager
	void SystemManager::Update(float deltaTime) 
	{
	
		for (ISystem* system : m_activeSystems) {
			system->Update(deltaTime);
		}
	
	}

}
