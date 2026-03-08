#ifndef ENGINE_GMATH_HPP
#define ENGINE_GMATH_HPP
#include "Defines.hpp"
#include "raylib.h"
#include <array>
#include <cmath>
Vector2 operator+(Vector2 lh, Vector2 rh);
Vector2 operator-(Vector2 lh, Vector2 rh);
Vector2 operator*(Vector2 v, f32 f);
Vector2 operator*(Vector2 lh, Vector2 rh);
Vector2 operator*(f32 f, Vector2 v);
Vector2 operator/(Vector2 v, f32 f);
Vector2 operator/(f32 f, Vector2 v);
#endif //ENGINE_GMATH_HPP