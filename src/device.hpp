#pragma once
#include "window.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"

class Device
{
public:
	Device();
	~Device();

	// not copyable or movable
	Device(const Device&) = delete;
	void operator=(const Device&) = delete;
	Device(Device&&) = delete;
	Device& operator=(Device&&) = delete;

private:
	//void createInstance();
	//void setupDebugMessenger();
	//void createSurface();
	//void pickPhysicalDevice();
	//void createLogicalDevice();
	//void createCommandPool();
	Instance instance;
	DebugMessenger debugMessenger;
};