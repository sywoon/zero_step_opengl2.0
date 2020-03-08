#pragma once 

void Log(const char* pszFmt, ...);

void InitLogFile(const char* pszPath = 0, const char* pszFile = 0);
void SetAutoReturn(const bool value);
void ClearLog();

bool MkDir(const char* pszPath);
bool DirExist(const char* pszPath);
