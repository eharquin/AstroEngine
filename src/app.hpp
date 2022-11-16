#pragma once
// astro
#include "ag_window.hpp"
#include "ag_instance.hpp"
#include "ag_device.hpp"

#include "ag_renderer.hpp"
#include "simple_render_system.hpp"
#include "sierpinski_triangle_system.hpp"

// libs
#include "GLFW/glfw3.h"

// std
#include <vector>


class App
{
public:
	static constexpr int WIDTH = 1240;
	static constexpr int HEIGHT = 720;

	bool space = false;
	bool prevSpace = false;

	App();
	~App();

	void run();

private:
	
	AgWindow agWindow{ WIDTH, HEIGHT, "AstroEngine window" };
	AgInstance agInstance{};
	AgDevice agDevice{ agWindow, agInstance };

	AgRenderer agRenderer{ agWindow, agDevice };
	SimpleRenderSystem simpleRenderSystem{ agDevice, agRenderer.getRenderPass() };

	std::vector<AgGameObject> gameObjects;

	AgGameObject createTriangleModel();

	void loadGameObjects();

	void processInput(GLFWwindow* window);
};