#include "sierpinski_triangle_system.hpp"

SierpinskiTriangleSystem::SierpinskiTriangleSystem(AgDevice& agDevice)
	: agDevice(agDevice)
{
	std::srand(static_cast <unsigned> (std::time(0))); // use current time as seed for random generator
	generateVertices(n);
}

glm::vec3 SierpinskiTriangleSystem::getRandomColor()
{

	float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
	return { r, g, b };
}

std::vector<AgVertexBuffer::Vertex> SierpinskiTriangleSystem::generateSierpinskiTriangleVertex(int n, AgVertexBuffer::Vertex up, AgVertexBuffer::Vertex left, AgVertexBuffer::Vertex right)
{
	std::vector<AgVertexBuffer::Vertex> vertices;

	if (n == 1)
	{
		glm::vec3 color = getRandomColor();
		up.color = color;
		left.color = color;
		right.color = color;
		vertices.push_back(up);
		vertices.push_back(left);
		vertices.push_back(right);
		return vertices;
	}

	// up
	AgVertexBuffer::Vertex upUp = up;
	AgVertexBuffer::Vertex upLeft = { (left.pos + up.pos) / 2.0f, glm::vec3(1.0f) };
	AgVertexBuffer::Vertex upRight = { (right.pos + up.pos) / 2.0f, glm::vec3(1.0f) };
	auto upVertices = generateSierpinskiTriangleVertex(n - 1, upUp, upLeft, upRight);
	vertices.insert(vertices.begin(), upVertices.begin(), upVertices.end());

	// left
	AgVertexBuffer::Vertex leftUp = upLeft;
	AgVertexBuffer::Vertex leftLeft = left;
	AgVertexBuffer::Vertex leftRight = { (right.pos + left.pos) / 2.0f, glm::vec3(1.0f) };
	auto leftVertices = generateSierpinskiTriangleVertex(n - 1, leftUp, leftLeft, leftRight);
	vertices.insert(vertices.begin(), leftVertices.begin(), leftVertices.end());

	// right
	AgVertexBuffer::Vertex rightUp = upRight;
	AgVertexBuffer::Vertex rightLeft = leftRight;
	AgVertexBuffer::Vertex rightRight = right;
	auto rightVertices = generateSierpinskiTriangleVertex(n - 1, rightUp, rightLeft, rightRight);
	vertices.insert(vertices.begin(), rightVertices.begin(), rightVertices.end());

	return vertices;
}
