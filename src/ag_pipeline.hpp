#pragma once
// astro
#include "ag_swap_chain.hpp"

// std
#include <string>
#include <vector>

class AgPipeline
{
public:
	AgPipeline(AgDevice& agDevice, VkRenderPass renderPass, const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~AgPipeline();

	VkPipeline getVkPipeline() { return pipeline; }

private:
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
	
	// astro ref
	AgDevice& agDevice;

	void createPipelineLayout();
	void createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);

	static std::vector<char> readFile(const std::string& filePath);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};