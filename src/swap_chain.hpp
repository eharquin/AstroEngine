#pragma once
#include "logical_device.hpp"
#include "window.hpp"
#include "window_surface.hpp"
#include "physical_device.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>


class SwapChain
{
public:
	SwapChain(Window& window, WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	~SwapChain();

private:
	VkSwapchainKHR swapChain;
	Window& window;
	WindowSurface& windowSurface;
	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapChain();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};