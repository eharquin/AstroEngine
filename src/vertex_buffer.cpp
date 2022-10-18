#include "vertex_buffer.hpp"

// std
#include <vector>

VertexBuffer::VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, size_t size)
    : physicalDevice(physicalDevice), logicalDevice(logicalDevice), size(size)
{
    assert(size >= 3 && "vextex size should store at least 3 vertex");
	createVertexBuffer(size);
}

VertexBuffer::~VertexBuffer()
{
    vkDestroyBuffer(logicalDevice.getVkDevice(), vertexBuffer, nullptr);
    vkFreeMemory(logicalDevice.getVkDevice(), vertexBufferMemory, nullptr);
}

void VertexBuffer::subData(const std::vector<Vertex>& vertices)
{
    void* data;
    vkMapMemory(logicalDevice.getVkDevice(), vertexBufferMemory, 0, size, 0, &data);
    memcpy(data, vertices.data(), sizeof(Vertex) * vertices.size());
    vkUnmapMemory(logicalDevice.getVkDevice(), vertexBufferMemory);
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

    if (vkCreateBuffer(logicalDevice.getVkDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to create vertex buffer!");

    // get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice.getVkDevice(), vertexBuffer, &memRequirements);

    // allocate memory
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr; // optional
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(logicalDevice.getVkDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate vertex buffer memory!");

    // bind memory to the buffer
    vkBindBufferMemory(logicalDevice.getVkDevice(), vertexBuffer, vertexBufferMemory, 0);
}

uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.getVkPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }
    throw std::runtime_error("failed to find suitable memory type!");
}
