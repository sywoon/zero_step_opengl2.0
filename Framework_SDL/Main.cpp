#include <iostream>
#include "gl/glew.h"
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv)
{
	CDisplay display(800, 600, "hello");
	Vertex vertices[] = {
		Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0, 0)), 
		Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
		Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0)),
	};

	Mesh mesh(vertices, sizeof(vertices)/sizeof(vertices[0]));
	Shader shader("res/basicShader");
	Texture tex("res/wall.jpg");
	Transform transform;
	Camera camera(glm::vec3(0, 0, -3), 70.0f, (float)WIDTH/(float)HEIGHT, 0.01, 1000.0f);

	float counter = 0.0f;
	while (!display.IsClosed())
	{
		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);

		float sinCounter = sinf(counter);
		float cosCounter = cosf(counter);

		transform.GetPos()->x = sinCounter;
		transform.GetRot()->z = counter * 50;
		transform.SetScale(glm::vec3(cosCounter, cosCounter, cosCounter));

		shader.Use();
		tex.Bind();
		
		shader.Update(transform, camera);
		mesh.Draw();
		display.Update();

		counter += 0.01f;
	}
	return 0;
}


