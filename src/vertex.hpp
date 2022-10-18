#pragma once

//libs
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

// std
#include <array>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};
