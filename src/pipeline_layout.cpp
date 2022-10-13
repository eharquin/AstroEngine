#include "pipeline_layout.hpp"

PipelineLayout::PipelineLayout(LogicalDevice& logicalDevice)
	: logicalDevice(logicalDevice)
{
	createPipelineLayout();
}

PipelineLayout::~PipelineLayout()
{
	vkDestroyPipelineLayout(logicalDevice.getVkDevice(), pipelineLayout, nullptr);
}

void PipelineLayout::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr; // optional
	pipelineLayoutInfo.flags = 0; // optional
	pipelineLayoutInfo.setLayoutCount = 0; // optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // optional

	if (vkCreatePipelineLayout(logicalDevice.getVkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");
}
