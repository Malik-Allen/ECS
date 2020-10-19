#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "IEntity.h"

namespace ECS {

	typedef std::string ComponentId;

	// Component Base Class
	class IComponent {

		friend class ComponentManager;

	protected:

		// The entity id of the owning entity
		EntityId			m_ownerId;

		// The id for this component
		ComponentId			m_componentId;

		// The active state of this component
		bool				m_isActive;


	public:

		IComponent() :
			m_ownerId(""),
			m_componentId(""),
			m_isActive(true)
		{}

		virtual ~IComponent() {}

		// Sets the active state of this component
		void SetEnabled(bool isActive) { m_isActive = isActive; }

		// Returns the active state of this 
		bool IsEnabled() const { return m_isActive; }

		// Returns this component's id
		const ComponentId& GetComponentId() const { return m_componentId; }

		// Returns this component's parent entity's id
		const EntityId& GetOwnerId() const { return m_ownerId; }

	};

}

#endif // !ICOMPONENT_H


