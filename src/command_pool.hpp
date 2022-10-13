#pragma once
#include "logical_device.hpp"

class CommandPool
{
public:
	CommandPool(Surface& surface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	~CommandPool();

	VkCommandPool getVkCommandPool() { return commandPool; }

private:
	VkCommandPool commandPool;

	Surface& surface;
	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;

	void createCommandPool();
};