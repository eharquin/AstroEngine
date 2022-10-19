// astro
#include "ag_window.hpp"

// std
#include <iostream>

AgWindow::AgWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
{
	initWindow();
}

AgWindow::~AgWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int AgWindow::getFramebufferSize(int* width, int* height)
{
	glfwGetFramebufferSize(window, width, height);
	
	if (*width == 0 || *height == 0)
		return 0;

	return 1;
}

void AgWindow::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void AgWindow::framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	auto window = reinterpret_cast<AgWindow*>(glfwGetWindowUserPointer(glfwWindow));
	window->framebufferResized = true;
	window->width = width;
	window->height = height;
}

