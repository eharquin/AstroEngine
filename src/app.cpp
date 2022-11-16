// astro
#include "app.hpp"

// std
#include <iostream>
#include <memory>

App::App()
{
	loadGameObjects();
}

App::~App() {}

AgGameObject App::createTriangleModel()
{
	std::vector<AgModel::Vertex> vertices{
	  {{0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	  {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	  {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}} };

	std::vector<uint32_t> indices{};

	AgGameObject triangleObject = AgGameObject::createGameObject();

	triangleObject.model = std::make_shared<AgModel>(agDevice, vertices, indices);
	
	return triangleObject;
}

void App::run()
{

	while (!agWindow.shouldClose())
	{
		// input
		// -----
		glfwPollEvents();

		//processInput(agWindow.getGLFWWindow());


		if (VkCommandBuffer commandBuffer = agRenderer.beginFrame())
		{
			agRenderer.beginSwapChainRenderPass(commandBuffer);

			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);

			agRenderer.endSwapChainRenderPass(commandBuffer);
			agRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(agDevice.getDevice());
}

void App::loadGameObjects()
{
	auto triangle1 = createTriangleModel();

	gameObjects.push_back(triangle1);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void App::processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	prevSpace = space;
	space = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

	if (space && !prevSpace)
	{

	}
}