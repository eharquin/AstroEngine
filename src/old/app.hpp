#pragma once
// astro
#include "ag_window.hpp"
#include "ag_instance.hpp"
#include "ag_device.hpp"

#include "ag_renderer.hpp"
#include "simple_render_system.hpp"
#include "ag_descriptors.hpp"



#include "sierpinski_triangle_system.hpp"

// libs
#include "GLFW/glfw3.h"

// std
#include <vector>


class App
{
public:

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

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

	std::vector<AgGameObject> gameObjects;

	std::unique_ptr<AgDescriptorPool> globalPool;



	AgGameObject createTriangleModel();

	void loadGameObjects();

	void processInput(GLFWwindow* window);
};