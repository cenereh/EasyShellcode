#include "ES_Executable.h"

#include <time.h>

void ES_Executable::Generate32ByteKey()
{
	srand(time(NULL));

	for (int i = 0; i < ES_ENCRYPT_KEYSIZE; i++)
	{
		m_Key[i] = rand() % 0xFF + 0x01;
	}
}

void ES_Executable::XorEncryptBuffer()
{
	int KeyIndex = 0;

	for (DWORD_PTR i = 0; i < m_Size; i++)
	{
		m_Buffer[i] ^= m_Key[KeyIndex];
		KeyIndex++;

		if (KeyIndex == ES_ENCRYPT_KEYSIZE)
			KeyIndex = 0;
	}
}

ES_Executable::~ES_Executable()
{
	VirtualFree(m_Buffer, 0, MEM_RELEASE);
}

bool ES_Executable::Init(std::wstring ExecutableToOpen, bool Encrypt)
{
	m_Handle = CreateFile(ExecutableToOpen.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (m_Handle == INVALID_HANDLE_VALUE)
		return false;

	m_Size = GetFileSize(m_Handle, 0);

	if (!m_Size)
	{
		CloseHandle(m_Handle);
		return false;
	}

	m_Buffer = (BYTE*)VirtualAlloc(0, m_Size, MEM_COMMIT, PAGE_READWRITE);

	if (!m_Buffer)
	{
		CloseHandle(m_Handle);
		return false;
	}

	if (!ReadFile(m_Handle, m_Buffer, m_Size, 0, 0))
	{
		CloseHandle(m_Handle);
		return false;
	}
	else
	{
		CloseHandle(m_Handle);

		if (Encrypt)
		{
			Generate32ByteKey();
			XorEncryptBuffer();

			m_IsShellcodeEncrypted = true;
		}

		return true;
	}
}
