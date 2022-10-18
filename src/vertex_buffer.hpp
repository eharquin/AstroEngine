#pragma once

// astro
#include "logical_device.hpp"
#include "vertex.hpp"

// libs
#include <glm/glm.hpp>

// std
#include <vector>

class VertexBuffer
{
public:
    VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, size_t size);
    ~VertexBuffer();

    void subData(const std::vector<Vertex>& vertices);

    size_t getSize() { return size; }

    void bind(VkCommandBuffer commandBuffer);

private:
    size_t size;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    PhysicalDevice& physicalDevice;
    LogicalDevice& logicalDevice;

    void createVertexBuffer(size_t size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    
};