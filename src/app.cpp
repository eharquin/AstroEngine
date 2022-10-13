#include "app.hpp"


App::App() {}

App::~App() {}

void App::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();
		renderer.drawFrame();
	}

	vkDeviceWaitIdle(logicalDevice.getVkDevice());
}