#pragma once

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>
#include <stdexcept>
#include <optional>


#include "ag_instance.hpp"
#include "surface.hpp"

class PhysicalDevice
{
public:
	typedef struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	typedef struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	PhysicalDevice(AgInstance& instance, Surface& surface);
	
	VkPhysicalDevice getVkPhysicalDevice() { return physicalDevice; }
	QueueFamilyIndices getQueueFamilyIndices() { return queueFamilyIndices; }
	SwapChainSupportDetails getSwapChainSupportDetails() { return swapChainSupport; }

	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

private:
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	QueueFamilyIndices queueFamilyIndices;
	SwapChainSupportDetails swapChainSupport;

	AgInstance& instance;
	Surface& surface;

	void pickPhysicalDevice();

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	int rateDeviceSuitability(VkPhysicalDevice device);

	std::vector<VkPhysicalDevice> getPhysicalDevices();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);
};