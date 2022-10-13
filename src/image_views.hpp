#pragma once
#include "logical_device.hpp"
#include <vector>
#include "swap_chain.hpp"

class ImageViews
{
public:
	ImageViews(LogicalDevice& logicalDevice, SwapChain& swapChain);
	~ImageViews();

	std::vector<VkImageView> getVkImageViews() { return swapChainImageViews; }

private:
	std::vector<VkImageView> swapChainImageViews;
	LogicalDevice& logicalDevice;
	SwapChain& swapChain;

	void createImageViews();
};