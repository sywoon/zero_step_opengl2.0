#pragma once

#include <windows.h>
#include "GL/glew.h"


class CWin32
{
public:
	static CWin32* Create();
	static void Destroy();

	HWND GetHWnd() const { return _hWnd; }
	bool IsLooping() const { return _isLooping; }
	void SetVisible(bool value) { _isVisible = value; }
	void SetFullScreen(bool value) { _bFullScreen = value; }

	bool Init();
	void Run();

	LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	CWin32();
	~CWin32();

	bool RegisterMyClass();
	void UnRegisterMyClass();

	HWND CreateMyWindow(LPVOID lpParam);

	void Update(DWORD milliseconds);
	void Draw();
	

private:
	bool ChangeScreenSetting();
	int GetWidth();
	int GetHeight();

	bool InitOpenGL(HWND hWnd, HGLRC& hRC);
	void ReshapeGL();
	void DestroyWin(HWND hWnd, HGLRC hRC);
	void DealWinMessage(HWND hWnd);

	void ToggleFullScreen();
	void TerminateApplication();


private:
	HINSTANCE _hInstance;
	HWND _hWnd;
	HDC	_hDC;
	HGLRC _hRC;
	bool _bFullScreen;
	bool _resizeDraw;
	bool _isVisible;
	bool _isLooping;

	int _nSrnWidth;
	int _nSrnHeight;
	int _nWinWidth;
	int _nWinHeight;

	DWORD _dwLastTickCount;
};


