#pragma once
#include "logical_device.hpp"

class SyncObjects
{
public:
	SyncObjects(LogicalDevice& logicalDevice);
	~SyncObjects();

	VkFence getVkFence() { return inFlightFence; }
	VkSemaphore getImageAvailableSemaphore() { return imageAvailableSemaphore; }
	VkSemaphore getRenderFinishedSemaphore() { return renderFinishedSemaphore; }

private:
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	LogicalDevice& logicalDevice;

	void createSyncObjects();
};