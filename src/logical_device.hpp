#pragma once

// libs
#include <vulkan/vulkan_core.h>

#include "physical_device.hpp"

class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice& physicalDevice, Surface& surface);
	~LogicalDevice();

	VkDevice getVkDevice() { return device; }
	VkQueue getGraphicsQueue() { return graphicsQueue; }
	VkQueue getPresentQueue() { return presentQueue; }

private:
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	PhysicalDevice& physicalDevice;
	Surface& surface;

	void createLogicalDevice();
};