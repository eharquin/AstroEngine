#pragma once
#include "window.hpp"
#include "pipeline.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "window_surface.hpp"
#include "swap_chain.hpp"

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
	Pipeline pipeline{ "shaders/vert.spv", "shaders/frag.spv" };

	Instance instance;
	DebugMessenger debugMessenger;
	WindowSurface windowSurface;
	PhysicalDevice physicalDevice;
	LogicalDevice logicalDevice;
	SwapChain swapChain;
};

