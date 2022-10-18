#include "command_buffer.hpp"

CommandBuffer::CommandBuffer(LogicalDevice& logicalDevice, SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline, FrameBuffers& frameBuffers, CommandPool& commandPool, VertexBuffer& vertexBuffer)
	: logicalDevice(logicalDevice), swapChain(swapChain), renderPass(renderPass), pipeline(pipeline), frameBuffers(frameBuffers), commandPool(commandPool), vertexBuffer(vertexBuffer)
{
	createCommandBuffer();
}

CommandBuffer::~CommandBuffer()
{
}

void CommandBuffer::record(uint32_t bufferIndex, uint32_t imageIndex)
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
    renderPassInfo.renderPass = renderPass.getVkRenderPass();
    renderPassInfo.framebuffer = frameBuffers.getVkFramebuffer()[imageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChain.getVkExtent2D();

    VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[bufferIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[bufferIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getVkPipeline());

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChain.getVkExtent2D().width;
    viewport.height = (float)swapChain.getVkExtent2D().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[bufferIndex], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChain.getVkExtent2D();
    vkCmdSetScissor(commandBuffers[bufferIndex], 0, 1, &scissor);

    vertexBuffer.bind(commandBuffers[bufferIndex]);

    vkCmdDraw(commandBuffers[bufferIndex], static_cast<uint32_t>(vertexBuffer.getSize()), 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[bufferIndex]);

    if (vkEndCommandBuffer(commandBuffers[bufferIndex]) != VK_SUCCESS)
        throw std::runtime_error("failed to record command buffer!");
}

void CommandBuffer::createCommandBuffer()
{
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.commandPool = commandPool.getVkCommandPool();;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(logicalDevice.getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}
