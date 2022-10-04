#include "instance.hpp"
#include "debug_messenger.hpp"

// libs
#include <GLFW/glfw3.h>

// std
#include <stdexcept>
#include <iostream>
#include <vector>

Instance::Instance()
{
    createInstance();
}

Instance::~Instance()
{
    vkDestroyInstance(instance, nullptr);
}

void Instance::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
        throw std::runtime_error("validation layers requested, but not available!");

    // define application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "AE graphics app";
    appInfo.pNext = nullptr; // optional
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "AstroEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // get layers to enable
    uint32_t layerCount = 0;
    std::vector<const char*> layerNames;
    if (enableValidationLayers)
    {
        layerCount = static_cast<uint32_t>(validationLayers.size());
        layerNames = validationLayers;
    }

    // get required extensions
    std::vector<const char*> requiredExtensions = getRequiredExtensions();
    uint32_t requiredExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    if (printExtensions)
    {
        std::cout << "glfw uses extensions:\n";
        for (uint32_t i = 0; i < requiredExtensionCount; i++)
        {
            std::cout << '\t' << requiredExtensions[i] << '\n';
        }
    }

    // get DebugMessenger info
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        DebugMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
    }

    // define instance creation info 
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = &debugCreateInfo;
    createInfo.flags = 0; // optional
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = layerCount;
    createInfo.ppEnabledLayerNames = layerNames.data();
    createInfo.enabledExtensionCount = requiredExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifdef __APPLE__
    std::vector<const char*> requiredExtensions;
    for (uint32_t i = 0; i < glfwExtensionCount; i++)
        requiredExtensions.emplace_back(glfwExtensionsNames[i]);
    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
#endif

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        throw std::runtime_error("failed to create instance!");


    // get instance extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    if (printExtensions)
    {
        std::cout << "glfw uses extensions:\n";
        for (uint32_t i = 0; i < extensionCount; i++)
        {
            std::cout << '\t' << extensions[i].extensionName << " " << extensions[i].specVersion << '\n';
        }
    }
}

std::vector<const char*> Instance::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensionsNames = nullptr;
    glfwExtensionsNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // initialize vector using input iterator
    std::vector<const char*> extensions(glfwExtensionsNames, glfwExtensionsNames + glfwExtensionCount);
    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

bool Instance::checkValidationLayerSupport()
{
    // get instance layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // check if the defined layers exist
    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0) 
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}