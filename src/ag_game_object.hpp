#pragma once
// astro
#include "ag_model.hpp"

// std
#include <memory>

struct TransformComponent {
	glm::vec3 translation{};
	glm::vec3 scale{ 1.f, 1.f, 1.f };
	glm::vec3 rotation{};

	// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
	// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
	// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	glm::mat4 mat4();

	glm::mat3 normalMatrix();
};

class AgGameObject
{
public:
	using id_t = unsigned int;
	
	static AgGameObject createGameObject()
	{
		static id_t currentId = 0;
		return AgGameObject{ currentId++ };
	}

	id_t getId() { return id; }

	std::shared_ptr<AgModel> model;
	glm::vec3 color{};
	TransformComponent transform{};

private:
	AgGameObject(id_t objId) : id{objId} {}

	id_t id;
};