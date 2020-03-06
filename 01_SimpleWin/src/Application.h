#pragma once

#include <windows.h>
#include "GLProgram.h"
#include "Pipeline.h"

class CApplication
{
public:
	static CApplication* Create();
	static CApplication* GetInstance();
	static void Destroy();

	void AppDidInited();
	void Update(DWORD milliseconds);
	void Draw();

protected:
	CApplication();
	~CApplication();

private:
	float _fAngle;
	GLProgram _program;
	Pipeline _pipeline;
};