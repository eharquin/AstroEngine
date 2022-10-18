#include "physical_device.hpp"
#include "utils.hpp"

// std
#include <iostream>
#include <set>
#include <map>


PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) : instance(instance), surface(surface)
{
	pickPhysicalDevice();
}

void PhysicalDevice::pickPhysicalDevice()
{
	// get physical devices
	std::vector<VkPhysicalDevice> devices = getPhysicalDevices();

	// Use an ordered map to automatically sort candidates by increasing score
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : devices)
	{
		int score = rateDeviceSuitability(device);
		candidates.insert(std::make_pair(score, device));
	}

	// Check if the best candidate is suitable at all
	if (candidates.rbegin()->first > 0)
		physicalDevice = candidates.rbegin()->second;
	else
		throw std::runtime_error("failed to find a suitable GPU!");
}

// DEPRECATED
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
	if (extensionsSupported)
	{
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

int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device) {
	
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

	// Application can't function without these fonctionalities
	if (!(indices.isComplete() && extensionsSupported && swapChainAdequate))
		return 0;

	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	return score;
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
