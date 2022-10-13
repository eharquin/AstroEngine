#pragma once
#include "logical_device.hpp"
#include "command_pool.hpp"
#include "frame_buffers.hpp"
#include "pipeline.hpp"

class CommandBuffer
{
public:
	CommandBuffer(LogicalDevice& logicalDevice, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, FrameBuffers& frameBuffers, CommandPool& commandPool);
	~CommandBuffer();

	void record(uint32_t frameIndex);

	VkCommandBuffer getVkCommandBuffer() { return commandBuffer; }

private:
	VkCommandBuffer commandBuffer;
	
	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	RenderPass& renderPass;
	Pipeline& pipeline;
	FrameBuffers& frameBuffers;
	CommandPool& commandPool;

	void createCommandBuffer();
};