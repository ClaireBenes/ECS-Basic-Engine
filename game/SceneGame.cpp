#include "SceneGame.hpp"
#include "AssetsManager.hpp"

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef,

	Game& game)
	: ecs{ std::move(ecsRef) }, game{ game }
{
}
void SceneGame::Load()
{
	AssetsManager::LoadTexture("Astroid", "assets/Astroid.png", 0);
	for( int i = 0; i < 10; i++ )
	{
		auto asteroid = ecs->CreateEntity();
		auto& transform = ecs->CreateTransform2DComponent(asteroid);
		transform.pos = Vector2{ (float) GetRandomValue(0, GetScreenWidth()), (float) GetRandomValue(0, GetScreenHeight()) };
		ecs->CreateSpriteComponent(asteroid, "Astroid");
	}

	//TODO : Update All Asteroid positions with ECS
	//Update the function  SystemPhysicsUpdate in ECS for that
}

void SceneGame::Update(f32 dt)
{
	ecs->UpdateScene(dt);
}

void SceneGame::Draw()
{
	ecs->PrepareDraw();
	ecs->DrawScene();
}

void SceneGame::Unload()
{
	AssetsManager::UnloadSceneTextures(ToSceneId(SceneName::SceneGame));
}