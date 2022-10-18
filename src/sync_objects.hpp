#pragma once
#include "logical_device.hpp"

class SyncObjects
{
public:
	SyncObjects(LogicalDevice& logicalDevice);
	~SyncObjects();

	std::vector<VkSemaphore> getImageAvailableSemaphores() { return imageAvailableSemaphores; }
	std::vector<VkSemaphore> getRenderFinishedSemaphores() { return renderFinishedSemaphores; }
	std::vector<VkFence> getVkFences() { return inFlightFences; }

private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	LogicalDevice& logicalDevice;

	void createSyncObjects();
};