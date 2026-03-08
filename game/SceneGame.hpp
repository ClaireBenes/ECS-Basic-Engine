#ifndef GAMEIMPL_SCENE_GAME_HPP
#define GAMEIMPL_SCENE_GAME_HPP
#include "IScene.hpp"
#include "raylib.h"
#include "ECSManager.hpp"
#include "Game.hpp"
#include <vector>
#include <array>
#include <memory>
using std::vector;
using std::array;
using std::unique_ptr;
using std::shared_ptr;
class SceneGame : public IScene
{
public:
	SceneGame(shared_ptr<ECSManager> ecsRef, Game& game);
	void Load() override;
	void Update(f32 dt) override;
	void Draw() override;
	void Unload() override;
private:
	Game& game;
	shared_ptr<ECSManager> ecs;

	u64 playerId;
	float nextFireTime = 0.0f;
};

#endif //GAMEIMPL_SCENE_GAME_HPP