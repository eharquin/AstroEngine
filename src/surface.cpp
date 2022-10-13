#include "surface.hpp"

// std
#include <stdexcept>

Surface::Surface(Window& window, Instance& instance) : window{window}, instance{instance}
{
	createSurface();
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(instance.getVkInstance(), surface, nullptr);
}

void Surface::createSurface()
{
	if (glfwCreateWindowSurface(instance.getVkInstance(), window.getGLFWWindow(), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}
