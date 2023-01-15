#pragma once
// astro
#include "ag_model.hpp"

// std
#include <memory>

class AgGameObject
{
public:
	using id_t = unsigned int;
	
	static AgGameObject createGameObject()
	{
		static id_t currentId = 0;
		return AgGameObject{ currentId++ };
	}

	glm::mat4 transform()
	{
		auto transform = glm::mat4{ 1.0f };

		transform = glm::scale(transform, scale);
		// rotation 
		transform = glm::translate(transform, position);
		
		return transform;
	}

	//AgGameObject(const AgGameObject &) = delete;
	//AgGameObject &operator=(const AgGameObject &) = delete;
	//AgGameObject(AgGameObject &&) = default;
	//AgGameObject &operator=(AgGameObject &&) = default;

	id_t getId() { return id; }

	std::shared_ptr<AgModel> model;

	glm::vec3 position{};
	glm::vec2 velocity{};
	glm::vec3 color{};

	glm::vec3 rotation{0.0f};
	glm::vec3 scale{ 1.0f };

	float mass = 0;

private:
	AgGameObject(id_t objId) : id{objId} {}

	id_t id;
};