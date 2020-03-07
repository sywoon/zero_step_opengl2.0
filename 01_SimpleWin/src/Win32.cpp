#include "Win32.h"
#include "Application.h"
#include "Win32.h"
#include "Log.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>



static const UINT WM_TOGGLE_FULLSCREEN = (WM_USER + 1);
static CWin32* s_pWin32 = NULL;
const char* _pszClassName = "OpenGL";
const char* _pszWinTitle = "GLFrame";


static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (s_pWin32 && s_pWin32->GetHWnd() == hWnd)
    {
        return s_pWin32->WindowProc(uMsg, wParam, lParam);
    }
    else
    {
    	switch (uMsg)
		{
		case WM_CREATE:  //此时CWin32还没接管
			{																
				s_pWin32->SetVisible(true);
				return 0;
			}
		default:
        	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    	}
    }
}


CWin32* CWin32::Create()
{
	if (s_pWin32)
		return s_pWin32;

	s_pWin32 = new CWin32();
	return s_pWin32;
}

void CWin32::Destroy()
{
	if (s_pWin32)
	{
		delete s_pWin32;
		s_pWin32 = NULL;
	}
}

CWin32::CWin32()
		: _hWnd(NULL)
		, _hInstance(NULL)
		, _hDC(NULL)
		, _hRC(NULL)
		, _bFullScreen(false)
		, _resizeDraw(false)
		, _isVisible(false)
		, _isLooping(true)
		, _nWinWidth(800)
		, _nWinHeight(600)
		, _dwLastTickCount(0)
{
	assert(s_pWin32 == NULL);
	s_pWin32 = this;

	_nSrnWidth = GetSystemMetrics(SM_CXSCREEN);
	_nSrnHeight = GetSystemMetrics(SM_CYSCREEN);

	_hInstance = GetModuleHandle(NULL);
}

CWin32::~CWin32()
{
}

bool CWin32::Init()
{
	if (!RegisterMyClass())
    	return false;

    HWND hWnd = CreateMyWindow(NULL);
	if (NULL == hWnd)
		return false;

	_hWnd = hWnd;

	HGLRC hRC;
	if (!InitOpenGL(hWnd, hRC))
	{
		DestroyWin(hWnd, hRC);
		return false;
	}
	_hRC = hRC;

	CApplication::GetInstance()->AppDidInited();

	return true;
}

void CWin32::Run()
{
	DealWinMessage(_hWnd);
	DestroyWin(_hWnd, _hRC);
	UnRegisterMyClass();
}

