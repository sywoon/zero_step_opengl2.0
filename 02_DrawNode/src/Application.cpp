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

	ccVertex2F pos = { 0.0f, 1.0f };
	ccColor4B color = { 10, 255, 10, 255 };
	this->DrawDot(pos, 0.2f, color);

	ccVertex2F from = {-0.3, -1.5};
	ccVertex2F to = { 0.3, -1.5 };
	ccColor4B color2 = { 200, 100, 10, 255 };
	this->DrawSegment(from, to, 0.1, color2);

	const int polygon_count = 5;
	ccVertex2F verts[polygon_count];  //顺时针5个点
	verts[0] = {-0.5, -0.7};
	verts[1] = { -0.5, 0.0 };
	verts[2] = { 0, 0.3 };
	verts[3] = { 0.5, 0.0 };
	verts[4] = { 0.5, -0.7 };

	ccColor4B fillColor = { 10, 100, 100, 255 };
	ccColor4B borderColor = { 200, 100, 100, 255 };
	float borderWidth = 0.03;
	DrawPolygon(verts, polygon_count, fillColor, borderWidth, borderColor);
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


void CApplication::DrawSegment(const ccVertex2F& from, const ccVertex2F& to, float radius, const ccColor4B& color)
{
	unsigned int vertex_count = 6 * 3; //6个三角形  两个端点+中间
	ensureCapacity(vertex_count);

	ccVertex2F a = from;
	ccVertex2F b = to;


	ccVertex2F n = v2fnormalize(v2fperp(v2fsub(b, a)));
	ccVertex2F t = v2fperp(n);

	ccVertex2F nw = v2fmult(n, radius);
	ccVertex2F tw = v2fmult(t, radius);
	ccVertex2F v0 = v2fsub(b, v2fadd(nw, tw));
	ccVertex2F v1 = v2fadd(b, v2fsub(nw, tw));
	ccVertex2F v2 = v2fsub(b, nw);
	ccVertex2F v3 = v2fadd(b, nw);
	ccVertex2F v4 = v2fsub(a, nw);
	ccVertex2F v5 = v2fadd(a, nw);
	ccVertex2F v6 = v2fsub(a, v2fsub(nw, tw));
	ccVertex2F v7 = v2fadd(a, v2fadd(nw, tw));


	ccV2F_C4B_T2F_Triangle* triangles = (ccV2F_C4B_T2F_Triangle*)(_buffer + _nBufferCount);

	ccV2F_C4B_T2F_Triangle triangles0 = {
		{v0, (color), __t(v2fneg(v2fadd(n, t)))},
		{v1, (color), __t(v2fsub(n, t))},
		{v2, (color), __t(v2fneg(n))},
	};
	triangles[0] = triangles0;

	ccV2F_C4B_T2F_Triangle triangles1 = {
		{v3, (color), __t(n)},
		{v1, (color), __t(v2fsub(n, t))},
		{v2, (color), __t(v2fneg(n))},
	};
	triangles[1] = triangles1;

	ccV2F_C4B_T2F_Triangle triangles2 = {
		{v3, (color), __t(n)},
		{v4, (color), __t(v2fneg(n))},
		{v2, (color), __t(v2fneg(n))},
	};
	triangles[2] = triangles2;

	ccV2F_C4B_T2F_Triangle triangles3 = {
		{v3, (color), __t(n)},
		{v4, (color), __t(v2fneg(n))},
		{v5, (color), __t(n) },
	};
	triangles[3] = triangles3;

	ccV2F_C4B_T2F_Triangle triangles4 = {
		{v6, (color), __t(v2fsub(t, n))},
		{v4, (color), __t(v2fneg(n)) },
		{v5, (color), __t(n)},
	};
	triangles[4] = triangles4;

	ccV2F_C4B_T2F_Triangle triangles5 = {
		{v6, (color), __t(v2fsub(t, n))},
		{v7, (color), __t(v2fadd(n, t))},
		{v5, (color), __t(n)},
	};
	triangles[5] = triangles5;

	_nBufferCount += vertex_count;

	_bDirty = true;
}



