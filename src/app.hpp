#pragma once

// astro
#include "window.hpp"
#include "pipeline.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"
#include "image_views.hpp"
#include "render_pass.hpp"
#include "pipeline_layout.hpp"
#include "frame_buffers.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "sync_objects.hpp"
#include "vertex.hpp"
#include "vertex_buffer.hpp"

// libs
#include "GLFW/glfw3.h"

// std
#include <vector>

class App
{
public:
	static constexpr int WIDTH = 1240;
	static constexpr int HEIGHT = 720;

	int sierpinskiIterations = 1;

	bool space = false;
	bool prevSpace = false;

	App();
	~App();


	void run();

private:

	uint32_t currentFrame = 0;

	Window window{ WIDTH, HEIGHT, "window" };

	Instance instance{};
	DebugMessenger debugMessenger{ instance }; // TODO nest DebugMessenger inside Instance object (VS problem with mutual inclusion)
	Surface surface{ window, instance };
	PhysicalDevice physicalDevice{ instance, surface };

	LogicalDevice logicalDevice{ physicalDevice, surface };
	SwapChain swapChain{ window, surface, physicalDevice, logicalDevice };
	ImageViews imageViews{ logicalDevice, swapChain };
	PipelineLayout pipelineLayout{ logicalDevice };
	RenderPass renderPass{ logicalDevice, swapChain };
	Pipeline pipeline{ logicalDevice, swapChain, pipelineLayout, renderPass, "shaders/vert.spv", "shaders/frag.spv" };
	FrameBuffers frameBuffers{ logicalDevice, swapChain, imageViews, renderPass };
	CommandPool commandPool{ surface, physicalDevice, logicalDevice };
	VertexBuffer vertexBuffer{ physicalDevice, logicalDevice, 1024 * 100 };
	CommandBuffer commandBuffer{ logicalDevice, swapChain, renderPass, pipeline, frameBuffers, commandPool , vertexBuffer};
	SyncObjects syncObjects{ logicalDevice };


	void drawFrame();

	void recreateSwapChain();

	std::vector<Vertex> generateSierpinskiTriangleVertex(int n, Vertex up, Vertex left, Vertex right);

	static glm::vec3 getRandomColor();

	void processInput(GLFWwindow* window);

	const Vertex up = { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} };
	const Vertex left = { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} };
	const Vertex right = { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} };
	const std::vector<Vertex> vertices = { up, left, right };

};

