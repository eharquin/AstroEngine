#include "ag_command_buffer.hpp"
#include <stdexcept>

AgCommandBuffer::AgCommandBuffer(AgDevice& agDevice, uint32_t imageCount)
	: agDevice(agDevice), imageCount(imageCount)
{
    createCommandPool();
	createCommandBuffer();
}

AgCommandBuffer::~AgCommandBuffer()
{
    vkDestroyCommandPool(agDevice.getDevice(), commandPool, nullptr);
}

void AgCommandBuffer::record(AgSwapChain* agSwapChain, AgPipeline* agPipeline, VertexBuffer& vertexBuffer, uint32_t currentFrame, uint32_t imageIndex)
{
    //std::cout << "current frame : " << currentFrame << " " << "image index" << imageIndex << std::endl;
	
    VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr; // optional
	beginInfo.flags = 0; // optional
	beginInfo.pInheritanceInfo = nullptr; // optional

	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.pNext = nullptr; // optional
    renderPassInfo.renderPass = agSwapChain->getRenderPass();
    renderPassInfo.framebuffer = agSwapChain->getFramebuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = agSwapChain->getExtent();

    VkClearValue clearColor = { {{0.2f, 0.3f, 0.3f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, agPipeline->getVkPipeline());

    VkExtent2D extent = agSwapChain->getExtent();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

    vertexBuffer.bind(commandBuffers[currentFrame]);

    vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(vertexBuffer.getSize()), 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[currentFrame]);

    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
        throw std::runtime_error("failed to record command buffer!");
}

void AgCommandBuffer::createCommandPool()
{
    AgDevice::QueueFamilyIndices queueFamilyIndices = agDevice.findQueueFamilies(agDevice.getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext = nullptr; // optional
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(agDevice.getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        throw std::runtime_error("failed to create command pool!");
}

void AgCommandBuffer::createCommandBuffer()
{
    // resize to swapChain images size
    commandBuffers.resize(agDevice.MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
    allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(agDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}
