#ifndef ENGINEIMPL_WINDOW_HPP
#define ENGINEIMPL_WINDOW_HPP

#include "Defines.hpp"
class Window
{
public:
	Window(i32 width, i32 height, str title);
	~Window();
	static bool ShouldClose();
private:
	i32 width;
	i32 height;
	str title;
};

#endif //ENGINEIMPL_WINDOW_HPP