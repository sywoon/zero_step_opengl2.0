#include "Application.h"
#include <assert.h>
#include "GL/glew.h"
#include "Math3d.h"


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
		, _buffer(NULL)
		, _vbo(0)
		, _gWorldLocation(0)
		, _uBufferCapacity(0)
		, _nBufferCount(0)
		, _bDirty(false)
{
}

CApplication::~CApplication()
{
	if (_vbo != 0)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_buffer)
	{
		free(_buffer);
		_buffer = NULL;
	}
}

void CApplication::ensureCapacity(unsigned int count)
{
	if (_nBufferCount + count > _uBufferCapacity)
	{
		_uBufferCapacity += MAX(_uBufferCapacity, count);
		_buffer = (ccV2F_C4B_T2F*)realloc(_buffer, _uBufferCapacity * sizeof(ccV2F_C4B_T2F));
	}
}


void CApplication::AppDidInited()
{
	_program.initWithVertexShaderFilename("res/drawNode.vs", "res/drawNode.fs");
	_program.addAttribute("a_position", kCCVertexAttrib_Position);
	_program.addAttribute("a_color", kCCVertexAttrib_Color);
	_program.addAttribute("a_texcoord", kCCVertexAttrib_TexCoords);

	_program.link();

	_gWorldLocation = _program.getUniformLocationForName("gWorld");

	Vector3f cameraPos(0, 0, -3);
	Vector3f cameraTarget(0, 0, 1);
	Vector3f cameraUp(0, 1, 0);
	_pipeline.SetCamera(cameraPos, cameraTarget, cameraUp);
	_pipeline.SetPerspectiveProjection(60, 800, 600, 1.0f, 100.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ccVertex2F pos = { 0.0f, 0.0f };
	ccColor4B color = { 10, 255, 10, 255 };
	this->DrawDot(pos, 0.2f, color);
}


void CApplication::Update(DWORD milliseconds)
{
	//_fAngle += (float)(milliseconds) / 15.0f;

	//_pipeline.Rotate(0, _fAngle, 0);
	//_pipeline.Translate(0, 0, 3.0f);

	_program.use();
}

void CApplication::Draw()
{
	if (_bDirty)
	{
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F) * _nBufferCount, _buffer, GL_STREAM_DRAW);
		
		CHECK_GL_ERROR_DEBUG();

		_bDirty = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 数据的偏移值 按列主序存放(右乘向量)  但数组数据按行顺序存放  所以需要转置一次
	glUniformMatrix4fv(_gWorldLocation, 1, GL_TRUE, (const float*)_pipeline.GetMat());

	glEnableVertexAttribArray(kCCVertexAttrib_Position);
	glEnableVertexAttribArray(kCCVertexAttrib_Color);
	glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// vertex
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid*)offsetof(ccV2F_C4B_T2F, vertices));
	// color
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid*)offsetof(ccV2F_C4B_T2F, colors));
	// texcood
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid*)offsetof(ccV2F_C4B_T2F, texCoords));


	glDrawArrays(GL_TRIANGLES, 0, _nBufferCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	CHECK_GL_ERROR_DEBUG();
}


void CApplication::DrawDot(const ccVertex2F& pos, float radius, const ccColor4B& color)
{
	unsigned int vertex_count = 2 * 3;  //2个三角形
	ensureCapacity(vertex_count);

	ccV2F_C4B_T2F a = { {pos.x - radius, pos.y - radius}, color, {-1.0, -1.0} };
	ccV2F_C4B_T2F b = { {pos.x - radius, pos.y + radius}, color, {-1.0,  1.0} };
	ccV2F_C4B_T2F c = { {pos.x + radius, pos.y + radius}, color, { 1.0,  1.0} };
	ccV2F_C4B_T2F d = { {pos.x + radius, pos.y - radius}, color, { 1.0, -1.0} };

	ccV2F_C4B_T2F_Triangle* triangles = (ccV2F_C4B_T2F_Triangle*)(_buffer + _nBufferCount);
	ccV2F_C4B_T2F_Triangle triangle0 = { a, b, c };
	ccV2F_C4B_T2F_Triangle triangle1 = { a, c, d };
	triangles[0] = triangle0;
	triangles[1] = triangle1;

	_nBufferCount += vertex_count;

	_bDirty = true;
}