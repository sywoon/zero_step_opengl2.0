#include "Mesh.h"
#include <vector>

Vertex::Vertex(const glm::vec3 & pos, const glm::vec2& texCoord)
{
	this->pos = pos;
	this->texCoord = texCoord;
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	_drawCount = numVertices;
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	positions.reserve(numVertices);
	texCoords.reserve(numVertices);

	for (unsigned int i = 0; i < numVertices; i++)
	{
		positions.push_back(*vertices[i].GetPos());
		texCoords.push_back(*vertices[i].GetTexCoord());
	}

	glGenBuffers(NUM_BUFFERS, _vectexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, _vectexArrayBuffers[POSITION_VB]);
	/*glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertices[0]),
					vertices, GL_STATIC_DRAW);*/
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]),
		&positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, _vectexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]),
		&texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, _drawCount);
	glBindVertexArray(0);
}