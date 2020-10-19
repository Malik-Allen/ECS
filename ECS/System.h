#ifndef SYSTEM_H
#define SYSTEM_H

#include "ISystem.h"
#include "IEntity.h"

#include <tuple>
#include <vector>

namespace ECS {

	class EntityManager;

	template <typename... Comps>
	class System : public ISystem
	{
		using ComponentTuple = std::tuple <Comps* ... > ;

	protected:

		std::vector<ComponentTuple> m_components;

	public:

		System(EntityManager* entityComponent) : ISystem(entityComponent) {}
		virtual ~System() {}

		virtual void OnEntityCreated(const IEntity& entity) override final;
		virtual void OnEntityRemoved(const IEntity& entity) override final;

		

	};


	template<typename ...Comps>
	inline void System<Comps...>::OnEntityCreated( const IEntity& entity )
	{

		ComponentTuple compTuple;
		size_t matchingComponents = 0;

		for ( auto compPair : entity.GetComponents() )
		{

			

		}


	}

	template<typename ...Comps>
	inline void System<Comps...>::OnEntityRemoved( const IEntity& entity )
	{
	}

}


#endif // !SYSTEM_H








