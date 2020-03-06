#include <windows.h>
#include <assert.h>
#include <stdio.h>


#include "Log.h"
#include "Application.h"
#include "Win32.h"


#define USE_WIN32_CONSOLE


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

	InitLogFile();
	//InitLogFile("Log");
	//InitLogFile("Log", "output.log");
	//ClearLog();
	Log("=========start===========");
	Log("中文测试utf-8");
	
	CApplication::Create();
	CWin32* pWin32 = CWin32::Create();

	pWin32->SetFullScreen(false);
    while (pWin32->IsLooping())  //切换全屏用
    {
    	if (!pWin32->Init())
    		break;

    	pWin32->Run();
    }

    CWin32::Destroy();
    CApplication::Destroy();
	
	Log("=========end===========\n");

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return 0;
}

