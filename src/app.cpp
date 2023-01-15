// astro
#include "app.hpp"

// std
#include <iostream>
#include <memory>
#include <chrono>

App::App()
{
	// create global descriptor pool
	VkDescriptorPoolSize uboPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, agDevice.MAX_FRAMES_IN_FLIGHT };
	VkDescriptorPoolSize samplerPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, agDevice.MAX_FRAMES_IN_FLIGHT };
	std::array<VkDescriptorPoolSize, 2> poolSizes{ uboPoolSize , samplerPoolSize };

	globalPool = std::make_unique<AgDescriptorPool>(agDevice, poolSizes.size(), poolSizes.data(), 0, agDevice.MAX_FRAMES_IN_FLIGHT);

	loadGameObject();
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

	VkDescriptorSetLayoutBinding uboBinding{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 , VK_SHADER_STAGE_VERTEX_BIT, nullptr };
	VkDescriptorSetLayoutBinding samplerBinding{ 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 , VK_SHADER_STAGE_FRAGMENT_BIT, nullptr };
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboBinding, samplerBinding };

	AgDescriptorSetLayout globalSetLayout(agDevice, bindings.size(), bindings.data());

	std::vector<VkDescriptorSetLayout> layouts(agDevice.MAX_FRAMES_IN_FLIGHT, globalSetLayout.getLayout());
	std::vector<VkDescriptorSet> globalDescriptorsSets(agDevice.MAX_FRAMES_IN_FLIGHT);



	for (int i = 0; i < agDevice.MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->getDescriptorBufferInfo();

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = gameObjects.at(0).model->getTextureImageView();
		imageInfo.sampler = gameObjects.at(0).model->getTextureSampler();

		globalPool->allocateDescriptor(globalSetLayout.getLayout(), globalDescriptorsSets[i]);
		AgDescriptorSetWriter writer{ globalSetLayout };
		writer.writeBuffer(0, &bufferInfo);
		writer.writeImage(1, &imageInfo);
		writer.overwrite(globalDescriptorsSets[i]);
	}

	SimpleRenderSystem simpleRenderSystem{ agDevice, agRenderer.getRenderPass(), globalSetLayout.getLayout()};

	AgCamera camera;
	KeyboardController keyboardController;
	auto viewerObject = AgGameObject::createGameObject();

	auto currentTime = std::chrono::high_resolution_clock::now();

	while (!agWindow.shouldClose())
	{
		glfwPollEvents();

		processInput(agWindow.getGLFWWindow());

		auto newTime = std::chrono::high_resolution_clock::now();

		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - newTime).count();

		currentTime = newTime;
		
		// set camera view
		//camera.setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
		//camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));
		keyboardController.moveInPlaneXZ(agWindow.getGLFWWindow(), time, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

		// set camera projection
		float aspect = agRenderer.getAspectRatio();
		//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
		camera.setPerspectiveProjection(glm::radians(50.0f), agRenderer.getAspectRatio(), 0.1f, 10.0f);

		if (VkCommandBuffer commandBuffer = agRenderer.beginFrame())
		{
			int frameIndex = agRenderer.getCurrentFrame();

			// update
			UniformBufferObject ubo{};
			ubo.model = glm::mat4(1.0f);
			ubo.view = camera.getView();
			ubo.proj = camera.getProjection();
			uboBuffers[frameIndex]->write(&ubo);

			if (objectsRotation)
			{
				for (auto& object : gameObjects)
				{
					object.transform.rotation.y = glm::mod(object.transform.rotation.y + 0.001f, glm::two_pi<float>());
					object.transform.rotation.x = glm::mod(object.transform.rotation.x + 0.0005f, glm::two_pi<float>());
				}
			}

			// render
			agRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, globalDescriptorsSets[frameIndex]);
			agRenderer.endSwapChainRenderPass(commandBuffer);
			agRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(agDevice.getDevice());
}

void App::loadGameObject()
{
	std::vector<AgModel::Vertex> vertices{
	  {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // top left
	  {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // top right
	  {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // bottom right
	  {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // bottom left

	  {{-1.0f, -1.0f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // top left
	  {{1.0f, -1.0f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},  // top right
	  {{1.0f, 1.0f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // bottom right
	  {{-1.0f, 1.0f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // bottom left
	};

	std::vector<uint32_t> indices{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
	};

	//gameObject.model = std::make_shared<AgModel>(agDevice, vertices, indices, "./textures/viking_room.png");
	//gameObject.model = std::make_shared<AgModel>(agDevice, "./models/viking_room.obj", "./textures/viking_room.png");

	std::shared_ptr<AgModel> model = testCubeFace();
	auto cube = AgGameObject::createGameObject();
	cube.model = model;
	cube.transform.translation = { .0f, .0f, 2.5f };
	cube.transform.scale = { .5f, .5f, .5f };
	gameObjects.push_back(std::move(cube));
}

std::unique_ptr<AgModel> App::testCubeFace()
{
	std::vector<AgModel::Vertex> vertices{
		{ {-.5f, -.5f, -.5f}, { .9f, .9f, .9f }, {0.0f, 0.0f}},
		{ {-.5f, -.5f, .5f}, {.9f, .9f, .9f}, {1.0f, 0.0f}},
		{ {-.5f, .5f, .5f}, {.9f, .9f, .9f}, {1.0f, 1.0f} },
		{ {-.5f, .5f, -.5f}, {.9f, .9f, .9f}, {0.0f, 1.0f} },

		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}, {0.0f, 0.0f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}, {1.0f, 0.0f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}, {1.0f, 1.0f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}, {0.0f, 1.0f}},

		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}, {0.0f, 0.0f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}, {1.0f, 0.0f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}, {1.0f, 1.0f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}, {0.0f, 1.0f}},

		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}, {0.0f, 0.0f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}, {1.0f, 0.0f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}, {1.0f, 1.0f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}, {0.0f, 1.0f}},

		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}, {0.0f, 0.0f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}, {1.0f, 0.0f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}, {1.0f, 1.0f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}, {0.0f, 1.0f}},

		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}, {0.0f, 0.0f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}, {1.0f, 0.0f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}, {1.0f, 1.0f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}, {0.0f, 1.0f}},
	};

	std::vector<uint32_t> indices{
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23,
	};

	return std::make_unique<AgModel>(agDevice, vertices, indices, "./textures/bejam.jpg");
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
		objectsRotation = !objectsRotation;

		
	}
}

