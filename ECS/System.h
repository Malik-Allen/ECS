#ifndef SYSTEM_H
#define SYSTEM_H

#include "ISystem.h"
#include "IEntity.h"
#include "IComponent.h"

#include <tuple>
#include <vector>

namespace ECS {

	class EntityManager;


	// Unfortunately.. for now
	template <typename T>
	class System : public ISystem
	{
		

	protected:

		std::vector<T*> m_components;
		


	public:

		System(EntityManager* entityComponent) : ISystem(entityComponent) {}
		virtual ~System() {}

	private:

		// Whenever we add a new component, lets go through our lists of systems on the manager 
		
		virtual void OnComponentAdded( IComponent* component ) override final
		{

			T* componentType = dynamic_cast<T*>(component);

			if ( componentType != nullptr )
			{
				m_components.push_back( componentType );
			}
			

		}

		

	};

}


#endif // !SYSTEM_H








