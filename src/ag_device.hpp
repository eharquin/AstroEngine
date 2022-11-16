#pragma once
// astro
#include "ag_window.hpp"
#include "ag_instance.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "vulkan/vulkan_core.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// std
#include <optional>

class AgDevice
{
public:
	AgDevice(AgWindow& agWindow, AgInstance& agInstance);
	~AgDevice();

	const int MAX_FRAMES_IN_FLIGHT = 2;

	// surface
	VkSurfaceKHR getSurface() { return surface; }

	// physical device
	typedef struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;
		bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value(); }
	};
	typedef struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device); // TODO : verify if it's important to reask 
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device); // TODO : check when it should be reask
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }

	// device
	VkDevice getDevice() { return device; }
	VkQueue getGraphicsQueue() { return graphicsQueue; }
	VkQueue getPresentQueue() { return presentQueue; }
	VkQueue getTransferQueue() { return transferQueue; }

	// command pools
	VkCommandPool getGraphicsCommandPool() { return graphicsCommandPool; }
	VkCommandPool getTransferCommandPool() { return transferCommandPool; }

	// buffer utils
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

private:
	// astro ref
	AgWindow& agWindow;
	AgInstance& agInstance;

	// surface
	VkSurfaceKHR surface;
	void createSurface();

	// physical device
	VkPhysicalDevice physicalDevice;
	void pickPhysicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	std::vector<VkPhysicalDevice> getPhysicalDevices();

	// device
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue transferQueue;
	void createDevice();

	// command pools
	VkCommandPool graphicsCommandPool;
	VkCommandPool transferCommandPool;
	void createCommandPools();
};

