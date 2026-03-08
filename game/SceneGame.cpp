#include "SceneGame.hpp"
#include "AssetsManager.hpp"

#include "GMath.hpp"

constexpr int AsteroidCount = 10;

SceneGame::SceneGame(shared_ptr<ECSManager> ecsRef, Game& game)
	: ecs{ std::move(ecsRef) }, game{ game }
{
}
void SceneGame::Load()
{
	AssetsManager::LoadTexture("Astroid", "assets/Astroid.png", ToSceneId(SceneName::SceneGame));
	AssetsManager::LoadTexture("Player", "assets/Ship.png", ToSceneId(SceneName::SceneGame));
	AssetsManager::LoadTexture("Projectile", "assets/Laser.png", ToSceneId(SceneName::SceneGame));
	AssetsManager::LoadTexture("Stars", "assets/Stars.png", ToSceneId(SceneName::SceneGame));

	Vector2 screenSize = Vector2(GetScreenWidth(), GetScreenHeight());

	// Stars
	u64 starsId = ecs->CreateEntity();
	Transform2D& transform = ecs->CreateTransform2DComponent(starsId);
	transform.pos = screenSize / 2;
	transform.scale = Vector2(1.5f, 1.5f);
	ecs->CreateSpriteComponent(starsId, "Stars");

	// Asteroids
	for( int i = 0; i < AsteroidCount; i++ )
	{
		u64 asteroidId = ecs->CreateEntity();
		Transform2D& transform = ecs->CreateTransform2DComponent(asteroidId);
		transform.pos = Vector2 {
			static_cast<float>(GetRandomValue(0, GetScreenWidth())),
			static_cast<float>(GetRandomValue(0, GetScreenHeight()))
		};
		Sprite& sprite = ecs->CreateSpriteComponent(asteroidId, "Astroid");
		Rigidbody2D& body = ecs->CreateRigidbody2DComponent(
			asteroidId, transform.pos,
		    Rectangle(-sprite.tex.width * 0.5f, -sprite.tex.height * 0.5f, sprite.tex.width, sprite.tex.height)
		);
		body.layer = Rigidbody2D::Layer::Asteroid;
		body.mask = Rigidbody2D::Layer::Player;
		body.velocity = {
			static_cast<float>(GetRandomValue(-150, 150)),
			static_cast<float>(GetRandomValue(-150, 150))
		};
		body.angularVelocity = static_cast<float>(GetRandomValue(-90, 90));
		ecs->CreateScreenWrapperComponent(asteroidId, body.boundingBox);
		ecs->CreateHealthComponent(asteroidId, 2, 0.1f);
	}

	// Player
	playerId = ecs->CreateEntity();
	Transform2D& playerTransform = ecs->CreateTransform2DComponent(playerId);
	Sprite& playerSprite = ecs->CreateSpriteComponent(playerId, "Player");
	playerTransform.pos = screenSize * 0.5f;
	Rigidbody2D& playerBody = ecs->CreateRigidbody2DComponent(playerId, playerTransform.pos,
		Rectangle(-playerSprite.tex.width * 0.5f, -playerSprite.tex.height * 0.5f,
		playerSprite.tex.width, playerSprite.tex.height)
	);
	playerBody.layer = Rigidbody2D::Layer::Player;
	playerBody.mask = Rigidbody2D::Layer::None;
	playerBody.linearDamping = 1.0f;
	playerBody.angularDamping = 6.0f;
	ecs->CreateScreenWrapperComponent(playerId, playerBody.boundingBox);
	ecs->CreateHealthComponent(playerId, 300, 1.0f);
}

void SceneGame::Update(f32 dt)
{
	if (ecs->EntityExists(playerId))
	{
		const float playerMoveSpeed = 1500.0f;
		const float playerRotationSpeed = 1500.0f;

		Transform2D& playerTransform = ecs->GetComponent<Transform2D>(playerId);
		Rigidbody2D& playerBody = ecs->GetComponent<Rigidbody2D>(playerId);

		float angle = playerTransform.rotation * DEG2RAD;
		Vector2 forward { cos(angle), sin(angle) };

		nextFireTime -= dt;

		// Shoot projectile
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && nextFireTime <= 0.0f)
		{
			u64 projectileId = ecs->CreateEntity();
			Transform2D& projectileTransform = ecs->CreateTransform2DComponent(projectileId);
			projectileTransform.pos = playerTransform.pos + forward * 10.0f;
			projectileTransform.rotation = playerTransform.rotation;
			Sprite& projectileSprite = ecs->CreateSpriteComponent(projectileId, "Projectile");
			Rigidbody2D& projectileBody = ecs->CreateRigidbody2DComponent(
				projectileId, projectileTransform.pos,
				Rectangle(-projectileSprite.tex.width * 0.5f, -projectileSprite.tex.height * 0.5f,
				projectileSprite.tex.width, projectileSprite.tex.height)
			);
			projectileBody.layer = Rigidbody2D::Layer::Projectile;
			projectileBody.mask = Rigidbody2D::Layer::Asteroid;
			projectileBody.velocity = forward * 900.0f;
			ecs->CreateLifetimeComponent(projectileId, 1.0f);

			nextFireTime = 0.5f;
		}

		// Move forward
		if (IsKeyDown(KEY_W))
		{
			playerBody.velocity = playerBody.velocity + forward * playerMoveSpeed * dt;
		}

		// Rotate left and right
		if (IsKeyDown(KEY_A))
		{
			playerBody.angularVelocity -= playerRotationSpeed * dt;
		}
		if (IsKeyDown(KEY_D))
		{
			playerBody.angularVelocity += playerRotationSpeed * dt;
		}
	}

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