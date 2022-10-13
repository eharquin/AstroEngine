#include "renderer.hpp"

Renderer::Renderer(SyncObjects& syncObjects, SwapChain& swapChain, CommandBuffer& commandBuffer, LogicalDevice& logicalDevice)
	: logicalDevice(logicalDevice), swapChain(swapChain), commandBuffer(commandBuffer), syncObjects(syncObjects)
{

}

Renderer::~Renderer()
{

}

void Renderer::drawFrame()
{
	VkFence inFlightFence = syncObjects.getVkFence();
	vkWaitForFences(logicalDevice.getVkDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(logicalDevice.getVkDevice(), 1, &inFlightFence);

	VkSwapchainKHR swapChainTemp = swapChain.getVkSwapChainKHR();

	uint32_t imageIndex;
	vkAcquireNextImageKHR(logicalDevice.getVkDevice(), swapChainTemp, UINT64_MAX, syncObjects.getImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(commandBuffer.getVkCommandBuffer(), 0);
	commandBuffer.record(imageIndex);

	VkSemaphore waitSemaphores[] = { syncObjects.getImageAvailableSemaphore() };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkCommandBuffer commandBufferTemp = commandBuffer.getVkCommandBuffer();

	VkSemaphore signalSemaphores[] = { syncObjects.getRenderFinishedSemaphore() };

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

	if (vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS)
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

	vkQueuePresentKHR(logicalDevice.getPresentQueue(), &presentInfo);
}
