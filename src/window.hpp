#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

class Window
{
public:
	Window(int w, int h, std::string name);
	~Window();

	// not copyable
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool shouldClose() { return glfwWindowShouldClose(window); }
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
	GLFWwindow* getGLFWWindow() { return window; }
	bool wasWindowResized() { return framebufferResized; }
	void resetWindowResizedFlag() { framebufferResized = false; }

	int getFramebufferSize(int* width, int* height);

private:
	void initWindow();

	int width;
	int height;

	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	std::string windowName;
	GLFWwindow* window;
};

