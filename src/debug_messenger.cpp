#include "debug_messenger.hpp"
#include "utils.hpp"

DebugMessenger::DebugMessenger(Instance& instance) : instance(instance)
{
    setupDebugMessenger();
}

DebugMessenger::~DebugMessenger()
{
    if(enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(instance.getVkInstance(), debugMessenger, nullptr);
}

void DebugMessenger::setupDebugMessenger()
{
    if (enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        Utils::populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(instance.getVkInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
            throw std::runtime_error("failed to set up debug messenger!");
    }
}
