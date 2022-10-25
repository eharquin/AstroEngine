#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

class AgWindow
{
public:
	AgWindow(int w, int h, std::string name);
	~AgWindow();

	// not copyable
	AgWindow(const AgWindow&) = delete;
	AgWindow& operator=(const AgWindow&) = delete;

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
	
	static void windowPosCallback(GLFWwindow*, int xpos, int ypos);
	static void windowRefreshCallback(GLFWwindow*);
	static void windowSizeCallback(GLFWwindow*, int width, int height);



	std::string windowName;
	GLFWwindow* window;
};

