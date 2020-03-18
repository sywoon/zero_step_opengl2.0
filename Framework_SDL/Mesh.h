#pragma once
#include "glm/glm.hpp"
#include "gl/glew.h"

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord);

	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec2* GetTexCoord() { return &texCoord; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class Mesh
{
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NUM_BUFFERS,
	};


public:
	Mesh(Vertex* vertices, unsigned int numVertices);
	virtual ~Mesh();

	void Draw();

private:
	GLuint _vertexArrayObject;
	GLuint _vectexArrayBuffers[NUM_BUFFERS];
	unsigned int _drawCount;
};