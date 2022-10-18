#include "logical_device.hpp"
#include "utils.hpp"

// std
#include <set>

LogicalDevice::LogicalDevice(PhysicalDevice& physicalDcevice)
	: physicalDevice{physicalDcevice}
{
	createLogicalDevice();
}

LogicalDevice::~LogicalDevice()
{
	vkDestroyDevice(device, nullptr);
}

void LogicalDevice::createLogicalDevice()
{
	PhysicalDevice::QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.pNext = nullptr; // optional
		queueCreateInfo.flags = 0; // optional
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pNext = nullptr; // optional
	createInfo.flags = 0; // optional
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.enabledLayerCount = 0; // deprecated
	createInfo.ppEnabledLayerNames = nullptr; // deprecated
	createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice.deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = physicalDevice.deviceExtensions.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(physicalDevice.getVkPhysicalDevice(), &createInfo, nullptr, &device) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device!");

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}
