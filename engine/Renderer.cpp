#include <cmath>
#include "Renderer.hpp"
#include "raylib.h"
namespace render
{
	void BeginDraw()
	{
		BeginDrawing();
	}
	void ClearScreen()
	{
		ClearBackground(BLACK);
	}
	void EndDraw()
	{
		EndDrawing();
	}
	void DrawTexture(Texture texture, i32 x, i32 y, Color tint)
	{
		::DrawTexture(texture, x, y, tint);
	}
	void DrawSprite(Texture texture2D, Rectangle srcRect, Rectangle dstRect, Vector2 origin, float rotation, Color tint)
	{
		::DrawTexturePro(texture2D, srcRect, dstRect, origin, rotation, tint);
	}

	void DrawRectLine(Rectangle rect, float thickness, Color tint)
	{
		::DrawRectangleLinesEx(rect, thickness, tint);
	}
}
