#pragma once
// astro
#include "ag_swap_chain.hpp"

// std
#include <string>
#include <vector>

class AgPipeline
{
public:
	AgPipeline(AgDevice& agDevice, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~AgPipeline();

	

	VkPipeline getVkPipeline() { return pipeline; }

	void bind(VkCommandBuffer commandBuffer);

private:
	VkPipeline pipeline;
	
	// astro ref
	AgDevice& agDevice;

	void createGraphicsPipeline(VkPipelineLayout pipelineLayout, VkRenderPass renderPass, const std::string& vertexFilepath, const std::string& fragmentFilepath);

	static std::vector<char> readFile(const std::string& filePath);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};