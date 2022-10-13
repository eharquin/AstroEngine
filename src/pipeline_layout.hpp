#pragma once
#include "logical_device.hpp"


class PipelineLayout
{
public:
	PipelineLayout(LogicalDevice& logicalDevice);
	~PipelineLayout();

	VkPipelineLayout getVkPipelineLayout() { return pipelineLayout; }

private:
	VkPipelineLayout pipelineLayout;

	LogicalDevice& logicalDevice;

	void createPipelineLayout();
};