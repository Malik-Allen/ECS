#ifndef ISYSTEM_H
#define ISYSTEM_H

namespace ECS {

	class EntityManager;
	class IComponent;

	// System 
	class ISystem
	{

		// Active statr of this System
		bool				m_isActive;

		// Entity Manager containing all the Components that this System will perform its actions on
		EntityManager*		m_entityManager;

	public:

		explicit ISystem(EntityManager* entityManager) : m_isActive(true), m_entityManager(entityManager) {}
		virtual ~ISystem() {}


		virtual void Update(float deltaTime) = 0;

		// Assign the state of this System
		void SetEnabled(bool isActive) { this->m_isActive = isActive; }

		// Returns the active state of this System
		bool IsActive() const { return m_isActive; }


		// TODO: Remember Systems will be the last thing created, but it can get a little tricky if components are added after an entity has been created
			// Think about a way to tackle this issue
			// Should be Templated 
		virtual void OnComponentAdded( IComponent* component ) = 0;
		// virtual void OnEntityRemoved( const IEntity& entity ) = 0;

	};
	
}




#endif // !ISYSTEM_H



