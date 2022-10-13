#include "sync_objects.hpp"

SyncObjects::SyncObjects(LogicalDevice& logicalDevice)
	: logicalDevice(logicalDevice)
{
	createSyncObjects();
}

SyncObjects::~SyncObjects()
{
	vkDestroySemaphore(logicalDevice.getVkDevice(), imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice.getVkDevice(), renderFinishedSemaphore, nullptr);
	vkDestroyFence(logicalDevice.getVkDevice(), inFlightFence, nullptr);
}

void SyncObjects::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(logicalDevice.getVkDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(logicalDevice.getVkDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(logicalDevice.getVkDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphores!");
	}
}
