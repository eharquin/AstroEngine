#pragma once

// astro
#include "ag_swap_chain.hpp"
#include "ag_device.hpp"

// libs

// std
#include <vector>
#include <memory>
#include <cassert>


class AgRenderer
{
public:

	AgRenderer(AgWindow& agWindow, AgDevice& agDevice);
	~AgRenderer();

	AgRenderer(const AgRenderer&) = delete;
	AgRenderer& operator=(const AgRenderer&) = delete;

	// draw frame
	VkCommandBuffer beginFrame();
	void endFrame();

	// swapChain
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	uint32_t getCurrentFrame() const { return agSwapChain->getCurrentFrame(); }
	VkExtent2D getSwapChainExtent() const { return agSwapChain->getExtent(); }
	const float getAspectRatio() const { return agSwapChain->getAspectRatio(); }

	VkRenderPass getRenderPass() const { return agSwapChain->getRenderPass(); }
	bool isFrameInProgress() const { return isFrameStarted; }
	VkCommandBuffer getCurrentCommandBuffer() const 
	{
		assert(isFrameStarted && "cannot get command buffer when frame is not in progress!");
		return graphicsCommandBuffers[agSwapChain->getCurrentFrame()];
	}

private:
	// astro ref
	AgWindow& agWindow;
	AgDevice& agDevice;

	// swap chain
	std::unique_ptr<AgSwapChain> agSwapChain;
	void recreateSwapChain();
	uint32_t currentImageIndex = 0;
	bool isFrameStarted = false;

	// command buffers
	std::vector<VkCommandBuffer> graphicsCommandBuffers;

	void allocateCommandBuffers();
};

