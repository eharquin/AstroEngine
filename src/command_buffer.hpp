#pragma once

// astro
#include "logical_device.hpp"
#include "command_pool.hpp"
#include "frame_buffers.hpp"
#include "pipeline.hpp"
#include "vertex_buffer.hpp"

// std
#include <vector>

class CommandBuffer
{
public:
	CommandBuffer(LogicalDevice& logicalDevice, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, FrameBuffers& frameBuffers, CommandPool& commandPool, VertexBuffer& vertexBuffer);
	~CommandBuffer();

	void record(uint32_t bufferIndex, uint32_t imageIndex);

	std::vector<VkCommandBuffer> getVkCommandBuffers() { return commandBuffers; }

private:
	std::vector<VkCommandBuffer> commandBuffers;
	
	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	RenderPass& renderPass;
	Pipeline& pipeline;
	FrameBuffers& frameBuffers;
	CommandPool& commandPool;
	VertexBuffer& vertexBuffer;

	void createCommandBuffer();
};