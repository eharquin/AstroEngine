#pragma once
// astro
#include "logical_device.hpp"
#include "command_pool.hpp"
#include "ag_pipeline.hpp"
#include "vertex_buffer.hpp"

// std
#include <vector>

class CommandBuffer
{
public:
	CommandBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool);
	~CommandBuffer();

	void record(AgSwapChain& agSwapChain, AgPipeline& pipeline, VertexBuffer& vertexBuffer, uint32_t bufferIndex, uint32_t imageIndex);

	std::vector<VkCommandBuffer> getVkCommandBuffers() { return commandBuffers; }

private:
	std::vector<VkCommandBuffer> commandBuffers;
	
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;

	void createCommandBuffer();
};