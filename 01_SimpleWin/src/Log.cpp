#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <sys/stat.h>
#include "Charset.h"


#ifdef WIN32
#	include <time.h>
#	include <direct.h>
#else
#	include <sys/time.h>
#	include <dirent.h>
#endif

#include "Log.h"

static char _pszFile[512] = "output.log";
static bool _autoRtn = true;  //自动换行


HANDLE _CreateMyFile(const char* pszFile)
{
	HANDLE hFile = ::CreateFile(pszFile, 
							GENERIC_WRITE | GENERIC_READ,
							0,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("create file:%s failed!\n", pszFile);
		return NULL;
	}
	return hFile;
}

void Log(const char* pszFmt, ...)
{
	va_list args;
	va_start(args, pszFmt);

    //vc6 not support
    const int extLen = _autoRtn ? 2 : 1;  //额外增加 \n + \0
	int len = vsnprintf(0, 0, pszFmt, args) + extLen;
	char* pszBuf = (char*)_alloca(len * sizeof(char));//栈中分配, 不需要释放

	vsnprintf(pszBuf, len, pszFmt, args);
	va_end(args);

	pszBuf[len-1] = '\0';
	if (_autoRtn)
	{
		pszBuf[len-2] = '\n';
	}

	//由于cl和clang对utf-8源码处理方式不同
	//cl /source-charset:utf-8 会自动将源码中的字符串转为ansi
	//clang 默认就支持utf-8源码
	
	bool bFromUtf8 = false;
	char* pszAnsi = NULL;

#ifdef COMPILE_CL
	bFromUtf8 = false;
#else
	if (utf82ansi(pszBuf, &pszAnsi))
	{
		bFromUtf8 = true;
	}
#endif

	HANDLE hFile = _CreateMyFile(_pszFile);
	if (NULL == hFile)
		return;

	SetFilePointer(hFile, 0, 0, FILE_END);

	DWORD dwWriteSize = 0;
	BOOL bRet = 0;
	if (bFromUtf8)
	{
		printf("%s", pszAnsi);
		bRet = ::WriteFile(hFile, pszAnsi, strlen(pszAnsi),
					&dwWriteSize, NULL);
		free(pszAnsi);
	} else {
		printf("%s", pszBuf);
		bRet = ::WriteFile(hFile, pszBuf, strlen(pszBuf),
					&dwWriteSize, NULL);
	}
	
	if (bRet)
	{
		FlushFileBuffers(hFile);
	}
	CloseHandle(hFile);

	if (_autoRtn)
	{
		printf("\n");
	}
}

void InitLogFile(const char* pszPath, const char* pszFile)
{
	if (NULL == pszPath)  //没路径和文件名 默认根目录下
	{
		time_t ltime;
		time(&ltime);
		
		struct tm curTime; 
		localtime_s(&curTime, &ltime); 
		//tm* curTime = localtime(&ltime);
		
		//.tm_wday 一年中第几周
		//.tm_hour .tm_min .tm_sec
		//.tm_isdst 夏令时?
		sprintf_s(_pszFile, sizeof(_pszFile), "%u_%u_%u.log",
			curTime.tm_year + 1900, curTime.tm_mon + 1, curTime.tm_mday);
		return;
	}

	if (!DirExist(pszPath) && !MkDir(pszPath))
	{
		printf("mkdir failed:%s\n", pszPath);
		return;
	}

	ZeroMemory(_pszFile, 512);
	if (pszFile)
	{
		sprintf_s(_pszFile, sizeof(_pszFile), "%s/%s", pszPath, pszFile);
		return;
	}

	time_t ltime;
	time(&ltime);
	
	struct tm curTime; 
	localtime_s(&curTime, &ltime);
	//tm* curTime = localtime(&ltime);
	sprintf_s(_pszFile, sizeof(_pszFile), "%s/%u_%u_%u.log", pszPath,
		curTime.tm_year + 1900, curTime.tm_mon + 1, curTime.tm_mday);
}

void SetAutoReturn(const bool value)
{
	_autoRtn = value;
}

void ClearLog()
{
	HANDLE hFile = _CreateMyFile(_pszFile);
	if (NULL == hFile)
		return;

	SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
}


bool MkDir(const char* pszPath)
{
#ifdef WIN32
	return 0 == _mkdir(pszPath);
#else
	return 0 == mkdir(pszPath, 0775);
#endif
}

//末尾不能带'/'
bool DirExist(const char* pszPath)
{
	struct stat s;
	int ret = stat(pszPath, &s);
	if( 0 != stat(pszPath, &s) ) 
		return false;

	return (s.st_mode & S_IFDIR) != 0;
}