bool CWin32::RegisterMyClass()
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize			= sizeof(WNDCLASSEX);
	//移动时重画，并为窗口取得DC
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc		= (WNDPROC)(_WindowProc);
	windowClass.hInstance		= _hInstance;
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName	= _pszClassName;

	if (RegisterClassEx(&windowClass) == 0)
	{
		MessageBox(HWND_DESKTOP, "窗口窗口失败!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void CWin32::UnRegisterMyClass()
{
	UnregisterClass(_pszClassName, _hInstance);
}

HWND CWin32::CreateMyWindow(LPVOID lpParam)
{
	DWORD windowStyle = WS_TILEDWINDOW;
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						

	if (_bFullScreen)  //如果为全屏模式，尝试转化为全屏模式
	{
		//全屏模式转换失败，弹出对话框提示，并尝试窗口模式
		if (!ChangeScreenSetting())
		{	
			MessageBox(HWND_DESKTOP, "模式转换失败.\n在窗口模式下运行.", "Error", MB_OK | MB_ICONEXCLAMATION);
			_bFullScreen = false;
		}
		else
		{
			// ShowCursor(false);

			//设置窗口模式为顶层窗口
			windowStyle = WS_POPUP;  
			windowExtendedStyle |= WS_EX_TOPMOST;
		}																
	}

	/// 调整我们窗口的大小，使其客户区的大小为我们设置的大小
	int nX = 0;
	int nY = 0;
	RECT windowRect;  //整个窗口大小 非全屏下 大于客户区
	if (_bFullScreen)
	{
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = GetWidth();
		windowRect.bottom = GetHeight();
	}
	else
	{
		nX = (_nSrnWidth - _nWinWidth) / 2;
		nY = (_nSrnHeight - _nWinHeight) / 2;

		//使窗口具有3D外观
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = GetWidth();
		windowRect.bottom = GetHeight();

		//使窗口具有3D外观
		windowExtendedStyle |=  WS_EX_WINDOWEDGE;
		// [0,960,0,640]  转换前
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		// [-8,968,-31,648]  转换后
	}

	Log("win size:%d,%d,", windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
	HWND hWnd = CreateWindowEx(windowExtendedStyle,						// 窗口的扩展风格 */
							_pszClassName,								// 窗口的类名 */
							_pszWinTitle,								// 窗口标题 */
							windowStyle,								// 窗口的风格 */
							nX,nY,                                      // 窗口的左上角位置 */
							windowRect.right - windowRect.left,			// 窗口的宽度 */
							windowRect.bottom - windowRect.top,			// 窗口的高度 */
                            HWND_DESKTOP,								// 窗口的父窗口为桌面 */
							0,											// 无菜单 */
							_hInstance,									// 传入窗口的实例句柄 */
							lpParam);									// param指针

	if (NULL == hWnd)
	{
		_isLooping = false;
		char szError[255] = {0};
		sprintf(szError, "窗口创建失败:%d!", (int)GetLastError());
		MessageBox(HWND_DESKTOP, szError, "Error", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	return hWnd;
}



//切为全屏模式 
bool CWin32::ChangeScreenSetting()
{
	DEVMODE dmScreenSettings;
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
	dmScreenSettings.dmSize			= sizeof(DEVMODE);
	dmScreenSettings.dmPelsWidth	= _nSrnWidth;
	dmScreenSettings.dmPelsHeight	= _nSrnHeight;
	dmScreenSettings.dmBitsPerPel	= 32;					/**< 设置位深度 */
	//dmScreenSettings.dmDisplayFrequency = 75;             /**< 设置屏幕刷新率 */
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//| DM_DISPLAYFREQUENCY;

	/// 改变显示模式
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		return false;

	return true;
}


int CWin32::GetWidth()
{
	return _bFullScreen ? _nSrnWidth : _nWinWidth;
}

int CWin32::GetHeight()
{
	return _bFullScreen ? _nSrnHeight : _nWinHeight;
}

//切换 全屏/窗口模式
void CWin32::ToggleFullScreen()
{
	PostMessage(_hWnd, WM_TOGGLE_FULLSCREEN, 0, 0);
}

void CWin32::TerminateApplication()
{
	PostMessage(_hWnd, WM_QUIT, 0, 0);
	_isLooping = false;
}

bool CWin32::InitOpenGL(HWND hWnd, HGLRC& hRC)
{
	HDC hDC = GetDC(hWnd);  //设备描述表
	if (hDC == NULL)
		return false;
	_hDC = hDC;

	PIXELFORMATDESCRIPTOR pfd =	   //像素描述结构
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,								 /**< 版本号 */
		PFD_DRAW_TO_WINDOW	|			 /**< 缓存区的输出显示在一个窗口中 */
		PFD_SUPPORT_OPENGL	|			 /**< 缓存区支持OpenGL绘图 */
		//PFD_STEREO			|			 /**< 颜色缓存区是立体缓存 已不再支持*/ 
		PFD_DOUBLEBUFFER,				 /**< 颜色缓存区是双缓存 */
		PFD_TYPE_RGBA,					 /**< 使用RGBA颜色格式 */
		32,								 /**< 颜色缓存区中颜色值所占的位深 */
		0, 0, 0, 0, 0, 0,				 /**< 使用默认的颜色设置 */
		0,								 /**< 无Alpha缓存 */
		0,								 /**< 颜色缓存区中alpha成分的移位计数 */
		0,								 /**< 无累计缓存区 */
		0, 0, 0, 0,						 /**< 累计缓存区无移位 */
		32,								 /**< 32位深度缓存 */
		0,								 /**< 无蒙版缓存 */
		0,								 /**< 无辅助缓存区 */
		PFD_MAIN_PLANE,					 /**< 必须为PFD_MAIN_PLANE，设置为主绘图层 */
		0,								 /**< 表示OpenGL实现所支持的上层或下层平面的数量 */
		0, 0, 0							 /**< 过时，已不再使用 */
	};

	GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);	/**< 查找一个兼容的像素格式 */
	if (pixelFormat == 0)
		return false;													

	if (!SetPixelFormat(hDC, pixelFormat, &pfd))		/**< 设置像素格式 */
		return false;													

	hRC = wglCreateContext(hDC);		/**< 创建OpenGL的渲染描述表 */
	if (hRC == NULL)
		return false;													

	if (!wglMakeCurrent(hDC, hRC))			/**< 设置当前的OpenGL的渲染对象为当前的窗口 */
		return false;													
    
	ShowWindow(hWnd, SW_NORMAL);			/**< 显示窗口 */
	ReshapeGL();							/**< 告诉OpenGL调整窗口大小 */

	glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);  //GL_FLAT  GL_SMOOTH
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	_resizeDraw = true;


	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		Log("Error:%s", glewGetErrorString(status));
		return false;
	}

	return true;				
}


// 当窗口大小改变时，通知OpenGL调整大小
// 重新设置opengl投影矩阵
void CWin32::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);	/**< 重新设置视口 */

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f);	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	////视点  目标点  朝向
	////gluLookAt(0.0,5.0,0.0,  0,0,0, 1.0,0.0,0);
	////
	//float eye = 10.0f;
	//float logicH = eye * tanf(45.0/2*M_PI/180.0) * 2;
	//Log("logic height %f", logicH);

	////视点  目标点  朝向
	//gluLookAt(0.0,0.0,eye, 0,0,0, 0.0,1.0,0.0);
}

