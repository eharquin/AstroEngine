#include "debug_messenger.hpp"
#include "utils.hpp"

DebugMessenger::DebugMessenger(Instance &instance) : instance(instance)
{
    setupDebugMessenger();
}

DebugMessenger::~DebugMessenger()
{
    DestroyDebugUtilsMessengerEXT(instance.getVkInstance(), debugMessenger, nullptr);
}

void DebugMessenger::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    Utils::populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance.getVkInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug messenger!");
}
