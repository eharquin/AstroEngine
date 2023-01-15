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
	
	/*
	glfwSetWindowPosCallback(window, windowPosCallback);
	glfwSetWindowRefreshCallback(window, windowRefreshCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);
	*/
}

void AgWindow::framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	auto window = reinterpret_cast<AgWindow*>(glfwGetWindowUserPointer(glfwWindow));
	window->framebufferResized = true;
	window->width = width;
	window->height = height;
}

void AgWindow::windowPosCallback(GLFWwindow*, int xpos, int ypos)
{
	std::cout << "window pos callback" << std::endl;
}

void AgWindow::windowRefreshCallback(GLFWwindow*)
{
	std::cout << "window refresh callback" << std::endl;
}

void AgWindow::windowSizeCallback(GLFWwindow*, int width, int height)
{
	std::cout << "window size callback" << std::endl;
}