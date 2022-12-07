// astro
#include "simple_render_system.hpp"

// std
#include <memory>


SimpleRenderSystem::SimpleRenderSystem(AgDevice& agDevice, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout)
	: agDevice(agDevice)
{
	createPipelineLayout(descriptorSetLayout);
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(agDevice.getDevice(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<AgGameObject> gameObjects, VkDescriptorSet descriptorSet)
{
	agPipeline->bind(commandBuffer);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

	for (auto gameObject : gameObjects)
	{
		gameObject.model->bind(commandBuffer);

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = gameObject.transform();

		glm::mat4 tranform_matrix = projection * view * model;

		PushConstants pushConstants{};
		pushConstants.data = glm::mat4( 0.0f );
		pushConstants.transform_matrix = tranform_matrix;

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

		gameObject.model->draw(commandBuffer);
	}
}


 void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout)
{
	// define push constants
	VkPushConstantRange pushConstantsInfo{};
	pushConstantsInfo.offset = 0;
	pushConstantsInfo.size = sizeof(PushConstants);
	// push constants onlly accesible from vertex shaders
	pushConstantsInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	// create pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr; // optional
	pipelineLayoutInfo.flags = 0; // optional
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantsInfo;

	if (vkCreatePipelineLayout(agDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout!");
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
{
	agPipeline = std::make_unique<AgPipeline>(agDevice, pipelineLayout, renderPass, "shaders/vert.spv", "shaders/frag.spv");
}