#include "Transform.h"
#include "glm/gtx/transform.hpp"


Transform::Transform(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 scale)
{
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
}

Transform::~Transform()
{
}

glm::mat4 Transform::GetModel() const
{
	glm::mat4 matPos = glm::translate(pos);
	glm::mat4 matScale = glm::scale(scale);
	glm::mat4 matRotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 matRotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 matRotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 matRot = matRotX * matRotY * matRotZ;

	return matPos * matScale * matRot;
}
