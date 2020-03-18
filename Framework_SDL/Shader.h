#pragma once
#include "gl/glew.h"
#include <string>
#include "Transform.h"
#include "Camera.h"


class Shader
{
	enum
	{
		TRANSFORM_U,
		NUM_UNIFORM,
	};

public:
	Shader(const std::string& fileName);
	~Shader();

	void Use();
	void Update(const Transform& transform, const Camera& camera);

private:
	static const unsigned int NUM_SHADERS = 2;

	GLuint _program;
	GLuint _shaders[NUM_SHADERS];

	GLuint _uniforms[NUM_UNIFORM];
};