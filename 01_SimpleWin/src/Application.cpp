#include "Application.h"
#include <assert.h>
#include "GL/glew.h"
#include "Math3d.h"


GLuint vbo = 0;
GLuint ibo = 0;
GLuint gWorldLocation = 0;

static void createVertexBuffer()
{
	Vector3f vertices[] = {
		{ -1.0f, -1.0f, 0.5773f },  //顶点
		{ 1.0f, 0.0f, 0.0f },  //颜色

		{ 0.0f, -1.0f, -1.15475f },
		{ 0.0f, 1.0f, 0.0f },

		{ 1.0f, -1.0f, 0.5773f },
		{ 0.0f, 0.0f, 1.0f },

		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void createIndexBuffer()
{
	unsigned int indices[] = 
	{
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2,
	};

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


static CApplication* s_pInstance = NULL;


CApplication* CApplication::Create()
{
	assert(s_pInstance == NULL);

	s_pInstance = new CApplication();
	return s_pInstance;
}

CApplication* CApplication::GetInstance()
{
	return s_pInstance;
}

void CApplication::Destroy()
{
	if (s_pInstance)
	{
		free(s_pInstance);
		s_pInstance = NULL;
	}
}

CApplication::CApplication()
		: _fAngle(0.0f)
{
}

CApplication::~CApplication()
{
}

void CApplication::AppDidInited()
{
	_program.initWithVertexShaderFilename("res/baseShader.vs", "res/baseShader.fs");
	_program.link();

	gWorldLocation = _program.getUniformLocationForName("gWorld");

	Vector3f cameraPos(0, 0, -3);
	Vector3f cameraTarget(0, 0, 1);
	Vector3f cameraUp(0, 1, 0);
	_pipeline.SetCamera(cameraPos, cameraTarget, cameraUp);
	_pipeline.SetPerspectiveProjection(60, 800, 600, 1.0f, 100.0f);

	createVertexBuffer();
	createIndexBuffer();
}

void CApplication::Update(DWORD milliseconds)
{
	_fAngle += (float)(milliseconds) / 15.0f;

	_pipeline.Rotate(0, _fAngle, 0);
	_pipeline.Translate(0, 0, 3.0f);

	_program.use();
}

void CApplication::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 数据的偏移值 按列主序存放(右乘向量)  但数组数据按行顺序存放  所以需要转置一次
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const float*)_pipeline.GetMat());
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  //上一个必须是vbo
	glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


