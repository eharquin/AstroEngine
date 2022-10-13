#pragma once
#include "sync_objects.hpp"
#include "swap_chain.hpp"
#include "command_buffer.hpp"

class Renderer
{
public:
	Renderer(SyncObjects& syncObjects, SwapChain& swapChain, CommandBuffer& commandBuffer, LogicalDevice& logicalDevice);
	~Renderer();

	void drawFrame();

private:
	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	CommandBuffer& commandBuffer;
	SyncObjects& syncObjects;

};