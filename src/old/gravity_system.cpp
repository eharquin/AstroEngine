#include "gravity_system.hpp"

GravitySystem::GravitySystem(float strength) 
	: gravityStrength(strength)
{}

GravitySystem::~GravitySystem()
{}

void GravitySystem::update(std::vector<AgGameObject> gameObjects, float dt, int substeps)
{
	float sub_dt = dt / substeps;

	for (int i = 0; i < substeps; i++)
		simulationStep(gameObjects, sub_dt);
}

glm::vec2 GravitySystem::calculateForce(AgGameObject& fromObject, AgGameObject& toObject)
{
	glm::vec2 offset = fromObject.pos - toObject.pos;
	float distanceSquared = glm::dot(offset, offset);
	
	// if the objects are very near to each other
	if (glm::abs(distanceSquared) <= 1e-10f)
		return glm::vec2{ 0.0f, 0.0f };

	float force = gravityStrength * fromObject.mass * toObject.mass / distanceSquared;

	return force * offset / glm::sqrt(distanceSquared);
}

void GravitySystem::simulationStep(std::vector<AgGameObject> gameObjects, float dt)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		for (int j = 0; j < gameObjects.size(); j++)
		{
			if (i == j)
				continue;

			glm::vec2 force = calculateForce(gameObjects[i], gameObjects[j]);

			gameObjects[i].velocity += dt * -force / gameObjects[i].mass;
			gameObjects[j].velocity += dt * force / gameObjects[j].mass;
		}
	}

	// update each positions objects with his final velocity
	for (auto& obj : gameObjects)
		obj.pos += dt * obj.velocity;
}
