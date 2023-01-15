#pragma once
// astro
#include "ag_game_object.hpp"

// std
#include <vector>

class GravitySystem
{
public:
	GravitySystem(float strength);
	~GravitySystem();

	float gravityStrength;
	
	void update(std::vector<AgGameObject> gameObjects, float dt, int substeps = 1);
	glm::vec2 calculateForce(AgGameObject& froObject, AgGameObject& toObject);

private:
	void simulationStep(std::vector<AgGameObject> gameObjects, float dt);

};