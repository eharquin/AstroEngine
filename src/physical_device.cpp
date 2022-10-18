#include "physical_device.hpp"
#include "utils.hpp"

// std
#include <iostream>
#include <set>
#include <map>


PhysicalDevice::PhysicalDevice(AgInstance& instance, Surface& surface) : instance(instance), surface(surface)
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

	queueFamilyIndices = findQueueFamilies(physicalDevice);
	swapChainSupport = querySwapChainSupport(physicalDevice);
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

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

	PhysicalDevice::SwapChainSupportDetails details = querySwapChainSupport(device);
	PhysicalDevice::QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
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

PhysicalDevice::QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices{};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// seeking for a queue family with graphics operations support
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.getVkSurfaceKHR(), &presentSupport);

		if (presentSupport)
			indices.presentFamily = i;

		if (indices.isComplete())
			break;

		i++;
	}

	return indices;
}

PhysicalDevice::SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.getVkSurfaceKHR(), &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.getVkSurfaceKHR(), &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.getVkSurfaceKHR(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.getVkSurfaceKHR(), &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.getVkSurfaceKHR(), &presentModeCount, details.presentModes.data());
	}

	return details;
}