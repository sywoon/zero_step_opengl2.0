#pragma once
#include <string>
#include "gl/glew.h"

class Texture
{
public:
	Texture(const std::string& fileName);
	virtual ~Texture();

	void Bind();

private:
	GLuint _texture;
};