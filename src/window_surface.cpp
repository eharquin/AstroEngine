#include "window_surface.hpp"

// std
#include <stdexcept>

WindowSurface::WindowSurface(Window& window, Instance& instance) : window{window}, instance{instance}
{
	createSurface();
}

WindowSurface::~WindowSurface()
{
	vkDestroySurfaceKHR(instance.getVkInstance(), surface, nullptr);
	vkDestroyInstance(instance.getVkInstance(), nullptr);
}

void WindowSurface::createSurface()
{
	if (glfwCreateWindowSurface(instance.getVkInstance(), window.getGLFWWindow(), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}
