#include "Util.h"
#include <fstream>
#include "Log.h"


using namespace std;

void error_out(std::string file, unsigned int linenum, const char* pMsg)
{
	Log("file:%s line:%d msg:%s", file.c_str(), linenum, pMsg);
}

bool ReadFile(const char* fileName, std::string& outFile)
{
	ifstream f(fileName);
	bool ret = false;

	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else
	{
		ErrorOut(fileName);
	}
	return ret;
}




