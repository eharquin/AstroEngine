#include "ag_command_buffer.hpp"
#include <stdexcept>

AgCommandBuffer::AgCommandBuffer(AgDevice& agDevice)
	: agDevice(agDevice)
{
    createCommandPool();
	createCommandBuffer();
}

AgCommandBuffer::~AgCommandBuffer()
{
    vkDestroyCommandPool(agDevice.getDevice(), commandPool, nullptr);
}

void AgCommandBuffer::record(AgSwapChain* agSwapChain, AgPipeline* agPipeline, VertexBuffer& vertexBuffer, uint32_t bufferIndex, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr; // optional
	beginInfo.flags = 0; // optional
	beginInfo.pInheritanceInfo = nullptr; // optional

	if (vkBeginCommandBuffer(commandBuffers[bufferIndex], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.renderPass = agSwapChain->getRenderPass();
    renderPassInfo.framebuffer = agSwapChain->getFramebuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = agSwapChain->getExtent();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[bufferIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[bufferIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, agPipeline->getVkPipeline());

    VkExtent2D extent = agSwapChain->getExtent();

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[bufferIndex], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffers[bufferIndex], 0, 1, &scissor);

    vertexBuffer.bind(commandBuffers[bufferIndex]);

    vkCmdDraw(commandBuffers[bufferIndex], static_cast<uint32_t>(vertexBuffer.getSize()), 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[bufferIndex]);

    if (vkEndCommandBuffer(commandBuffers[bufferIndex]) != VK_SUCCESS)
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
    commandBuffers.resize(3);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
    allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(agDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}
