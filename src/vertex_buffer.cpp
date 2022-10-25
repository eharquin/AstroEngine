// astro
#include "vertex_buffer.hpp"

// std
#include <vector>
#include <stdexcept>

VertexBuffer::VertexBuffer(AgDevice& agDevice, size_t size)
    : agDevice(agDevice), size(size)
{
    assert(size >= 3 && "vextex size should store at least 3 vertex");
	createVertexBuffer(size);
}

VertexBuffer::~VertexBuffer()
{
    vkDestroyBuffer(agDevice.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(agDevice.getDevice(), vertexBufferMemory, nullptr);
}

void VertexBuffer::subData(const std::vector<Vertex>& vertices)
{
    // TODO : check data size
    void* data;
    vkMapMemory(agDevice.getDevice(), vertexBufferMemory, 0, size, 0, &data);
    memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
    vkUnmapMemory(agDevice.getDevice(), vertexBufferMemory);
}

void VertexBuffer::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

void VertexBuffer::createVertexBuffer(size_t size)
{
    // create buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr; // optional
    bufferInfo.flags = 0; // optional
    bufferInfo.size = sizeof(Vertex) * size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.queueFamilyIndexCount = 0; // optional
    bufferInfo.pQueueFamilyIndices = nullptr; // optional

    if (vkCreateBuffer(agDevice.getDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to create vertex buffer!");

    // get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(agDevice.getDevice(), vertexBuffer, &memRequirements);

    // allocate memory
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr; // optional
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(agDevice.getDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate vertex buffer memory!");

    // bind memory to the buffer
    vkBindBufferMemory(agDevice.getDevice(), vertexBuffer, vertexBufferMemory, 0);
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(agDevice.getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }
    throw std::runtime_error("failed to find suitable memory type!");
}
