#ifndef SYSTEM_H
#define SYSTEM_H

#include "ISystem.h"
#include "Entity.h"
#include "Component.h"

#include "Utility/TemplateHelper.h"

#include <tuple>
#include <vector>

#include <iostream>

namespace ECS {

	template <typename ... Components>
	class System : public ISystem
	{
		friend class SystemManager;

		using ComponentTuple = std::tuple< Components* ... >;

	protected:

		// The list of Component Tuples, where each tuple is a set of components owned by the same entity
		std::vector<ComponentTuple>			m_components;

	public:

		explicit System(uint64_t systemId) : ISystem(systemId) {}
		virtual ~System() {}

		virtual void Update( float deltaTime ) override {}

	private:

		// If the passed entity's components match this system's signature, the components will be added to this system
		// If not, then we check to see if any of the entity's components are in the system and remove them
		virtual void OnEntitySignatureChanged( const Entity& entity ) override final
		{

			int NumberOfComponents = sizeof...( Components );
			int NumberOfMatchingComponents = 0;

			ComponentTuple componentTuple;
			for ( auto* c : entity.GetComponents() )	// For all the components on the entity
			{
				if ( c != nullptr )
				{
					
					if (ProcessEntityComponent<0, Components ...>(c, componentTuple)) 
						// When a component matches any of the acceptable types of component for this system
						// Increment the number of matching components
					{		
						++NumberOfMatchingComponents;

						
						if (NumberOfMatchingComponents == NumberOfComponents) {

							// TODO:
							// Make sure this entity's Components do not already exist on this system before adding them
							// Instead if they do exist remove them
							// This can end up being a lot of looping!
							for (size_t i = 0; i < m_components.size(); i++) {

								Component* c = std::get<0>(m_components[i]);

								if (c->GetOwnerEntity() == entity.GetId()) {

									m_components[i] = m_components[m_components.size() - 1];
									m_components.pop_back();
									break;

								}
							}

							// Add the created tuple to this system 
							m_components.push_back(componentTuple);
							break;
						}

					}

				}
				else // The moment we find a null component, we now we are at the end of the array, no need to continue
				{
					break;
				}

			}

			if (NumberOfMatchingComponents != NumberOfComponents) {	
				// If this entity'components do not match this system, lets look to see if the entity's components are already in the system

				for (size_t i = 0; i < m_components.size(); i++) {

					Component* c = std::get<0>(m_components[i]);

					if (c->GetOwnerEntity() == entity.GetId()) {

						m_components[i] = m_components[m_components.size() - 1];
						m_components.pop_back();
						break;

					}
				}

			}

			// Loop through the component tuple

		}

	private:

		// Checks to see if the passed Component's Type matches the type of component type required for this system
		// If true, returns true, otherwise recursively checks the passed component's type agains all acceptable component types
		// If there is no match returns false
		template<size_t INDEX, class ComponentClass /*Current Component Class*/, class ... Components>
		bool ProcessEntityComponent( Component* component, ComponentTuple& tupleToFill) 
		{
			
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			CanConvert_From<ComponentClass, Component>();

			if (ComponentClass::ID == component->GetComponentType()) {

				std::get<INDEX>(tupleToFill) = dynamic_cast<ComponentClass*>(component);
				return true;

			}
			else {
				// We drop the ComponentClass with each loop of recursion
				// When we run out of ComponentClasses to check, we return false, via the recursive ender
				return ProcessEntityComponent<INDEX + 1, Components ... >(component, tupleToFill);
			}

		}
		

		template<size_t INDEX>
		bool ProcessEntityComponent(Component* component, ComponentTuple& tupleToFill) {
			return false;
		}
		

	};

	

}


#endif // !SYSTEM_H








