#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
	~Camera();

	//世界转投影
	inline glm::mat4 GetViewProjection() const 
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};