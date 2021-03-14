#ifndef COMPONENT_H
#define COMPONENT_H

#include "ECS_Definitions.h"

namespace ECS {

	
	class Component {
		
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		friend class ComponentManager;
		
		// The owning entity's id
		EntityId				m_ownerId;

		// The unique identier for this component
		ComponentId				m_componentId;

		// The unique identier for this component, used only by the component manager
		uint64_t				m_componentManagerId;

		// This component's unique type identifier
		uint64_t				m_componentType;


	public:


		explicit Component(uint64_t componentType) : m_ownerId(0), m_componentId(0), m_componentManagerId(0), m_componentType( componentType ) {}

		virtual ~Component() {}

		inline const EntityId& GetOwnerEntity() const { return m_ownerId; }

		inline const ComponentId& GetComponentId() const { return m_componentId; }

		inline const uint64_t& GetComponentType() const { return m_componentType; }

	};



}


#endif // !ICOMPONENT_H


