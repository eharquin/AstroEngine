#include "app.hpp"

void App::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();

	}
}