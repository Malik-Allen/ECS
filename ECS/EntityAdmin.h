#ifndef ENTITYADMIN_H
#define ENTITYADMIN_H

#include "Entity.h"
#include "Component.h"
#include "ISystem.h"

#include <map>
#include <array>

namespace ECS
{

	class EntityAdmin
	{
		

		static constexpr std::size_t m_maxEntity{ 1000 };
		static constexpr std::size_t m_maxComponent{ 1000 };
		static constexpr std::size_t m_maxSystem{ 1000 };

	private:

		std::map<EntityId, Entity*>	m_entities;

		std::array<Component*, m_maxComponent>*		m_components;

		std::array<ISystem*, m_maxSystem>		m_systems;

		uint64_t					m_entityCounter;

		uint64_t					m_componentCounter;


		// Template Parameter Compile Constraint, Thanks Bjarne Stroustrup: https://www.stroustrup.com/bs_faq2.html#constraints
		template<class T, class B> struct CanConvert_From
		{
			static void constraints( T* p ) { B* pb = p; }
			// Complile-time check to see if class T can be converted to class B, valid for derivation check of class T from class B
			CanConvert_From() { void( *p )( T* ) = constraints; }
		};


	public:

		EntityAdmin() : m_components(nullptr), m_entityCounter(0), m_componentCounter(0) {}
		~EntityAdmin() {}

		void Update( float deltaTime );

		void OnCreate();
		void OnDestroy();


		EntityId CreateEntity();
		void DestroyEntity(EntityId);
		void DestroyAllEntities();

		// Perform registration for thie component
		template<typename T, typename ... Args> 
		T* AddComponent( EntityId entityId, Args&& ... args )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, Component>();

			// Component Classes can support different constructors, 0 -> n number of paramters in their constructor
			T* component = new T( std::forward<Args>( args ) ... );

			if ( component == nullptr )
			{
				// Some Debug
				return nullptr;
			}

			// If no entity with the passed id exists, create a new one
			if ( m_entities[entityId] == nullptr )
			{
				Entity* entity = new Entity();
				if ( entity == nullptr )
				{
					// Some debug
					return nullptr;
				}

				entity->m_entityId = ++m_entityCounter;
				m_entities[entityId] = entity;
			}

			component->m_ownerId = entityId;
			component->m_componentId = ++m_componentCounter;

			m_entities[entityId]->m_components.push_back( component );
			m_components->push_back( component );




			return component;

		}

		// Sign out the component with the passed id for scheduled deletion
		void RemoveComponent( EntityId entity, ComponentId componentId );

		template<typename T>
		void AddSystem()
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<T, ISystem>();

			T* system = new T();

			if ( system == nullptr )
			{
				// some debug
				return;
			}

			m_systems.push_back( system );

		}

		template<typename T>
		void RemoveSystem()
		{
			std::iterator i = m_systems.begin();
			while ( i != m_systems.end() )
			{

				if ( T * ( i ) != nullptr )
				{
					m_systems.erase( i );
					break;
				}

			}

		}
		

	};
}


#endif // !ENTITYADMIN_H

