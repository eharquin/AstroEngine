#pragma once

// libs
#include <vulkan/vulkan_core.h>

#include "physical_device.hpp"

class LogicalDevice
{
public:
	LogicalDevice(PhysicalDevice& physicalDevice, WindowSurface& windowSurface);
	~LogicalDevice();

	VkDevice getVkDevice() { return device; }

private:
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	PhysicalDevice& physicalDevice;
	WindowSurface& windowSurface;

	void createLogicalDevice();

};