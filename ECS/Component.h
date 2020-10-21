#ifndef COMPONENT_H
#define COMPONENT_H

#include "IComponent.h"

namespace ECS {


	class Component : public IComponent {

	public:

		explicit Component(ComponentType type) : IComponent(type) {}	// Makes the type Manadatory for all SubClass Components

		virtual ~Component() {}

	};



}


#endif // !ICOMPONENT_H


