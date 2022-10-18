#include "sync_objects.hpp"

SyncObjects::SyncObjects(LogicalDevice& logicalDevice)
	: logicalDevice(logicalDevice)
{
	createSyncObjects();
}

SyncObjects::~SyncObjects()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(logicalDevice.getVkDevice(), imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice.getVkDevice(), renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice.getVkDevice(), inFlightFences[i], nullptr);
	}
}

void SyncObjects::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(logicalDevice.getVkDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice.getVkDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice.getVkDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}
	}
}
