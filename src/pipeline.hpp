#pragma once
#include "logical_device.hpp"
#include "swap_chain.hpp"

// std
#include <string>
#include <vector>
#include "pipeline_layout.hpp"
#include "render_pass.hpp"

class Pipeline
{
public:
	Pipeline(LogicalDevice& logicalDevice, SwapChain& swapChain, PipelineLayout& pipelineLayout, RenderPass& renderPass, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Pipeline();

	VkPipeline getVkPipeline() { return graphicsPipeline; }

private:
	VkPipeline graphicsPipeline;

	LogicalDevice& logicalDevice;
	SwapChain& swapChain;
	PipelineLayout& pipelineLayout;
	RenderPass& renderPass;

	void createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);
};