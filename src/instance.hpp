#pragma once

// libs
#include <vulkan/vulkan.h>

// std
#include <vector>

#ifdef NDEBUG
	const bool printInfo = true;
	const bool enableValidationLayers = false;
#else
	const bool printInfo = true;
	const bool enableValidationLayers = true;
#endif // DEBUG

class Instance
{
public:
	
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	Instance();
	~Instance();

	VkInstance getVkInstance() { return instance; }


private:
	VkInstance instance;
	
	void createInstance();

	std::vector<const char*> getRequiredExtensions();

	std::vector<VkExtensionProperties> getInstanceExtensions();

	bool checkValidationLayerSupport();
};