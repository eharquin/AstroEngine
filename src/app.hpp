#pragma once

// astro
#include "ag_window.hpp"
#include "ag_instance.hpp"
#include "ag_device.hpp"
#include "ag_swap_chain.hpp"
#include "ag_pipeline.hpp"
#include "ag_command_buffer.hpp"
#include "vertex.hpp"
#include "vertex_buffer.hpp"

// libs
#include "GLFW/glfw3.h"

// std
#include <vector>
#include <memory>


class App
{
public:
	static constexpr int WIDTH = 1240;
	static constexpr int HEIGHT = 720;

	int sierpinskiIterations = 2;

	bool space = false;
	bool prevSpace = false;

	App();
	~App();


	void run();

private:

	uint32_t currentFrame = 0;

	AgWindow window{ WIDTH, HEIGHT, "window" };

	AgInstance instance{};

	AgDevice agDevice{ window, instance };

	std::unique_ptr<AgSwapChain> agSwapChain;
	std::unique_ptr<AgPipeline> agPipeline;

	VertexBuffer vertexBuffer{ agDevice, 1024 * 100 };
	
	std::unique_ptr<AgCommandBuffer> agCommandBuffer;

	void drawFrame();

	void recreateSwapChain();

	std::vector<Vertex> generateSierpinskiTriangleVertex(int n, Vertex up, Vertex left, Vertex right);

	static glm::vec3 getRandomColor();

	void processInput(GLFWwindow* window);

	const Vertex up = { {0.0f, -1.0f}, {1.0f, 0.0f, 0.0f} };
	const Vertex left = { {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} };
	const Vertex right = { {-1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} };
	//const std::vector<Vertex> vertices = { up, left, right };

};

