#pragma once

#include <windows.h>
#include "GLProgram.h"
#include "Pipeline.h"
#include "GlType.h"

class CApplication
{
public:
	static CApplication* Create();
	static CApplication* GetInstance();
	static void Destroy();

	void AppDidInited();
	void Update(DWORD milliseconds);
	void Draw();

	void DrawDot(const ccVertex2F& pos, float radius, const ccColor4B& color);
	void DrawSegment(const ccVertex2F& from, const ccVertex2F& to, float radius, const ccColor4B& color);
	void DrawPolygon(ccVertex2F* verts, unsigned int count, const ccColor4B& fillColor, float borderWidth, const ccColor4B& borderColor);


protected:
	CApplication();
	~CApplication();

private:
	void ensureCapacity(unsigned int count);

private:
	float _fAngle;
	GLProgram _program;
	Pipeline _pipeline;

	GLuint	_vbo;
	GLuint	_gWorldLocation;

	ccV2F_C4B_T2F* _buffer;
	GLsizei		   _uBufferCapacity;
	GLsizei        _nBufferCount;

	bool           _bDirty;
};