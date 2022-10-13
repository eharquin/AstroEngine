#include "frame_buffers.hpp"

FrameBuffers::FrameBuffers(LogicalDevice& logicalDevice, SwapChain& swapChain, ImageViews& imageViews, RenderPass& renderPass)
    : logicalDevice(logicalDevice), swapChain(swapChain), imageViews(imageViews), renderPass(renderPass)
{
	createFrameBuffers();
}


FrameBuffers::~FrameBuffers()
{
    for (auto framebuffer : swapChainFramebuffers)
        vkDestroyFramebuffer(logicalDevice.getVkDevice(), framebuffer, nullptr);
}


void FrameBuffers::createFrameBuffers()
{
	std::vector<VkImage> swapChainImages = swapChain.getVkImages();
	swapChainFramebuffers.resize(swapChainImages.size());

    std::vector<VkImageView> swapChainImageViews = imageViews.getVkImageViews();

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
    {
        VkImageView attachments[] = { swapChainImageViews[i] };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.getVkRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.getVkExtent2D().width;
        framebufferInfo.height = swapChain.getVkExtent2D().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(logicalDevice.getVkDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer!");
    }
}