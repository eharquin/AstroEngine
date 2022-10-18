// astro
#include "window.hpp"

// std
#include <iostream>

Window::Window(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
{
	initWindow();
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int Window::getFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(window, width, height);
	
	if (*width == 0 || *height == 0)
		return 0;

	return 1;
}

void Window::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->framebufferResized = true;
	window->width = width;
	window->height = height;
}

