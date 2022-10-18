#include "render_pass.hpp"

RenderPass::RenderPass(LogicalDevice& logicalDevice, SwapChain& swapChain)
	: logicalDevice(logicalDevice), swapChain(swapChain)
{
	createRenderPass();
}

RenderPass::~RenderPass()
{
	cleanup();
}


void RenderPass::cleanup()
{
	vkDestroyRenderPass(logicalDevice.getVkDevice(), renderPass, nullptr);
}


void RenderPass::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.flags = 0; // optional
	colorAttachment.format = swapChain.getVkFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.flags = 0; // optional
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.inputAttachmentCount = 0; // optional
	subpass.pInputAttachments = nullptr; // optional
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pResolveAttachments = nullptr; // optional
	subpass.pDepthStencilAttachment = nullptr; // optional
	subpass.preserveAttachmentCount = 0; // optional
	subpass.pPreserveAttachments = nullptr; // optional

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dependencyFlags = 0; // optional

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.pNext = nullptr; // optional
	renderPassInfo.flags = 0; // optional
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1; 
	renderPassInfo.pDependencies = &dependency; 

	if (vkCreateRenderPass(logicalDevice.getVkDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass!");
}
