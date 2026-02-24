#include <algorithm>
#include "Engine.hpp"
#include "raylib.h"
#include "Window.hpp"
#include "IGame.hpp"
void Engine::Start(i32 windowWidth, i32 windowHeight, const str& gameNameP,

	unique_ptr<IGame>&& gameP) noexcept

{
	gameName = gameNameP;
	game = std::move(gameP);
	Window window{ windowWidth, windowHeight, gameName };
	SetTargetFPS(60);
	game->Load();
	while( !Window::ShouldClose() )
	{
		f32 dt = std::min(GetFrameTime(), 0.0166f);
		game->Update(dt);
		render::BeginDraw();
		render::ClearScreen();
		game->Draw();
		DrawFPS(50, 50);
		render::EndDraw();
	}
	game->Unload();
}