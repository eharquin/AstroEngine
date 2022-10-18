#pragma once

// libs
#include <vulkan/vulkan_core.h>

// std
#include <vector>
#include <optional>
#include <iostream>

class Utils
{
public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << messageSeverity << " " << messageType << " " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
};