#ifndef PARSER_H
#define PARSER_H

#include "Entity.h"
#include "Component.h"
#include "World.h"

#include <tuple>
#include <vector>

namespace ECS
{

	template<typename ... Components>
	struct Parser
	{
		using ComponentTuple = std::tuple< Components* ... >;

		Parser( World* world )
		{
			if ( world == nullptr )
			{
				return;
			}

			uint64_t entityCount = world->m_enityManager->m_entityCounter;

			for ( int i = 1; i <= entityCount; i++ )
			{
				SearchEntity( *world->m_enityManager->m_entities[i] );
			}
		}

		~Parser()
		{
			m_components.clear();
		}

		std::vector<ComponentTuple> GetComponents() const { return m_components; }

	private:

		std::vector<ComponentTuple>	m_components;

		void SearchEntity( const Entity& entity )
		{

			int NumberOfComponents = sizeof...( Components );
			int NumberOfMatchingComponents = 0;

			ComponentTuple componentTuple;
			for ( auto* c : entity.GetComponents() )	// For all the components on the entity
			{
				if ( c != nullptr )
				{

					if ( ProcessEntityComponent<0, Components ...>( c, componentTuple ) )
						// When a component matches any of the acceptable types of component for this system
						// Increment the number of matching components
					{
						++NumberOfMatchingComponents;


						if ( NumberOfMatchingComponents == NumberOfComponents )
						{
							for ( size_t i = 0; i < m_components.size(); i++ )
							{

								Component* c = std::get<0>( m_components[i] );

								if ( c->GetOwnerEntity() == entity.GetId() )
								{

									m_components[i] = m_components[m_components.size() - 1];
									m_components.pop_back();
									break;

								}
							}

							// Add the created tuple to this system 
							m_components.push_back( componentTuple );
							break;
						}

					}

				}
				else // The moment we find a null component, we now we are at the end of the array, no need to continue
				{
					break;
				}

			}

			if ( NumberOfMatchingComponents != NumberOfComponents )
			{
				// If this entity'components do not match this system, lets look to see if the entity's components are already in the system

				for ( size_t i = 0; i < m_components.size(); i++ )
				{

					Component* c = std::get<0>( m_components[i] );

					if ( c->GetOwnerEntity() == entity.GetId() )
					{

						m_components[i] = m_components[m_components.size() - 1];
						m_components.pop_back();
						break;

					}
				}

			}

			// Loop through the component tuple

		}

		template<size_t INDEX, class ComponentClass /*Current Component Class*/, class ... Components>
		bool ProcessEntityComponent( Component* component, ComponentTuple& tupleToFill )
		{

			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<ComponentClass, Component>();

			if ( ComponentClass::ID == component->GetComponentType() )
			{

				std::get<INDEX>( tupleToFill ) = dynamic_cast<ComponentClass*>( component );
				return true;

			}
			else
			{
				// We drop the ComponentClass with each loop of recursion
				// When we run out of ComponentClasses to check, we return false, via the recursive ender
				return ProcessEntityComponent<INDEX + 1, Components ... >( component, tupleToFill );
			}

		}


		template<size_t INDEX>
		bool ProcessEntityComponent( Component* component, ComponentTuple& tupleToFill )
		{
			return false;
		}
	};
}

#endif // !PARSER_H

