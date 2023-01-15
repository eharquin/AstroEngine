#include "ag_renderer.hpp"

AgRenderer::AgRenderer(AgWindow& agWindow, AgDevice& agDevice)
	: agWindow(agWindow), agDevice(agDevice)
{
	agSwapChain = std::make_unique<AgSwapChain>(agDevice, agWindow.getExtent());
	allocateCommandBuffers();
}

AgRenderer::~AgRenderer()
{
	agSwapChain = nullptr;
}

VkCommandBuffer AgRenderer::beginFrame()
{
	assert(!isFrameStarted && "can't call begin while already in progress!");

	VkResult result = agSwapChain->aquireNextImage(&currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return nullptr;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}
	
	isFrameStarted = true;

	VkCommandBuffer commandBuffer = getCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr; // optional
	beginInfo.flags = 0; // optional
	beginInfo.pInheritanceInfo = nullptr; // optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin recording command buffer!");

	return commandBuffer;
}

void AgRenderer::endFrame()
{
	assert(isFrameStarted && "can't call endFrame while frame is not in progress!");
	VkCommandBuffer commandBuffer = getCurrentCommandBuffer();

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to record command buffer!");

	VkResult result = agSwapChain->submitCommandBuffers(graphicsCommandBuffers, &currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || agWindow.wasWindowResized())
	{
		agWindow.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}

	isFrameStarted = false;
}

void AgRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "can't call begin render pass if frame is not in progress!");
	assert(commandBuffer == getCurrentCommandBuffer() && "can't beging render pass on command buffer from a different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.pNext = nullptr; // optional
	renderPassInfo.renderPass = agSwapChain->getRenderPass();
	renderPassInfo.framebuffer = agSwapChain->getFramebuffer(currentImageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = agSwapChain->getExtent();

	VkClearValue clearColor = { {{0.2f, 0.3f, 0.3f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkExtent2D extent = agSwapChain->getExtent();

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)extent.width;
	viewport.height = (float)extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void AgRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "can't end render pass if frame is not in progress!");
	assert(commandBuffer == getCurrentCommandBuffer() && "can't end render pass on command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}

void AgRenderer::recreateSwapChain()
{
	VkExtent2D extent = agWindow.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = agWindow.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(agDevice.getDevice());

	std::unique_ptr<AgSwapChain> oldSwapChain = std::move(agSwapChain);
	agSwapChain = std::make_unique<AgSwapChain>(agDevice, agWindow.getExtent(), oldSwapChain->getSwapChain());

	if (!oldSwapChain->compareSwapFormat(*agSwapChain))
		throw std::runtime_error("swap chain image format has changed!"); // TODO(enzo) : setup callback fonction notifying the app that a new incompatible render pass has been created

	oldSwapChain = nullptr;
}

void AgRenderer::allocateCommandBuffers()
{
	// create graphics command buffers
	graphicsCommandBuffers.resize(agDevice.MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr; // optional
	allocInfo.commandPool = agDevice.getGraphicsCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(graphicsCommandBuffers.size());

	if (vkAllocateCommandBuffers(agDevice.getDevice(), &allocInfo, graphicsCommandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers!");
}