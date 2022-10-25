#pragma once
// astro
#include "ag_device.hpp"
#include "vertex.hpp"

// libs
#include <glm/glm.hpp>

// std
#include <vector>

class VertexBuffer
{
public:
    VertexBuffer(AgDevice& agDevice, size_t size);
    ~VertexBuffer();

    void subData(const std::vector<Vertex>& vertices);
    size_t getSize() { return size; }
    void bind(VkCommandBuffer commandBuffer);

private:
    size_t size;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    // astro ref
    AgDevice& agDevice;

    void createVertexBuffer(size_t size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};