// astro
#include "app.hpp"

// std
#include <iostream>
#include <memory>
#include <chrono>

App::App()
{
	// create global descriptor pool
	VkDescriptorPoolSize poolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 };
	globalPool = std::make_unique<AgDescriptorPool>(agDevice, 1, &poolSize, 0, agDevice.MAX_FRAMES_IN_FLIGHT);

	loadGameObjects();
}

App::~App() {}


void App::run()
{
	// create ubo buffers
	std::vector<std::unique_ptr<AgBuffer>> uboBuffers{ agDevice.MAX_FRAMES_IN_FLIGHT };
	for (int i = 0; i < uboBuffers.size(); i++)
	{
		uboBuffers[i] = std::make_unique<AgBuffer>(
			agDevice,
			sizeof(UniformBufferObject),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		uboBuffers[i]->map();
	}

	VkDescriptorSetLayoutBinding binding{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 , VK_SHADER_STAGE_VERTEX_BIT, nullptr };
	AgDescriptorSetLayout globalSetLayout(agDevice, 1, &binding);

	std::vector<VkDescriptorSetLayout> layouts(agDevice.MAX_FRAMES_IN_FLIGHT, globalSetLayout.getLayout());
	std::vector<VkDescriptorSet> globalDescriptorsSets(agDevice.MAX_FRAMES_IN_FLIGHT);


	for (int i = 0; i < agDevice.MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->getDescriptorBufferInfo();

		globalPool->allocateDescriptor(globalSetLayout.getLayout(), globalDescriptorsSets[i]);
		AgDescriptorSetWriter writer{ globalSetLayout };
		writer.writeBuffer(0, bufferInfo);
		writer.overwrite(globalDescriptorsSets[i]);
	}

	SimpleRenderSystem simpleRenderSystem{ agDevice, agRenderer.getRenderPass(), globalSetLayout.getLayout()};

	while (!agWindow.shouldClose())
	{
		// input
		// -----
		glfwPollEvents();

		//processInput(agWindow.getGLFWWindow());

		if (VkCommandBuffer commandBuffer = agRenderer.beginFrame())
		{
			int frameIndex = agRenderer.getCurrentFrame();
			// update
			static auto startTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			UniformBufferObject ubo{};
			ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.view = glm::mat4{ 1.0f };
			ubo.proj = glm::mat4{ 1.0f };

			uboBuffers[frameIndex]->write(&ubo);

			// render
			agRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, globalDescriptorsSets[frameIndex]);
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

AgGameObject App::createTriangleModel()
{
	std::vector<AgModel::Vertex> vertices{
	  {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // top left
	  {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // top right
	  {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // bottom right
	  {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}  // bottom left
	};

	std::vector<uint32_t> indices{
		0, 1, 2,
		0, 2, 3
	};

	AgGameObject triangleObject = AgGameObject::createGameObject();

	triangleObject.model = std::make_shared<AgModel>(agDevice, vertices, indices);
	triangleObject.scale = glm::vec3(0.5f);
	
	return triangleObject;
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