// astro
#include "ag_model.hpp"

// tiny obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

bool AgModel::Vertex::operator==(const Vertex& other) const {
    return position == other.position && color == other.color && texCoord == other.texCoord;
}

VkVertexInputBindingDescription AgModel::Vertex::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> AgModel::Vertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}

AgModel::AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::string textureFile)
    : agDevice(agDevice)
{
    createVertexBuffer(vertices);
    createIndexBuffer(indices);
    createTexture(textureFile);
}

AgModel::AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    : agDevice(agDevice)
{
    createVertexBuffer(vertices);
    createIndexBuffer(indices);
    texture = std::make_unique<AgTexture>(agDevice);
}

AgModel::AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices)
    : agDevice(agDevice)
{
    createVertexBuffer(vertices);
    texture = std::make_unique<AgTexture>(agDevice);
}

AgModel::AgModel(AgDevice& agDevice, const std::string modelFile, const std::string textureFile)
    : agDevice(agDevice)
{
    loadModel(modelFile);
    createTexture(textureFile);
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

void AgModel::createTexture(const std::string textureFile)
{
    texture = std::make_unique<AgTexture>(agDevice, textureFile);
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

void AgModel::loadModel(const std::string modelFile)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelFile.c_str())) 
        throw std::runtime_error(warn + err);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};
            
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    createVertexBuffer(vertices);
    createIndexBuffer(indices);
}
