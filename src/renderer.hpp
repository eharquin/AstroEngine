#pragma once
#include "sync_objects.hpp"
#include "swap_chain.hpp"
#include "command_buffer.hpp"
#include "vertex_buffer.hpp"

class Renderer
{
public:
	Renderer(Window& window, LogicalDevice& logicalDevice, SwapChain& swapChain, ImageViews& imageViews, RenderPass& renderPass, FrameBuffers& frameBuffers, VertexBuffer& vertexBuffer, CommandBuffer& commandBuffer, SyncObjects& syncObjects);
	~Renderer();

	void drawFrame();

	void recreateSwapChain();

private:
	uint32_t currentFrame = 0;

	Window& window;
	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	ImageViews& imageViews;
	RenderPass& renderPass;
	FrameBuffers& frameBuffers;
	VertexBuffer& vertexBuffer;
	CommandBuffer& commandBuffer;
	SyncObjects& syncObjects;

};