#pragma once
// astro
#include "ag_pipeline.hpp"
#include "vertex_buffer.hpp"

// std
#include <vector>

class AgCommandBuffer
{
public:
	AgCommandBuffer(AgDevice& agDevice);
	~AgCommandBuffer();

	void record(AgSwapChain* agSwapChain, AgPipeline* agPipeline, VertexBuffer& vertexBuffer, uint32_t bufferIndex, uint32_t imageIndex);

	std::vector<VkCommandBuffer> getVkCommandBuffers() { return commandBuffers; }

private:
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	
	// astro ref
	AgDevice& agDevice;

	void createCommandPool();
	void createCommandBuffer();
};