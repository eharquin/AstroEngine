#include "app.hpp"

App::App()
{

}

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
		drawFrame();
		
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

void App::drawFrame()
{
	std::vector<VkCommandBuffer> commandBuffers = commandBuffer.getVkCommandBuffers();

	std::vector<VkSemaphore> imageAvailableSemaphores = syncObjects.getImageAvailableSemaphores();
	std::vector<VkSemaphore> renderFinishedSemaphores = syncObjects.getRenderFinishedSemaphores();
	std::vector<VkFence> inFlightFences = syncObjects.getVkFences();

	vkWaitForFences(logicalDevice.getVkDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDevice.getVkDevice(), 1, &inFlightFences[currentFrame]);

	VkSwapchainKHR swapChainTemp = swapChain.getVkSwapChainKHR();

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice.getVkDevice(), swapChainTemp, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (window.wasWindowResized())
		std::cout << "okok";

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
	{
		window.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	// Only reset the fence if we are submitting work
	vkResetFences(logicalDevice.getVkDevice(), 1, &inFlightFences[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	commandBuffer.record(currentFrame, imageIndex);

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkCommandBuffer commandBufferTemp = commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr; // optional
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBufferTemp;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		throw std::runtime_error("failed to submit draw command buffer!");

	VkSwapchainKHR swapChains[] = { swapChainTemp };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr; // optional
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(logicalDevice.getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
	{
		window.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void App::recreateSwapChain()
{
	VkExtent2D extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(logicalDevice.getVkDevice());

	frameBuffers.cleanup();
	imageViews.cleanup();
	swapChain.cleanup();

	swapChain.createSwapChain();
	imageViews.createImageViews();
	frameBuffers.createFrameBuffers();
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
}