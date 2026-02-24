#include "../engine/Engine.hpp"
#include "Game.hpp"

int main()
{
	Engine engine;
	engine.Start(1280, 720, "ECS implementation", std::make_unique<Game>());
	return 0;
}