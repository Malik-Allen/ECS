#ifndef ECS_DEFINITIONS_H
#define ECS_DEFINITIONS_H

#include <cstdint>

namespace ECS {

	using EntityId =  uint64_t;

	using ComponentId = uint64_t;

	static constexpr size_t MAX_ENTITIES	{ 1000 };

	static constexpr size_t MAX_COMPONENTS_PER_ENTITY	{ 1000 };

	static constexpr size_t MAX_SYSTEMS	{ 1000 };

	static constexpr size_t MAX_COMPONENTS	{ MAX_ENTITIES * MAX_COMPONENTS_PER_ENTITY };

	struct AutoComponentIdCounter {};

}


#endif // !ECS_DEFINITIONS_H

