#ifndef GAMEIMPL_COMPONENTS_HPP
#define GAMEIMPL_COMPONENTS_HPP
#include <optional>
#include <utility>
#include <vector>
#include <memory>
#include "raylib.h"
#include "Defines.hpp"
#include "AssetsManager.hpp"
#include "Renderer.hpp"

class ECSManager;

using std::vector;

enum class ComponentIndex
{
	Transform2D,
	Sprite,
	Rigidbody2D,
	ScreenWrapper,
	Lifetime,
	Health,

	COUNT,
};

struct Transform2D
{
	explicit Transform2D(u64 entityIdP) : entityId{ entityIdP }
	{
	}
	u64 entityId;
	Vector2 pos{ 0.0f, 0.0f };
	float rotation{ 0.0f };
	Vector2 scale{ 1.0f, 1.0f };
};

struct Sprite
{
	explicit Sprite(u64 entityIdP, const str& textNameP, float width, float height) :
		entityId{ entityIdP },
		srcRect{ 0, 0, width, height },
		dstRect{ 0, 0, width, height },
		texName{ textNameP },
		tex{ AssetsManager::GetTexture(textNameP) }
	{
	}
	u64 entityId;
	u8 opacity{ 255 };
	Rectangle srcRect{ 0, 0, 1, 1 };
	Rectangle dstRect{ 0, 0, 1, 1 };
	Vector2 origin{ 0.5f, 0.5f };
	float rotation{ 0 };
	str texName;
	Texture tex;
	Color color = WHITE;
};

struct Rigidbody2D
{
	enum class Layer
	{
		None,
		Player,
		Asteroid,
		Projectile,
	};

	explicit Rigidbody2D(u64 entityIdP, const Vector2& pos, const Rectangle& box) :
		entityId{ entityIdP },
		pos{ pos },
		boundingBox{ box }
	{
	}

	u64 entityId;
	Vector2 pos{ 0.0f, 0.0f };
	Vector2 velocity{ 0.0f, 0.0f };
	float linearDamping{ 0.0f };
	float angularDamping{ 0.0f };
	float angularVelocity{ 0.0f };
	Rectangle boundingBox{ 0, 0, 1, 1 };
	Layer layer = Layer::None;
	Layer mask = Layer::None;

	[[nodiscard]] Rectangle GetPositionedRectangle() const
	{
		return Rectangle{ pos.x + boundingBox.x, pos.y + boundingBox.y, boundingBox.width, boundingBox.height };
	}
	[[nodiscard]] f32 GetRealX() const
	{
		return pos.x + boundingBox.x;
	}
	[[nodiscard]] f32 GetRealY() const
	{
		return pos.y + boundingBox.y;
	}
#ifdef GDEBUG
	void DrawDebug() const
	{
		const Rectangle box{ pos.x + boundingBox.x,
			pos.y + boundingBox.y,
			boundingBox.width, boundingBox.height };

		render::DrawRectLine(box, 2, BLUE);
	}
#endif
};

struct ScreenWrapper
{
	explicit ScreenWrapper(u64 entityId, Rectangle boundingBox)
		: entityId{ entityId },
		  boundingBox{ boundingBox }
	{
	}

	u64 entityId;
	Rectangle boundingBox;
};

struct Lifetime
{
	explicit Lifetime(u64 entityId, float time)
		: entityId{ entityId },
		  remainingTime{ time }
	{
	}

	u64 entityId;
	float remainingTime;
};

struct Health
{
	explicit Health(u64 entityId, int points, float invincibleTime)
		: entityId{ entityId },
		  points{ points },
		  invincibleTime{ invincibleTime }
	{
	}

	u64 entityId;
	int points;
	float invincibleTime;
	float remainingInvincibleTime = 0.0f;

	void Damage(int amount)
	{
		if (remainingInvincibleTime > 0.0f)
		{
			return;
		}

		points -= amount;
		remainingInvincibleTime = invincibleTime;
	}
};

// Utils
struct Collision2D
{
	Collision2D(u64 entityId, Rectangle currentBox, Vector2 velocity,
		u64 otherId, Rectangle otherCurrentBox, Vector2 otherVelocity) :
		entityId{ entityId }, currentBox{ currentBox }, velocity{ velocity },
		otherId{ otherId }, otherCurrentBox{ otherCurrentBox }, otherVelocity{ otherVelocity }
	{
	}
	u64 entityId;
	Rectangle currentBox;

	Vector2 velocity;
	u64 otherId;
	Rectangle otherCurrentBox;
	Vector2 otherVelocity;
};
#endif //GAMEIMPL_COMPONENTS_HPP