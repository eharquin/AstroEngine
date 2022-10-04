#pragma once
#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"

class App
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;

	void run();

private:

	Window window{ WIDTH, HEIGHT, "window" };
	Pipeline pipeline{ "shaders/vert.spv", "shaders/frag.spv" };
	Device device{};
};

