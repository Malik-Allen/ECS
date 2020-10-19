#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <iterator>


namespace ECS {

	class ISystem;
	class IComponent;

	// Manager for a list of Systems
	class SystemManager
	{
		// 
		class EntityManager*		m_entityManager;

		// Linked-list of active Systems on this System Manager
		std::vector<ISystem*>		m_activeSystems;
		

	public:

		SystemManager(EntityManager* entityManager);
		~SystemManager();

		// Updates all Active Systems on this System Manager
		void Update(float deltaTime);


		// Add a System to this System Manager
		template <typename T>
		T* AddSystem() {

			ISystem* system = dynamic_cast<ISystem*> (new T(m_entityManager));

			if (!system)
				return nullptr;


			m_activeSystems.push_back(system);

			return dynamic_cast<T*>(system);
		}


		// Removes System of the passed type from the list of Systems on this Manager
		template <typename T>
		void RemoveSystem() 
		{

			// Iterator for vector
			auto i = m_activeSystems.begin();

			while (i != m_activeSystems.end()) {

				T* system = dynamic_cast<T*>(i);

				if (system != nullptr) {

					delete system, system = nullptr;
					
					m_activeSystems.erase(i);

					break;
				}

				++i;
			}

		}



	};

}

#endif // !SYSTEMMANAGER_H





