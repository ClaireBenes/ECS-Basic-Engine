#include "EcsManager.hpp"
#include "GMath.hpp"
#include "Jobs.hpp"
#include "raymath.h"

u64 ECSManager::maxId = 0;

constexpr bool UseJobs = false;

void ECSManager::UpdateScene(f32 dt)
{
	if constexpr (UseJobs)
	{
		jobs::Execute([&]()
		{
			SystemPhysicsUpdate(dt);
			SystemScreenWrapUpdate(dt);
		});
		jobs::Execute([&]()
		{
			SystemHealthUpdate(dt);
		});
		jobs::Execute([&]()
		{
			SystemLifetimeUpdate(dt);
		});
		jobs::Wait();
	}
	else
	{
		SystemPhysicsUpdate(dt);
		SystemScreenWrapUpdate(dt);
		SystemHealthUpdate(dt);
		SystemLifetimeUpdate(dt);
	}
}
void ECSManager::DrawScene()
{
	SystemSpriteDraw();
	CleanRemovedEntities();
}
u64 ECSManager::CreateEntity()
{
	u64 newId = maxId++;
	entityIds.emplace_back(newId);
	entities.emplace_back(newId);
	return newId;
}
Transform2D& ECSManager::CreateTransform2DComponent(u64 entityId)
{
	i32 newComponentId = static_cast<i32>( transforms.size() );
	transforms.emplace_back(entityId);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
	return transforms.back();
}
Sprite& ECSManager::CreateSpriteComponent(u64 entityId, const str& texName)
{
	i32 newComponentId = static_cast<i32>( sprites.size() );
	const Texture& tex = AssetsManager::GetTexture(texName);
	sprites.emplace_back(entityId, texName, static_cast<float>( tex.width ), static_cast<float>( tex.height ));
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
	return sprites.back();
}
Rigidbody2D& ECSManager::CreateRigidbody2DComponent(u64 entityId, const Vector2& pos, const Rectangle& box)
{
	i32 newComponentId = static_cast<i32>( bodies.size() );
	bodies.emplace_back(entityId, pos, box);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
	return bodies.back();
}

ScreenWrapper& ECSManager::CreateScreenWrapperComponent(u64 entityId, const Rectangle& boundingBox)
{
	i32 newComponentId = static_cast<i32>( screenWrappers.size() );
	screenWrappers.emplace_back(entityId, boundingBox);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::ScreenWrapper);
	return screenWrappers.back();
}

Lifetime& ECSManager::CreateLifetimeComponent(u64 entityId, float time)
{
	i32 newComponentId = static_cast<i32>( lifetimes.size() );
	lifetimes.emplace_back(entityId, time);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Lifetime);
	return lifetimes.back();
}

Health& ECSManager::CreateHealthComponent(u64 entityId, int points, float invincibleTime)
{
	i32 newComponentId = static_cast<i32>( healths.size() );
	healths.emplace_back(entityId, points, invincibleTime);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Health);
	return healths.back();
}

void ECSManager::UpdateEntityWithComponent(u64 entityId, i32 newComponentId, ComponentIndex componentIndex)
{
	i32 iComponentIndex = static_cast<i32>( componentIndex );
	FindEntity(entityId).components[iComponentIndex] = newComponentId;
}

void ECSManager::RemoveEntity(u64 entityId)
{
	entitiesToRemove.push_back(entityId);
}

bool ECSManager::EntityExists(u64 entityId)
{
	auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
	return itr != entityIds.end();
}

void ECSManager::CleanRemovedEntities()
{
	for( auto entityId : entitiesToRemove )
	{
		// Transform
		RemoveEntityComponent<Transform2D>(entityId);
		// Sprites
		RemoveEntityComponent<Sprite>(entityId);
		// Rigidbodies
		RemoveEntityComponent<Rigidbody2D>(entityId);
		// ScreenWrappers
		RemoveEntityComponent<ScreenWrapper>(entityId);
		// Lifetimes
		RemoveEntityComponent<Lifetime>(entityId);
		// Healths
		RemoveEntityComponent<Health>(entityId);

		std::erase(entityIds, entityId);
		std::erase_if(entities, [=](Entity entity)
		{
			return entity.id == entityId;
		});
	}
	entitiesToRemove.clear();
}
Entity& ECSManager::FindEntity(u64 entityId)
{
	auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
	return entities.at(std::distance(entityIds.begin(), itr));
}
i32 ECSManager::FindEntityComponent(u64 entityId, ComponentIndex componentIndex)
{
	return FindEntity(entityId).components.at(static_cast<i32>( componentIndex ));
}

