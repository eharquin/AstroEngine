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
#include "renderer.hpp"
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

	Renderer renderer{ window, logicalDevice, swapChain, imageViews, renderPass, frameBuffers, vertexBuffer, commandBuffer, syncObjects };


	std::vector<Vertex> generateSierpinskiTriangleVertex(int n, Vertex up, Vertex left, Vertex right)
	{
		std::vector<Vertex> vertices;
		//vertices.push_back(up);
		//vertices.push_back(left);
		//vertices.push_back(right);

		if (n == 1)
		{
			glm::vec3 color = getRandomColor();
			up.color = color;
			left.color = color;
			right.color = color;
			vertices.push_back(up);
			vertices.push_back(left);
			vertices.push_back(right);
			return vertices;
		}

		// up
		Vertex upUp = up;
		Vertex upLeft = { (left.pos + up.pos) / 2.0f, glm::vec3(1.0f) };
		Vertex upRight = { (right.pos + up.pos) / 2.0f, glm::vec3(1.0f) };
		auto upVertices = generateSierpinskiTriangleVertex(n - 1, upUp, upLeft, upRight);
		vertices.insert(vertices.begin(), upVertices.begin(), upVertices.end());

		// left
		Vertex leftUp = upLeft;
		Vertex leftLeft = left;
		Vertex leftRight = { (right.pos + left.pos) / 2.0f, glm::vec3(1.0f) };
		auto leftVertices = generateSierpinskiTriangleVertex(n - 1, leftUp, leftLeft, leftRight);
		vertices.insert(vertices.begin(), leftVertices.begin(), leftVertices.end());

		// right
		Vertex rightUp = upRight;
		Vertex rightLeft = leftRight;
		Vertex rightRight = right;
		auto rightVertices = generateSierpinskiTriangleVertex(n - 1, rightUp, rightLeft, rightRight);
		vertices.insert(vertices.begin(), rightVertices.begin(), rightVertices.end());
	}

	static glm::vec3 getRandomColor();

	void processInput(GLFWwindow* window);

	//const std::vector<Vertex> vertices = {
	//	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	//	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	//	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	//};

	const Vertex up = { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} };
	const Vertex left = { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} };
	const Vertex right = { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} };

	const std::vector<Vertex> vertices = { up, left, right };

};

