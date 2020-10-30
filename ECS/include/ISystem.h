#ifndef ISYSTEM_H
#define ISYSTEM_H

namespace ECS {

	
	class ISystem
	{

	public:

		ISystem() {}
		virtual ~ISystem() {}

		virtual void Update(float deltaTime) = 0;

		virtual void OnEntitySignatureChanged( const struct Entity& entity ) = 0;


	};
	
}




#endif // !ISYSTEM_H



