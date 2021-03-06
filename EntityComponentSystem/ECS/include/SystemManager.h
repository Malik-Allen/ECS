#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "Utility/TemplateHelper.h"
#include "ECS_Definitions.h"
#include "ISystem.h"

#include <array>



namespace ECS {

	// Manager for a list of Systems
	class SystemManager
	{
		friend class ComponentManager;

		// Linked-list of active Systems on this System Manager
		std::array<ISystem*, MAX_SYSTEMS>		m_activeSystems;

		// The Number of Systems active inside of this System Manager
		uint64_t			m_systemsCounter;

	public:

		SystemManager() : m_activeSystems(), m_systemsCounter( 0 ) {}
		
		~SystemManager() { UnregisterAllSystems(); }


		// Add a System to this System Manager
		template <typename T, typename ... Args>
		T* RegisterSystem( Args&& ... args ) 
		{

			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, ISystem>();

			if (m_systemsCounter >= MAX_SYSTEMS) 
			{
				DEBUG_LOG( LOG::WARNING, "Failed to register system: Max system count reached" );
				CONSOLE_LOG( LOG::WARNING, "Failed to register entity: Max system count reached" );
				return nullptr;
			}

			T* system = new T( std::forward<Args>(args) ... );

			if ( system == nullptr )
			{
				DEBUG_LOG( LOG::WARNING, "Failed to create system: system is nullptr" );
				CONSOLE_LOG( LOG::WARNING, "Failed to create system: system is nullptr" );
				return nullptr;
			}

			system->m_systemManagerId = this->m_systemsCounter;
			m_activeSystems[this->m_systemsCounter] = system;
			++m_systemsCounter;

			return system;

		}


		// Removes System of the passed type from the list of Systems on this Manager
		template <typename T>
		void UnregisterSystem() 
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, ISystem>();

			ISystem* system = nullptr;
			for (auto* s : m_activeSystems) {


				if (s != nullptr)
				{
					
					if (T::ID == s->m_systemId) {
						// When we find a system with the same id, we will remove it from our array of active systems
						system = s;

						uint64_t systemManagerId = system->m_systemManagerId;
						uint64_t lastIndex = --this->m_systemsCounter;

						m_activeSystems[systemManagerId] = m_activeSystems[lastIndex];
						m_activeSystems[lastIndex] = nullptr;

						if (m_activeSystems[systemManagerId] != nullptr) 
						{
							m_activeSystems[systemManagerId]->m_systemManagerId = systemManagerId;
						}

						delete system, system = nullptr;

						break;
					}


				}
				else // The moment we find a null system, we now we are at the end of the array, no need to continue
				{
					break;
				}


			}
			

		}

		template <typename T>
		T* GetSystem()
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, ISystem>();

			ISystem* system = nullptr;
			for ( auto* s : m_activeSystems )
			{


				if ( s != nullptr )
				{

					if ( T::ID == s->m_systemId )
					{
						// When we find a system with the same id, we will remove it from our array of active systems
						return dynamic_cast<T*>(s);
					}


				}
				else // The moment we find a null system, we now we are at the end of the array, no need to continue
				{
					break;
				}


			}

			return nullptr;
		}

		// Calls Update on all active systems, inside of this system manager
		void Update( float deltaTime )
		{
			for ( auto* s : m_activeSystems )
			{
				if (s != nullptr)
					s->Update(deltaTime);
				else
					break;
			}
		}

	private:

		// Updates Systems in the manager when an entity's signature has changed update the system manager's systems
		void OnEntitySignatureChanged( const Entity& entity )
		{

			for ( const auto& s : m_activeSystems )
			{

				if (s != nullptr)
				{
					s->OnEntitySignatureChanged(entity);
				}
				else // The moment we find a null system, we now we are at the end of the array, no need to continue
				{
					break;
				}

			}


		}

		// Unregisters all systems
		void UnregisterAllSystems()
		{

			for (auto* s : m_activeSystems) {

				if (s != nullptr)
				{

					delete s, s = nullptr;

				}

			}

			m_activeSystems.empty();

		}



	};

}

#endif // !SYSTEMMANAGER_H





