// astro
#include "command_pool.hpp"
#include "physical_device.hpp"

CommandPool::CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice)
	: physicalDevice(physicalDevice), logicalDevice(logicalDevice)
{
	createCommandPool();
}

CommandPool::~CommandPool()
{
	vkDestroyCommandPool(logicalDevice.getVkDevice(), commandPool, nullptr);
}

void CommandPool::createCommandPool()
{
	PhysicalDevice::QueueFamilyIndices queueFamilyIndices = physicalDevice.getQueueFamilyIndices();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.pNext = nullptr; // optional
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(logicalDevice.getVkDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool!");
}
