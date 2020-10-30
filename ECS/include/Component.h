#ifndef COMPONENT_H
#define COMPONENT_H

#include "ECS_Definitions.h"

namespace ECS {


	class Component {
		

		friend class ComponentManager;
		

		EntityId m_ownerId;

		ComponentId m_componentId;

		uint64_t m_componentType;

	public:


		explicit Component(uint64_t componentType) : m_ownerId(0), m_componentId(0), m_componentType( componentType ) {}

		virtual ~Component() {}

		const ComponentId& GetComponentId() const { return m_componentId; }

		const uint64_t& GetComponentType() const { return m_componentType; }

	};



}


#endif // !ICOMPONENT_H