void ECSManager::PrepareDraw()
{
	// Update sprite positions
	for( auto& sprite : sprites )
	{
		const auto& transform = GetComponent<Transform2D>(sprite.entityId);
		sprite.dstRect.x = transform.pos.x;// + sprite.dstRect.width * sprite.origin.x;
		sprite.dstRect.y = transform.pos.y;// + sprite.dstRect.height * sprite.origin.y;
		sprite.dstRect.width = sprite.srcRect.width * transform.scale.x;
		sprite.dstRect.height = sprite.srcRect.height * transform.scale.y;
		sprite.rotation = transform.rotation;
	}
}

void ECSManager::SystemSpriteDraw()
{
	for(auto& sprite : sprites)
	{
		Color colorAlpha{ sprite.color.r, sprite.color.g, sprite.color.b, sprite.opacity };
		render::DrawSprite(
			sprite.tex,
			sprite.srcRect, sprite.dstRect,
			sprite.origin * Vector2(sprite.dstRect.width, sprite.dstRect.height),
			sprite.rotation,
			colorAlpha
		);
	}

#ifdef GDEBUG
	for(auto& body : bodies)
	{
		DrawRectangle(body.pos.x - 1, body.pos.y - 1, 2, 2, RED);
		body.DrawDebug();
	}
#endif
}

void ECSManager::SystemPhysicsUpdate(f32 dt)
{
	for (auto& body : bodies)
	{
		Transform2D& transform = GetComponent<Transform2D>(body.entityId);

		// Slowly lose velocity over time
		if (body.linearDamping != 0.0f)
		{
			body.velocity = body.velocity - body.velocity * body.linearDamping * dt;
		}
		if (body.angularDamping != 0.0f)
		{
			body.angularVelocity -= body.angularVelocity * body.angularDamping * dt;
		}

		transform.pos = transform.pos + body.velocity * dt;
		transform.rotation += body.angularVelocity * dt;
		body.pos = transform.pos;

		for (Rigidbody2D& otherBody : bodies)
		{
			if (otherBody.entityId == body.entityId)
			{
				continue;
			}
			if (body.mask != otherBody.layer)
			{
				continue;
			}

			if (CheckCollisionRecs(body.GetPositionedRectangle(), otherBody.GetPositionedRectangle()))
			{
				if (FindEntityComponent(otherBody.entityId, ComponentIndex::Health) != -1)
				{
					Health& health = GetComponent<Health>(otherBody.entityId);
					health.Damage(1);
				}

				if (body.layer == Rigidbody2D::Layer::Projectile)
				{
					RemoveEntity(body.entityId);
					break;
				}
				if (otherBody.layer == Rigidbody2D::Layer::Player)
				{
					otherBody.velocity = otherBody.velocity + Vector2Normalize(otherBody.pos - body.pos) * 250.0f;
				}
			}
		}
	}
}

void ECSManager::SystemScreenWrapUpdate(float dt)
{
	Vector2 screenSize = Vector2(GetScreenWidth(), GetScreenHeight());

	for (ScreenWrapper& wrapper : screenWrappers)
	{
		Transform2D& transform = GetComponent<Transform2D>(wrapper.entityId);

		// Right-to-left
		if (transform.pos.x > screenSize.x + wrapper.boundingBox.width * 0.5f)
		{
			transform.pos.x -= screenSize.x + wrapper.boundingBox.width;
		}
		// Left-to-right
		else if (transform.pos.x < 0.0f - wrapper.boundingBox.width * 0.5f)
		{
			transform.pos.x += screenSize.x + wrapper.boundingBox.width;
		}

		// Bottom-to-top
		if (transform.pos.y > screenSize.y + wrapper.boundingBox.height * 0.5f)
		{
			transform.pos.y -= screenSize.y + wrapper.boundingBox.height;
		}
		// Top-to-bottom
		else if (transform.pos.y < 0.0f - wrapper.boundingBox.height * 0.5f)
		{
			transform.pos.y += screenSize.y + wrapper.boundingBox.height;
		}
	}
}

void ECSManager::SystemLifetimeUpdate(float dt)
{
	for (Lifetime& lifetime : lifetimes)
	{
		lifetime.remainingTime -= dt;
		if (lifetime.remainingTime <= 0.0f)
		{
			RemoveEntity(lifetime.entityId);
		}
	}
}

void ECSManager::SystemHealthUpdate(float dt)
{
	for (Health& health : healths)
	{
		health.remainingInvincibleTime -= dt;

		Sprite& sprite = GetComponent<Sprite>(health.entityId);
		if (health.remainingInvincibleTime > 0.0f)
		{
			sprite.color = Color(255, 0, 0, 255);
		}
		else
		{
			sprite.color = WHITE;
			if (health.points <= 0)
			{
				RemoveEntity(health.entityId);
			}
		}
	}
}
