#pragma once

// astro
#include "ag_pipeline.hpp"
#include "ag_game_object.hpp"

// libs
#include "GLFW/glfw3.h"

// std
#include <vector>
#include <memory>


class SimpleRenderSystem
{
public:
	SimpleRenderSystem(AgDevice& agDevice, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout);
	~SimpleRenderSystem();
	
	struct PushConstants
	{
		glm::mat4 data;
		glm::mat4 transform_matrix;
	};


	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<AgGameObject> gameObjects, VkDescriptorSet descriptorSet);

private:
	// pipeline
	VkPipelineLayout pipelineLayout;
	std::unique_ptr<AgPipeline> agPipeline;

	// astro ref
	AgDevice& agDevice;

	void createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout);
	void createPipeline(VkRenderPass renderpass);
};

