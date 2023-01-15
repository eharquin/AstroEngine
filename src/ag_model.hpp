#pragma once
// astro
#include "ag_buffer.hpp"
#include "ag_texture.hpp"

//std
#include <array>
#include <memory>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

class AgModel
{
public:
	
	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec2 texCoord;

		bool operator==(const Vertex& other) const;

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
	};

	AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, const std::string textureFile);
	AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices);
	AgModel(AgDevice& agDevice, const std::string modelFile, const std::string textureFile);
	~AgModel();

	// delete copy constructors
	AgModel(const AgModel&) = delete;
	AgModel& operator=(const AgModel&) = delete;

	void createVertexBuffer(std::vector<Vertex>& vertices);
	void createIndexBuffer(std::vector<uint32_t>& indices);
	void createTexture(const std::string textureFile);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);


	// test
	VkImageView getTextureImageView() const { return texture->getTextureImageView(); }
	VkSampler getTextureSampler() const { return texture->getTextureSampler(); }


private:
	// astro ref
	AgDevice& agDevice;

	std::unique_ptr<AgBuffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<AgBuffer> indexBuffer;
	uint32_t indexCount;

	bool hasTexture = false;
	std::unique_ptr<AgTexture> texture;

	void loadModel(const std::string modelFile);
};

// implementation of hash calculation for Vertex
namespace std
{
	template<> struct hash<AgModel::Vertex>
	{
		size_t operator()(AgModel::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}