#ifndef ISYSTEM_H
#define ISYSTEM_H

namespace ECS {

	
	class ISystem
	{

		ISystem(const ISystem&) = delete;
		ISystem& operator=(const ISystem&) = delete;
		ISystem(ISystem&&) = delete;
		ISystem& operator=(ISystem&&) = delete;

		friend class SystemManager;

		// Unique Identifier Managed by the SystemManager
		uint64_t		m_systemManagerId;

		// Unique Identifier Managed by the SystemManager
		uint64_t		m_systemId;

	public:

		explicit ISystem(uint64_t systemID) : m_systemManagerId(0), m_systemId(systemID) {}
		virtual ~ISystem() {}

		virtual void Update(float deltaTime) = 0;

		virtual void OnEntitySignatureChanged( const struct Entity& entity ) = 0;


	};
	
}




#endif // !ISYSTEM_H



