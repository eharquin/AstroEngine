#include "surface.hpp"

// std
#include <stdexcept>

Surface::Surface(Window& window, AgInstance& instance) : window{window}, instance{instance}
{
	createSurface();
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(instance.getVkInstance(), surface, nullptr);
}

void Surface::createSurface()
{
	// use glfw lib to create a surface 
	if (glfwCreateWindowSurface(instance.getVkInstance(), window.getGLFWWindow(), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");
}
