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
		
		using ComponentType = std::tuple< Components ... >;

		using ComponentTuple = std::tuple< Components* ... >;

		template<size_t N>
		using Nth = typename std::tuple_element<N, ComponentType>::type;;
		// using Nth = decltype( std::get<N>( std::declval<Component>() ) );
		
	protected:

		std::vector<ComponentTuple> m_components;

	public:

		System() {}
		virtual ~System() {}

		virtual void Update( float deltaTime ) override {}

		virtual void OnEntitySignatureChanged( const Entity& entity ) override final
		{
			
			// ComponentTuple componentTuple = std::make_tuple( std::forward<Components( Components* )... );

			int size = sizeof...( Components );

			Component* c = nullptr;
			c = new Nth<0>();

			std::cout << "Component Type: " << c->GetComponentType() << std::endl;

			delete c;

			c = new Nth<1>();

			std::cout << "Component Type: " << c->GetComponentType() << std::endl;

			delete c;

			c = new Nth<2>();

			std::cout << "Component Type: " << c->GetComponentType() << std::endl;
				 


			// std::cout << "Test: " << Nth<2>  << std::endl;


			std::cout << "Tuple Size: " << sizeof...(Components) << std::endl;
			ComponentTuple cTuple;
			int matchComponent = 0;
			for ( auto* c : entity.GetComponents() )
			{
				if ( c != nullptr )
				{
					std::cout << "Valid Component With Id: " << c->GetComponentId() << std::endl;

					if ( ProcessComponent<0, Components* ...>( c, 0, cTuple ) )
					{

					}


				}

			}

			// Loop through the component tuples



		}

		
		template<size_t INDEX, class ComponentClass, class ... ComponentArgs>
		bool ProcessComponent( Component* component, const uint64_t& componentType, ComponentTuple& tupleToFill )
		{
			// Complile-time check to see if class T can be converted to class B, 
				// valid for derivation check of class T from class B
			// CanConvert_From<ComponentClass, Component>();

			/*if ( component->GetComponentType() == componentType )
			{
				std::get<INDEX>( tupleToFill ) = dynamic_cast<ComponentClass*>(component);
				return true;

			}
			else
			{
				return ProcessComponent<INDEX + 1, ComponentArgs ...>( component, componentType, tupleToFill );
			}*/

			return true;

		}


		template<size_t INDEX>
		bool ProcessComponent( const Component& component, const uint64_t& componentType, ComponentTuple& tupleToFill )
		{
			return false;
		}

	};

}


#endif // !SYSTEM_H








