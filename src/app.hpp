#pragma once
#include "window.hpp"
#include "pipeline.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"
#include "image_views.hpp"
#include "render_pass.hpp"
#include "pipeline_layout.hpp"
#include "frame_buffers.hpp"
#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "sync_objects.hpp"
#include "renderer.hpp"

class App
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	App();
	~App();


	void run();

private:

	Window window{ WIDTH, HEIGHT, "window" };

	Instance instance{};
	DebugMessenger debugMessenger{ instance }; // TODO nest DebugMessenger inside Instance object (VS problem with mutual inclusion)
	Surface surface{ window, instance };
	PhysicalDevice physicalDevice{ instance, surface };

	LogicalDevice logicalDevice{ physicalDevice, surface };
	SwapChain swapChain{ window, surface, physicalDevice, logicalDevice };
	ImageViews imageViews{ logicalDevice, swapChain };
	PipelineLayout pipelineLayout{ logicalDevice };
	RenderPass renderPass{ logicalDevice, swapChain };
	Pipeline pipeline{ logicalDevice, swapChain, pipelineLayout, renderPass, "shaders/vert.spv", "shaders/frag.spv" };
	FrameBuffers frameBuffers{ logicalDevice, swapChain, imageViews, renderPass };
	CommandPool commandPool{ surface, physicalDevice, logicalDevice };
	CommandBuffer commandBuffer{ logicalDevice, swapChain, renderPass, pipeline, frameBuffers, commandPool };
	SyncObjects syncObjects{ logicalDevice };

	Renderer renderer{ syncObjects, swapChain, commandBuffer, logicalDevice };

};

