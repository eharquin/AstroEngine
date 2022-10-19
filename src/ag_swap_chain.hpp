#pragma once

// astro
#include "ag_device.hpp"

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>
#include <memory>

class AgSwapChain
{
public:
	AgSwapChain(AgDevice& agDevice, VkExtent2D windowExtent);
	AgSwapChain(AgDevice& agDevice, VkExtent2D windowExtent, std::shared_ptr<AgSwapChain> oldSwapChain);

	~AgSwapChain();

	VkSwapchainKHR getSwapChain() { return swapChain; }
	VkExtent2D getExtent() { return swapChainExtent; }

	VkRenderPass getRenderPass() { return renderPass; }

	VkFramebuffer getFramebuffer(int i) { return swapChainFramebuffers[i]; }

private:
	// swap chain 
	VkExtent2D windowExtent;
	VkSwapchainKHR swapChain;
	std::shared_ptr<AgSwapChain> oldSwapChain;
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
	AgDevice& agDevice;

	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createFrameBuffers();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};