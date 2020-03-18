#include "Camera.h"


Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
{
	this->pos = pos;
	this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->projection = glm::perspective(fov, aspect, zNear, zFar);
}

Camera::~Camera()
{
}