void CWin32::DestroyWin(HWND hWnd, HGLRC hRC)									
{
	if (hWnd != NULL)
	{
		HDC hDC = GetDC(hWnd);
		if (hDC != NULL)
		{
			wglMakeCurrent(hDC, 0);					/**< 设置当前的OpenGL的渲染对象为空NULL */
			if (hRC != 0)
			{
				wglDeleteContext(hRC);				/**< 释放OpenGL的渲染描述表 */
			}
			ReleaseDC(hWnd, hDC);					/**< 释放窗口的设备描述表 */
		}
		DestroyWindow(hWnd);
	}

	if (_bFullScreen)								/**< 如果为全屏模式，在程序结束后，变换到窗口模式，并显示鼠标 */
	{
		ChangeDisplaySettings(NULL, 0);				/**< 变换到窗口模式 */
		ShowCursor(true);							/**< 显示鼠标 */
	}
}

void CWin32::DealWinMessage(HWND hWnd)
{
	MSG msg;
	bool isMessagePumpActive = true;				/**< 当消息不为空时，处理消息循环 */
	_dwLastTickCount = GetTickCount();
	while (isMessagePumpActive)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0)
		{													
			if (msg.message != WM_QUIT)						
			{
				DispatchMessage(&msg);
			}
			else											
			{
				isMessagePumpActive = false;
			}
		}
		else
		{
			if (!_isVisible)
			{
				WaitMessage();								/**< 暂停程序，等待消息 */
			}
			else
			{												/**< 执行消息循环 */
				DWORD tickCount = GetTickCount();			/**< 返回计时器的当前值 */
				Update(tickCount - _dwLastTickCount);		/**< 调用用户自定义的更新函数 */
				_dwLastTickCount = tickCount;				/**< 重新设置上一次，计数器的值 */
				Draw();										/**< 调用用户自定义的绘制函数 */
				SwapBuffers(_hDC);						/**< 交换前后帧缓存 */
			}
		}
	}
}



LRESULT CWin32::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{																
			_isVisible = true;
			return 0;
		}
		break;

	case WM_SYSCOMMAND:  //截获系统命令
		switch (wParam)												
		{
			case SC_SCREENSAVE:   //截获屏幕保护启动命令
			case SC_MONITORPOWER: //截获显示其省电模式启动命令
				return 0;	//不启用这两个命令
			break;
		}
		break;

	case WM_CLOSE:
		{
			TerminateApplication();
			return 0;
		}
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		{
			_dwLastTickCount = GetTickCount();
			return 0;
		}
		break;

	case WM_MOVE:
		break;

	case WM_PAINT:
		if (_resizeDraw)	//如果需要重绘
		{
			ReshapeGL();	//重新设置窗口的大小
			//Draw();					//重新绘制
			//SwapBuffers(_hDC);	//交换前后帧缓存
			_resizeDraw = false;
		}
		break;

	case WM_SIZING:	 //窗口正在改变大小
		{
			RECT * rect = (RECT *)lParam;

			if (!_bFullScreen)
			{
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("WM_SIZING:%d,%d", _nWinWidth, _nWinHeight);
			}
			return TRUE;
		}
		break;

	case WM_SIZE:		//窗口改变大小后
		switch (wParam)			//处理不同的窗口状态
		{
		case SIZE_MINIMIZED:		//是否最小化?
			{
				_isVisible = false;	//如果是，则设置不可见
				Log("SIZE_MINIMIZED");
				return 0;											
			}
			break;

		case SIZE_MAXIMIZED:		//窗口是否最大化?
			{
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("SIZE_MAXIMIZED:%d,%d", _nWinWidth, _nWinHeight);
				ReshapeGL();
			}
			break;
		case SIZE_RESTORED:			//窗口被还原?
			{
				_isVisible = true;
				_nWinWidth = LOWORD(lParam);
				_nWinHeight = HIWORD(lParam);
				Log("SIZE_RESTORED:%d,%d", _nWinWidth, _nWinHeight);
				ReshapeGL();
				_dwLastTickCount = GetTickCount();
				return 0;											
			}
			break;
		}
		break;															

	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_ESCAPE:
				TerminateApplication();
				break;
			case VK_F1:
				ToggleFullScreen();
				break;
		}
		break;

	case WM_KEYUP:		
		break;

	case WM_TOGGLE_FULLSCREEN:	//切换 全屏/窗口模式
	    {
	    	_bFullScreen = !_bFullScreen;
	  //   	if(!_bFullScreen)
			// 	ShowCursor(true);
			// else
			// 	ShowCursor(false);

			PostMessage(_hWnd, WM_QUIT, 0, 0);
	    }
		break;				
	default:
		break;											
	}

	return DefWindowProc(_hWnd, uMsg, wParam, lParam);
}

void CWin32::Update(DWORD milliseconds)
{
	CApplication::GetInstance()->Update(milliseconds);
}

void CWin32::Draw()
{
	CApplication::GetInstance()->Draw();
}





