#include "Charset.h"
#include <windows.h>

//windows版本
//C语言提供的转换函数为mbstowcs()/wcstombs()

bool ansi2unicode(const char *inbuf, char **outbuf)
{
	const int nLen = MultiByteToWideChar( CP_ACP,
						0,
						inbuf,
						-1,
						NULL,
						0 );

	if (nLen <= 0)
		return false;

	const int size = (nLen+1) * sizeof(wchar_t);
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	MultiByteToWideChar( CP_ACP,
						0,
						inbuf,
						-1,
						(LPWSTR)(*outbuf),
						nLen);  
	return true;
}


bool unicode2ansi( const char *inbuf, char **outbuf)
{
	const int nLen = WideCharToMultiByte( CP_ACP,
										0,
										(LPWSTR)inbuf,
										-1,
										NULL,
										0,
										NULL,
										NULL );
	if (nLen <= 0)
		return false;

	const int size = nLen + 1;
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	WideCharToMultiByte( CP_ACP,
							0,
							(LPWSTR)inbuf,
							-1,
							*outbuf,
							nLen,
							NULL,
							NULL );
	return true;
}


bool utf82unicode( const char *inbuf, char **outbuf)
{
	const int nLen = MultiByteToWideChar( CP_UTF8,
											0,
											inbuf,
											-1,
											NULL,
											0 );  
	if (nLen <= 0)
		return false;

	const int size = (nLen + 1) * sizeof(wchar_t);
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	MultiByteToWideChar( CP_UTF8,
							0,
							inbuf,
							-1,
							(LPWSTR)(*outbuf),
							nLen);  
	return true;
}

bool unicode2utf8( const char * inbuf, char **outbuf)
{
	const int nLen = WideCharToMultiByte( CP_UTF8,
									0,
									(LPWSTR)inbuf,
									-1,
									NULL,
									0,
									NULL,
									NULL );
	if (nLen <= 0)
		return false;

	const int size = nLen + 1;
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	WideCharToMultiByte( CP_UTF8,
							0,
							(LPWSTR)inbuf,
							-1,
							*outbuf,
							nLen,
							NULL,
							NULL );
	return true;
}


//ansi先转unicode 再转utf8
bool ansi2utf8( const char *inbuf, char **outbuf)
{
	char *temp;
	if (!ansi2unicode(inbuf, &temp))
		return false;
	return unicode2utf8(temp, outbuf);

	//以下废弃 作为参考
	int nLen = MultiByteToWideChar( CP_ACP,
											0,
											inbuf,
											-1,
											NULL,
											0 );  

	char *buf;
	int size = (nLen + 1) * sizeof(wchar_t);
	buf = (char*)malloc(size);
	memset(buf, 0, size);

    MultiByteToWideChar(CP_ACP, 0, inbuf, -1, (LPWSTR)buf, nLen);  

    nLen = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)buf,  -1, NULL, 0, NULL, NULL);  
	size = nLen + 1;
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	WideCharToMultiByte( CP_UTF8,
							0,
							(LPWSTR)buf,
							-1,
							*outbuf,
							nLen, NULL, NULL);  
}


//utf8先转unicode 再转ansi
bool utf82ansi( const char *inbuf, char **outbuf)
{
	char *temp;
	if (!utf82unicode(inbuf, &temp))
		return false;
	return unicode2ansi(temp, outbuf);


	//以下废弃 作为参考
	int nLen = MultiByteToWideChar( CP_UTF8,
											0,
											inbuf,
											-1,
											NULL,
											0 );  

	char *buf;
	int size = (nLen + 1) * sizeof(wchar_t);
	buf = (char*)malloc(size);
	memset(buf, 0, size);

    MultiByteToWideChar(CP_UTF8, 0, inbuf, -1, (LPWSTR)buf, nLen);  

    nLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)buf,  -1, NULL, 0, NULL, NULL);  
	size = nLen + 1;
	*outbuf = (char*)malloc(size);
	memset(*outbuf, 0, size);

	WideCharToMultiByte( CP_ACP,
							0,
							(LPWSTR)buf,
							-1,
							*outbuf,
							nLen, NULL, NULL);  
}





