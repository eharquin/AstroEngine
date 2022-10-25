// astro
#include "app.hpp"

// std
#include <iostream>

App::App()
{
	agSwapChain = std::make_unique<AgSwapChain>(agDevice, window.getExtent());
	agPipeline = std::make_unique<AgPipeline>(agDevice, agSwapChain->getRenderPass(), "shaders/vert.spv", "shaders/frag.spv");
	agCommandBuffer = std::make_unique<AgCommandBuffer>(agDevice, agSwapChain->getImageCount());
}

App::~App() {}

void App::run()
{
	//std::srand(static_cast <unsigned> (std::time(0))); // use current time as seed for random generator

	std::vector<Vertex> vertices = generateSierpinskiTriangleVertex(sierpinskiIterations, up, left, right);

	vertexBuffer.subData(vertices);

	while (!window.shouldClose())
	{
		// input
		// -----
		glfwPollEvents();

		processInput(window.getGLFWWindow());
		drawFrame();
		
	}
	vkDeviceWaitIdle(agDevice.getDevice());
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
		sierpinskiIterations++;
		std::vector<Vertex> vertices = generateSierpinskiTriangleVertex(sierpinskiIterations, up, left, right);
		std::cout << vertices.size() << std::endl;
		vertexBuffer.subData(vertices);
	}
}

void App::drawFrame()
{

	uint32_t imageIndex;
	VkResult result = agSwapChain->aquireNextImage(&imageIndex);
	
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	agCommandBuffer->record(agSwapChain.get(), agPipeline.get(), vertexBuffer, agSwapChain->currentFrame, imageIndex);
	
	std::vector<VkCommandBuffer> commandBuffers = agCommandBuffer->getVkCommandBuffers();

	result = agSwapChain->submitCommandBuffers(commandBuffers, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
	{
		window.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

}

void App::recreateSwapChain()
{
	VkExtent2D extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(agDevice.getDevice());

	VkSwapchainKHR oldSwapChain = agSwapChain->getSwapChain();
	//agSwapChain = nullptr;
	//agPipeline = nullptr;

	agSwapChain = std::make_unique<AgSwapChain>(agDevice, window.getExtent(), oldSwapChain);

	// TODO : recreate only if the render pass is no longer compatible
	agPipeline = std::make_unique<AgPipeline>(agDevice, agSwapChain->getRenderPass(), "shaders/vert.spv", "shaders/frag.spv");
}

std::vector<Vertex> App::generateSierpinskiTriangleVertex(int n, Vertex up, Vertex left, Vertex right)
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

	return vertices;
}