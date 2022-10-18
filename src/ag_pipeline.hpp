#pragma once
// astro
#include "logical_device.hpp"
#include "ag_swap_chain.hpp"

// std
#include <string>
#include <vector>

class AgPipeline
{
public:
	AgPipeline(LogicalDevice& logicalDevice, AgSwapChain& AgSwapChain, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~AgPipeline();

	VkPipeline getVkPipeline() { return pipeline; }


private:
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
	
	// astro ref
	LogicalDevice& logicalDevice;
	AgSwapChain& agSwapChain;

	void createPipelineLayout();
	void createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);
};