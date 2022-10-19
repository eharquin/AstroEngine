// astro
#include "sync_objects.hpp"

// std
#include <stdexcept>

SyncObjects::SyncObjects(AgDevice& agDevice)
	: agDevice(agDevice)
{
	createSyncObjects();
}

SyncObjects::~SyncObjects()
{
	for (size_t i = 0; i < agDevice.MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(agDevice.getDevice(), imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(agDevice.getDevice(), renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(agDevice.getDevice(), inFlightFences[i], nullptr);
	}
}

void SyncObjects::createSyncObjects()
{
	imageAvailableSemaphores.resize(agDevice.MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(agDevice.MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(agDevice.MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < agDevice.MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(agDevice.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(agDevice.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(agDevice.getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}
	}
}
