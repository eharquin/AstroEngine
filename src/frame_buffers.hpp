#pragma once
#include "swap_chain.hpp"
#include "image_views.hpp"

// std
#include <vector>
#include "render_pass.hpp"

class FrameBuffers
{
public:
	FrameBuffers(LogicalDevice& logicalDevice, SwapChain& swapChain, ImageViews& imageViews, RenderPass& renderPass);
	~FrameBuffers();

	std::vector<VkFramebuffer> getVkFramebuffer() { return swapChainFramebuffers; }

private:
	std::vector<VkFramebuffer> swapChainFramebuffers;

	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	ImageViews& imageViews;
	RenderPass& renderPass;

	void createFrameBuffers();
};