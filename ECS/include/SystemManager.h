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

		// Linked-list of active Systems on this System Manager
		std::array<ISystem*, MAX_SYSTEMS>		m_activeSystems;

		uint64_t m_systemsCounter;

	public:

		SystemManager() : m_activeSystems(), m_systemsCounter( 0 ) {}
		~SystemManager() {}


		// Add a System to this System Manager
		template <typename T>
		T* RegisterSystem() {

			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, ISystem>();

			T* system = new T();

			if ( system == nullptr )
			{
				return nullptr;
			}

			m_activeSystems[m_systemsCounter] = system;
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

			

		}

		void Update( float deltaTime )
		{
			for ( auto& s : m_activeSystems )
			{
				s->Update( deltaTime );
			}
		}

		void OnEntitySignatureChanged( const Entity& entity )
		{

			for ( const auto& s : m_activeSystems )
			{

				if ( s != nullptr )
					s->OnEntitySignatureChanged( entity );


			}


		}



	};

}

#endif // !SYSTEMMANAGER_H





