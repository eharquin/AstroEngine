#pragma once
// astro
#include "Ag_buffer.hpp"

//std
#include <array>

class AgModel
{
public:
	
	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};

		static VkVertexInputBindingDescription getBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
	};

	AgModel(AgDevice& agDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	~AgModel();

	// delete copy constructors
	AgModel(const AgModel&) = delete;
	AgModel& operator=(const AgModel&) = delete;

	void createVertexBuffer(std::vector<Vertex>& vertices);
	void createIndexBuffer(std::vector<uint32_t>& indices);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	// astro ref
	AgDevice& agDevice;

	std::unique_ptr<AgBuffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<AgBuffer> indexBuffer;
	uint32_t indexCount;
};