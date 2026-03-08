#ifndef ENGINEIMPL_RENDERER_HPP
#define ENGINEIMPL_RENDERER_HPP
#include "Defines.hpp"
#include "raylib.h"
struct Texture;
struct Color;
struct Rectangle;
namespace render
{
	void BeginDraw();
	void ClearScreen();
	void EndDraw();
	GAPI void DrawTexture(Texture texture2D, i32 x, i32 y, Color tint);
	GAPI void DrawSprite(Texture texture2D, Rectangle srcRect, Rectangle dstRect, Vector2 origin, float rotation, Color tint);
	GAPI void DrawRectLine(Rectangle rect, float thickness, Color tint);
}

#endif //ENGINEIMPL_RENDERER_HPP