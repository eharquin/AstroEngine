#include "physical_device.hpp"
#include "utils.hpp"

// std
#include <iostream>
#include <set>


PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) : instance(instance), surface(surface)
{
	pickPhysicalDevice();
}


void PhysicalDevice::pickPhysicalDevice()
{
	// get physical devices
	std::vector<VkPhysicalDevice> devices = getPhysicalDevices();

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	// get device properties
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	

	// get device features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	Utils::QueueFamilyIndices indices = Utils::findQueueFamilies(device, surface.getVkSurfaceKHR());
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		Utils::SwapChainSupportDetails swapChainSupport = Utils::querySwapChainSupport(device, surface.getVkSurfaceKHR());
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(Utils::deviceExtensions.begin(), Utils::deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}



int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device) // TODO
{
	// not implemented yet
	return 0;
}



std::vector<VkPhysicalDevice> PhysicalDevice::getPhysicalDevices()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.getVkInstance() , &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.getVkInstance(), &deviceCount, devices.data());

	return devices;
}
