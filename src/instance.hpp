#pragma once

// libs
#include <vulkan/vulkan.h>

// std
#include <vector>

class Instance
{
public:
#ifdef NDEBUG
	const bool printExtensions = false;
	const bool enableValidationLayers = false;
#else
	const bool printExtensions = true;
	const bool enableValidationLayers = true;
#endif // DEBUG
	
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	Instance();
	~Instance();

	VkInstance getInstance() { return instance; }

private:
	VkInstance instance;
	
	void createInstance();

	std::vector<const char*> getRequiredExtensions();

	// TODO
	//std::vector<VkExtensionProperties> getInstanceExtensions();

	//void printExtensions(std::vector<const char*>);

	bool checkValidationLayerSupport();
};