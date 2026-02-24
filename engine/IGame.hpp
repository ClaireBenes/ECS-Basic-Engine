#ifndef ENGINEIMPL_IGAME_HPP
#define ENGINEIMPL_IGAME_HPP
#include "Defines.hpp"
class IGame
{
public:
	virtual ~IGame() = default;
	virtual void Load() = 0;
	virtual void Update(f32 dt) = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;
};
#endif //ENGINEIMPL_IGAME_HPP