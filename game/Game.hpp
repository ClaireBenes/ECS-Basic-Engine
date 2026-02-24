#ifndef GAMEIMPL_GAME_HPP
#define GAMEIMPL_GAME_HPP
#include "../engine/IGame.hpp"
#include "IScene.hpp"
#include "EcsManager.hpp"
#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
class Game : public IGame
{
public:
	void Load() override;
	void Update(f32 dt) override;
	void Draw() override;
	void Unload() override;
	void AddScene(unique_ptr<IScene> newScene);
	void RemoveCurrentScene();
	void SwitchScene(unique_ptr<IScene> newScene);

private:
	vector<unique_ptr<IScene>> sceneStack{};
	shared_ptr<ECSManager> ecs;
};

#endif //GAMEIMPL_GAME_HPP