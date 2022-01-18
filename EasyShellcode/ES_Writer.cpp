#define _CRT_SECURE_NO_WARNINGS

#include "ES_Writer.h"
#include <stdio.h>

ES_Writer::ES_Writer(ES_Executable* ESExec, std::wstring FileOutput)
	: m_fp(nullptr), m_Executable(ESExec)
{
	m_fp = _wfopen(FileOutput.c_str(), L"w");
}

ES_Writer::~ES_Writer()
{
	fclose(m_fp);
}

void ES_Writer::WriteToFile()
{
	bool WriteXorKeyAndAlgo = m_Executable->IsShellcodeEncrypted();
	DWORD_PTR XorBytesWritten = 0;

	DWORD_PTR FileSize = m_Executable->GetSize();
	fprintf(m_fp, "constexpr unsigned long long kESShellcodeSize = %ull;\n", FileSize);

	if (WriteXorKeyAndAlgo)
	{
		fprintf(m_fp, "#define ES_SHELLCODE_ENCRYPTED\n\n");
		BYTE* XorKey = m_Executable->GetEncryptionKey();

		// Write shellcode data and key
		fprintf(m_fp, "constexpr bool kESHasShellcodeBeenEncrypted = true;\n\n");

		fprintf(m_fp, "unsigned char xorKey[%d] = { ", ES_ENCRYPT_KEYSIZE);

		for (int i = 0; i < ES_ENCRYPT_KEYSIZE; i++)
		{
			fprintf(m_fp, "0x%02X", XorKey[i]);

			if (XorBytesWritten < ES_ENCRYPT_KEYSIZE)
				fprintf(m_fp, ", ");
		}

		fprintf(m_fp, " };\n\n");

		

	}
	else
	{
		fprintf(m_fp, "constexpr bool kESHasShellcodeBeenEncrypted = false;\n\n");
	}

	DWORD_PTR BytesWritten = 0, BytesWrittenOnSingleRun = 0;
	BYTE* File = m_Executable->GetBuffer();

	fprintf(m_fp, "unsigned char shellcode[kESShellcodeSize] = {\n\t");

	for (DWORD_PTR i = 0; i < FileSize; i++)
	{
		fprintf(m_fp, "0x%02X", File[i]);
		BytesWritten++, BytesWrittenOnSingleRun++;

		if (BytesWritten < FileSize)
			fprintf(m_fp, ",");

		if (BytesWrittenOnSingleRun == 12)
		{
			fprintf(m_fp, "\n\t");
			BytesWrittenOnSingleRun = 0;
		}
		else
			fprintf(m_fp, " ");
	}

	fprintf(m_fp, "\n};");

	// Write shellcode algo

	std::string xor_C_code = "\n\nvoid ESXorDecryptShellcode()\n" +
							 std::string("{\n") +
							 std::string("#ifndef ES_SHELLCODE_ENCRYPTED\n") +
							 std::string("	return;\n") +
							 std::string("#else\n") +
							 std::string("	int KeyIndex = 0;\n\n") +
							 std::string("	for (unsigned long long i = 0; i < kESShellcodeSize; i++)\n") +
							 std::string("	{\n") +
							 std::string("		shellcode[i] ^= xorKey[KeyIndex];\n") +
							 std::string("		KeyIndex++;\n\n") +
							 std::string("		if (KeyIndex == %d)\n") +
							 std::string("			KeyIndex = 0;\n\n") +
							 std::string("	}\n") +
							 std::string("	return;\n") +
							 std::string("#endif\n") +
							 std::string("}");
								
	fprintf(m_fp, xor_C_code.c_str(), ES_ENCRYPT_KEYSIZE);

	return;
}
