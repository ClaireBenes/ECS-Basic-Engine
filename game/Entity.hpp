#ifndef GAMEIMPL_ENTITY_HPP
#define GAMEIMPL_ENTITY_HPP

#include <array>

#include "Components.hpp"
#include "Defines.hpp"

constexpr i32 MAX_COMPONENTS = static_cast<i32>(ComponentIndex::COUNT);

struct Entity
{
	explicit Entity(u64 idP) : id{ idP }
	{
		components.fill(-1);
	}
	u64 id;
	std::array<i32, MAX_COMPONENTS> components;
};
#endif //GAMEIMPL_ENTITY_HPP