#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include "IEntity.h"
#include "ComponentTypes.h"

namespace ECS {

	typedef uint32_t ComponentId;
	typedef Component_Type ComponentType;

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

		// The type of component
		ComponentType		m_type;

	public:

		IComponent() :
			m_ownerId(0),
			m_componentId(0),
			m_isActive(true),
			m_type(Component_Type::Default)
		{}

		IComponent(ComponentType type) :	
			m_ownerId( 0 ),
			m_componentId( 0 ),
			m_isActive( true ),
			m_type( type )
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


