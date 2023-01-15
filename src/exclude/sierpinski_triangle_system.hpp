#pragma once
// astro
#include "ag_game_object.hpp"

// std
#include <vector>

class SierpinskiTriangleSystem
{
public:
	SierpinskiTriangleSystem(AgDevice& agDevice);
	SierpinskiTriangleSystem operator++(int) { generateVertices(++n); return *this; }

	AgGameObject getSierpinskiTriangle() { return sierpinskiTriangle; }

	void generateVertices(int n)
	{
		std::cout << "n : " << n << std::endl;
		std::vector<AgModel::Vertex> vertices = generateSierpinskiTriangleVertex(n, up, left, right);
		std::vector<uint32_t> indices{};
		sierpinskiTriangle.model = std::make_shared<AgModel>(agDevice, vertices, indices);
	}

private:
	AgGameObject sierpinskiTriangle = AgGameObject::createGameObject();
	int n = 1;

	// astro ref
	AgDevice& agDevice;

	const AgModel::Vertex up = { {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} };
	const AgModel::Vertex left = { {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	const AgModel::Vertex right = { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };

	std::vector<AgModel::Vertex> generateSierpinskiTriangleVertex(int n, AgModel::Vertex up, AgModel::Vertex left, AgModel::Vertex right);
	glm::vec3 getRandomColor();
};