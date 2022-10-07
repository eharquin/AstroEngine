#include "app.hpp"


App::App() : instance{}, windowSurface{ window, instance }, debugMessenger{ instance }, physicalDevice { instance, windowSurface }, logicalDevice{ physicalDevice, windowSurface }, swapChain{ window, windowSurface, physicalDevice, logicalDevice }
{
}

App::~App()
{

}

void App::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();

	}
}