#pragma once
// astro
#include "ag_device.hpp"

class SyncObjects
{
public:
	SyncObjects(AgDevice& agDevice);
	~SyncObjects();

	std::vector<VkSemaphore> getImageAvailableSemaphores() { return imageAvailableSemaphores; }
	std::vector<VkSemaphore> getRenderFinishedSemaphores() { return renderFinishedSemaphores; }
	std::vector<VkFence> getVkFences() { return inFlightFences; }

private:
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	// asto ref
	AgDevice& agDevice;

	void createSyncObjects();
};