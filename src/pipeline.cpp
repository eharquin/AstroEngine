#include "pipeline.hpp"

// std
#include <stdexcept>

#include "shader.hpp"

Pipeline::Pipeline(LogicalDevice& logicalDevice, SwapChain& swapChain, PipelineLayout& pipelineLayout, RenderPass& renderPass, const std::string& vertexFilepath, const std::string& fragmentFilepath)
	: logicalDevice(logicalDevice), swapChain(swapChain), pipelineLayout(pipelineLayout), renderPass(renderPass)
{
	createGraphicsPipeline(vertexFilepath, fragmentFilepath);
}

Pipeline::~Pipeline()
{
	vkDestroyPipeline(logicalDevice.getVkDevice(), graphicsPipeline, nullptr);
}

void Pipeline::createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	Shader vertexShader { logicalDevice, vertexFilepath };
	Shader fragmentShader{ logicalDevice, fragmentFilepath };

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.pNext = nullptr; // optional
	vertShaderStageInfo.flags = 0; // optional
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertexShader.getVkShaderModule();
	vertShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = nullptr; // optional

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.pNext = nullptr; // optional
	vertShaderStageInfo.flags = 0; // optional
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragmentShader.getVkShaderModule();
	fragShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = nullptr; // optional

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.pNext = nullptr; // optional
	dynamicState.flags = 0; // optional
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr; // optional
	vertexInputInfo.flags = 0; // optional
	vertexInputInfo.vertexBindingDescriptionCount = 0; // optional
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0; // optional
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // optional

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.pNext = nullptr; // optional
	inputAssembly.flags = 0; // optional
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain.getVkExtent2D().width;
	viewport.height = (float)swapChain.getVkExtent2D().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain.getVkExtent2D();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.pNext = nullptr; // optional
	viewportState.flags = 0; // optional
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;


	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.pNext = nullptr; // optional
	rasterizer.flags = 0; // optional
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // optional
	rasterizer.depthBiasClamp = 0.0f; // optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // optional
	rasterizer.lineWidth = 1.0f;
	
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.pNext = nullptr; // optional
	multisampling.flags = 0; // optional
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.minSampleShading = 1.0f; // optional
	multisampling.pSampleMask = nullptr; // optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // optional
	multisampling.alphaToOneEnable = VK_FALSE; // optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // optional
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.pNext = nullptr; // optional
	colorBlending.flags = 0; // optional
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // optional
	colorBlending.blendConstants[1] = 0.0f; // optional
	colorBlending.blendConstants[2] = 0.0f; // optional
	colorBlending.blendConstants[3] = 0.0f; // optional

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = nullptr; // optional
	pipelineInfo.flags = 0; // optional
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pTessellationState = nullptr; // optional
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout.getVkPipelineLayout();
	pipelineInfo.renderPass = renderPass.getVkRenderPass();
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
	pipelineInfo.basePipelineIndex = -1; // optional

	if (vkCreateGraphicsPipelines(logicalDevice.getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline!");
	
	// shader are delete here
}