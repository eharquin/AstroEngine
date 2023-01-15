#include "ag_model.hpp"

VkVertexInputBindingDescription AgModel::Vertex::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> AgModel::Vertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

AgModel::AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    : agDevice(agDevice)
{
    createVertexBuffer(vertices);
    createIndexBuffer(indices);
}

AgModel::~AgModel()
{
}

void AgModel::createVertexBuffer(std::vector<Vertex>& vertices)
{
    VkDeviceSize vertexSize = sizeof(vertices[0]);
    vertexCount = static_cast<uint32_t>(vertices.size());

    AgBuffer stagingBuffer{
        agDevice,
        vertexSize,
        vertexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    VkResult res = stagingBuffer.map();
    stagingBuffer.write(vertices.data());

    vertexBuffer = std::make_unique<AgBuffer>(
        agDevice,
        vertexSize,
        vertexCount,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    agDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), vertexBuffer->getSize());
}

void AgModel::createIndexBuffer(std::vector<uint32_t>& indices)
{
    indexCount = static_cast<uint32_t>(indices.size());

    if (indexCount == 0)
        return;

    hasIndexBuffer = true;

    AgBuffer stagingBuffer{
        agDevice,
        sizeof(uint32_t),
        indexCount,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    VkResult res = stagingBuffer.map();
    stagingBuffer.write(indices.data());

    indexBuffer = std::make_unique<AgBuffer>(
        agDevice,
        sizeof(uint32_t),
        indexCount,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    agDevice.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), indexBuffer->getSize());
}

void AgModel::bind(VkCommandBuffer commandBuffer)
{
    // bind vertex buffer
    VkBuffer buffers[] = { vertexBuffer->getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    // bind index buffer
    if (hasIndexBuffer)
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void AgModel::draw(VkCommandBuffer commandBuffer)
{
    if (hasIndexBuffer)
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);

    else
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}
