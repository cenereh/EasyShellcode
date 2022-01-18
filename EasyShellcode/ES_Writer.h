#pragma once
#include "ES_Executable.h"

class ES_Writer
{
public:
	ES_Writer(ES_Executable* ESExec, std::wstring FileOutput);
	~ES_Writer();

	void WriteToFile();
private:
	FILE* m_fp;
	ES_Executable* m_Executable;
};

