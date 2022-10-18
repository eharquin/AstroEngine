// astro
#include "renderer.hpp"

// std
#include <iostream>

Renderer::Renderer(Window& window, LogicalDevice& logicalDevice, SwapChain& swapChain, ImageViews& imageViews, RenderPass& renderPass, FrameBuffers& frameBuffers, VertexBuffer& vertexBuffer, CommandBuffer& commandBuffer, SyncObjects& syncObjects)
	: window(window), logicalDevice(logicalDevice), swapChain(swapChain), imageViews(imageViews), renderPass(renderPass), frameBuffers(frameBuffers), vertexBuffer(vertexBuffer), commandBuffer(commandBuffer), syncObjects(syncObjects)
{
}

Renderer::~Renderer()
{
}

void Renderer::drawFrame()
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


	if (result == VK_ERROR_OUT_OF_DATE_KHR  || result == VK_SUBOPTIMAL_KHR || window.framebufferResized)
	{
		window.framebufferResized = false;
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

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::recreateSwapChain()
{
	int width = 0, height = 0;
	while (!window.getFramebufferSize(&width, &height))
		glfwWaitEvents();

	vkDeviceWaitIdle(logicalDevice.getVkDevice());

	frameBuffers.cleanup();
	renderPass.cleanup();
	imageViews.cleanup();
	swapChain.cleanup();

	swapChain.createSwapChain();
	imageViews.createImageViews();
	renderPass.createRenderPass();
	frameBuffers.createFrameBuffers();
}
