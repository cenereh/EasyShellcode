#include "ES_Writer.h"

#include <iostream>


int main()
{
	std::cout << "Path of the file to turn into C shellcode: ";

	std::wstring FilePath;
	std::wcin >> FilePath;

	if (FilePath.size() == 0)
	{
		std::cout << "\nWrong input: empty string.\n";
	}
	else
	{
		ES_Executable* exec = new ES_Executable();

		bool EncryptShellcode = false;

		std::cout << "Do you wish to encrypt your shellcode? (Type \"no\" if not, type anything if yes.): ";
		std::wstring response;
		std::wcin >> response;

		if (response == std::wstring(L"no") || response == std::wstring(L"NO") || response == std::wstring(L"n") || response == std::wstring(L"N"))
		{
			EncryptShellcode = false;
			std::cout << "\nShellcode will not be encrypted.\n";
		}
		else
		{
			EncryptShellcode = true;
			std::cout << "\nShellcode will be encrypted.\n";
		}

		if (!exec->Init(FilePath, EncryptShellcode))
		{
			std::cout << "\nKernel error occured while opening file: " << GetLastError() << ".\nCheck if the path is correct!\n";
		}
		else
		{
			std::cout << "\nWhere do you want to save your shellcode file? (type \"default\" for default path): ";
			std::wstring ShlPath;
			std::wcin >> ShlPath;

			if (ShlPath == L"default")
			{
				ShlPath = EncryptShellcode ? L"out_shellcode.h" : L"out_shellcode_ne.h";
			}

			ES_Writer* writer = new ES_Writer(exec, ShlPath);

			writer->WriteToFile();

			delete writer;
			delete exec;

			std::wcout << L"\nShellcode written to " + ShlPath + L"!\n";

		}
	}

	getchar();
	return 0;
}