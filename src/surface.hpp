#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "ag_instance.hpp"
#include "window.hpp"

class Surface
{
public:
	Surface(Window& window, AgInstance& instance);
	~Surface();

	VkSurfaceKHR getVkSurfaceKHR() { return surface; }

private:
	VkSurfaceKHR surface;

	Window& window;
	AgInstance& instance;

	void createSurface();
};