#pragma once

// astro
#include "logical_device.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>

class AgSwapChain
{
public:
	AgSwapChain(Window& window, Surface& surface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	~AgSwapChain();

	VkSwapchainKHR getSwapChain() { return swapChain; }
	VkExtent2D getExtent() { return swapChainExtent; }

	VkRenderPass getRenderPass() { return renderPass; }

	VkFramebuffer getFramebuffer(int i) { return swapChainFramebuffers[i]; }

private:
	// swap chain 
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	uint32_t imageCount = 0;
	
	// image views
	std::vector<VkImageView> swapChainImageViews;
	
	// render pass
	VkRenderPass renderPass;

	// framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;

	// astro ref
	Window& window;
	Surface& surface;
	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;

	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createFrameBuffers();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};