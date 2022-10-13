#include "command_buffer.hpp"

CommandBuffer::CommandBuffer(LogicalDevice& logicalDevice, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, FrameBuffers& frameBuffers, CommandPool& commandPool)
	: logicalDevice(logicalDevice), swapChain(swapChain), renderPass(renderPass), pipeline(pipeline), frameBuffers(frameBuffers), commandPool(commandPool)
{
	createCommandBuffer();
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::record(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr; // optional
	beginInfo.flags = 0; // optional
	beginInfo.pInheritanceInfo = nullptr; // optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass.getVkRenderPass();
    renderPassInfo.framebuffer = frameBuffers.getVkFramebuffer()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChain.getVkExtent2D();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getVkPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChain.getVkExtent2D().width;
    viewport.height = (float)swapChain.getVkExtent2D().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChain.getVkExtent2D();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to record command buffer!");
}

void CommandBuffer::createCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.commandPool = commandPool.getVkCommandPool();;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(logicalDevice.getVkDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}
