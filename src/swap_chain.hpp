#pragma once
#include "logical_device.hpp"
#include "window.hpp"
#include "surface.hpp"
#include "physical_device.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>


class SwapChain
{
public:
	SwapChain(Window& window, Surface& surface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	~SwapChain();

	void cleanup();

	void createSwapChain();

	VkSwapchainKHR getVkSwapChainKHR() { return swapChain; }
	std::vector<VkImage> getVkImages() { return swapChainImages; }
	VkFormat getVkFormat() { return swapChainImageFormat; }
	VkExtent2D getVkExtent2D() { return swapChainExtent; }

private:
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	
	Window& window;
	Surface& surface;
	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;


	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};