#pragma once
#include "logical_device.hpp"

class CommandPool
{
public:
	CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	~CommandPool();

	VkCommandPool getVkCommandPool() { return commandPool; }

private:
	VkCommandPool commandPool;

	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;

	void createCommandPool();
};