void CApplication::DrawPolygon(ccVertex2F* verts, unsigned int count, const ccColor4B& fillColor, float borderWidth, const ccColor4B& borderColor)
{
	struct ExtrudeVerts { ccVertex2F offset, n; };
	struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts) * count);
	memset(extrude, 0, sizeof(struct ExtrudeVerts) * count);

	for (unsigned int i = 0; i < count; i++)
	{
		ccVertex2F v0 = (verts[(i - 1 + count) % count]);
		ccVertex2F v1 = (verts[i]);
		ccVertex2F v2 = (verts[(i + 1) % count]);

		ccVertex2F n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
		ccVertex2F n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));

		ccVertex2F offset = v2fmult(v2fadd(n1, n2), 1.0 / (v2fdot(n1, n2) + 1.0));
		struct ExtrudeVerts tmp = { offset, n2 };
		extrude[i] = tmp;
	}

	bool outline = (borderColor.a > 0.0 && borderWidth > 0.0);

	unsigned int triangle_count = 3 * count - 2;
	unsigned int vertex_count = 3 * triangle_count;
	ensureCapacity(vertex_count);

	ccV2F_C4B_T2F_Triangle* triangles = (ccV2F_C4B_T2F_Triangle*)(_buffer + _nBufferCount);
	ccV2F_C4B_T2F_Triangle* cursor = triangles;

	float inset = (outline == 0.0 ? 0.5 : 0.0);
	for (unsigned int i = 0; i < count - 2; i++)
	{
		ccVertex2F v0 = v2fsub((verts[0]), v2fmult(extrude[0].offset, inset));
		ccVertex2F v1 = v2fsub((verts[i + 1]), v2fmult(extrude[i + 1].offset, inset));
		ccVertex2F v2 = v2fsub((verts[i + 2]), v2fmult(extrude[i + 2].offset, inset));

		ccV2F_C4B_T2F_Triangle tmp = {
			{v0, (fillColor), __t(v2fzero)},
			{v1, (fillColor), __t(v2fzero)},
			{v2, (fillColor), __t(v2fzero)},
		};

		*cursor++ = tmp;
	}

	for (unsigned int i = 0; i < count; i++)
	{
		int j = (i + 1) % count;
		ccVertex2F v0 = (verts[i]);
		ccVertex2F v1 = (verts[j]);

		ccVertex2F n0 = extrude[i].n;

		ccVertex2F offset0 = extrude[i].offset;
		ccVertex2F offset1 = extrude[j].offset;

		if (outline)
		{
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));

			ccV2F_C4B_T2F_Triangle tmp1 = {
				{inner0, (borderColor), __t(v2fneg(n0))},
				{inner1, (borderColor), __t(v2fneg(n0))},
				{outer1, (borderColor), __t(n0)}
			};
			*cursor++ = tmp1;

			ccV2F_C4B_T2F_Triangle tmp2 = {
				{inner0, (borderColor), __t(v2fneg(n0))},
				{outer0, (borderColor), __t(n0)},
				{outer1, (borderColor), __t(n0)}
			};
			*cursor++ = tmp2;
		}
		else {
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, 0.5));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, 0.5));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, 0.5));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, 0.5));

			ccV2F_C4B_T2F_Triangle tmp1 = {
				{inner0, (fillColor), __t(v2fzero)},
				{inner1, (fillColor), __t(v2fzero)},
				{outer1, (fillColor), __t(n0)}
			};
			*cursor++ = tmp1;

			ccV2F_C4B_T2F_Triangle tmp2 = {
				{inner0, (fillColor), __t(v2fzero)},
				{outer0, (fillColor), __t(n0)},
				{outer1, (fillColor), __t(n0)}
			};
			*cursor++ = tmp2;
		}
	}

	_nBufferCount += vertex_count;

	_bDirty = true;

	free(extrude);
}