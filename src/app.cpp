#include "app.hpp"

App::App() {}

App::~App() {}

void App::run()
{
	std::srand(static_cast <unsigned> (std::time(0))); // use current time as seed for random generator

	vertexBuffer.subData(vertices);

	while (!window.shouldClose())
	{
		// input
		// -----
		processInput(window.getGLFWWindow());
		renderer.drawFrame();
		
		glfwPollEvents();
	}
	vkDeviceWaitIdle(logicalDevice.getVkDevice());
}

glm::vec3 App::getRandomColor()
{

	float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	return { r, g, b };
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
		std::cout << sierpinskiIterations;
		sierpinskiIterations++;
		std::vector<Vertex> vertices = generateSierpinskiTriangleVertex(sierpinskiIterations, up, left, right);
		vertexBuffer.subData(vertices);
	}
}