#pragma once

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>
#include <stdexcept>
#include <optional>


#include "instance.hpp"
#include "window_surface.hpp"

class PhysicalDevice
{
public:

	PhysicalDevice(Instance& instance, WindowSurface& surface);
	
	VkPhysicalDevice getVkPhysicalDevice() { return physicalDevice; }

private:
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	Instance& instance;
	WindowSurface& windowSurface;

	void pickPhysicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	int rateDeviceSuitability(VkPhysicalDevice device);

	std::vector<VkPhysicalDevice> getPhysicalDevices();
};