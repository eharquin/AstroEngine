#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "instance.hpp"
#include "window.hpp"

class WindowSurface
{
public:
	WindowSurface(Window& window, Instance& instance);
	~WindowSurface();

	VkSurfaceKHR getVkSurfaceKHR() { return surface; }

private:
	VkSurfaceKHR surface;
	Window& window;
	Instance& instance;

	void createSurface();
};