#pragma once
#include <string>


void error_out(std::string file, unsigned int linenum, const char* pMsg=NULL);
#define ErrorOut(msg) error_out(__FILE__, __LINE__, msg);

bool ReadFile(const char* fileName, std::string& outFile);

