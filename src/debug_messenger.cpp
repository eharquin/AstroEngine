#include "debug_messenger.hpp"

DebugMessenger::DebugMessenger(Instance &instance) : instance(instance)
{
    setupDebugMessenger();
}

DebugMessenger::~DebugMessenger()
{
    if (instance.enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(instance.getInstance(), debugMessenger, nullptr);
}

void DebugMessenger::setupDebugMessenger()
{
    if (!instance.enableValidationLayers) // TODO
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance.getInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug messenger!");
}
