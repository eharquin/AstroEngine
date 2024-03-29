#pragma once

//glm
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class AgCamera
{
public:
	// setter for projection matrix
	void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
	void setPerspectiveProjection(float fovy, float aspect, float near, float far);

	// setter for view matrix
	void setViewDirection(
		glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
	void setViewTarget(
		glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.0f, -1.0f, 0.0f });
	void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

	const glm::mat4& getProjection() const { return proj; }
	const glm::mat4& getView() const { return view; }
	
private:
	glm::mat4 proj{1.0f};
	glm::mat4 view{1.0f};
};