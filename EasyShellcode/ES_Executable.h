#pragma once
#include <Windows.h>
#include <string>

#define ES_ENCRYPT_KEYSIZE					32

class ES_Executable
{
public:
	inline ES_Executable() : m_Handle(INVALID_HANDLE_VALUE), m_Buffer(nullptr), m_Size(NULL), m_IsShellcodeEncrypted(false) {}
	~ES_Executable();

	bool Init(std::wstring ExecutableToOpen, bool Encrypt);

	inline BYTE* GetBuffer() { return m_Buffer; }
	inline DWORD_PTR GetSize() { return m_Size; }
	inline BYTE* GetEncryptionKey() { return m_Key; }

	inline bool IsShellcodeEncrypted() { return m_IsShellcodeEncrypted; }
private:
	void Generate32ByteKey();
	void XorEncryptBuffer();

	HANDLE		m_Handle;
	BYTE*		m_Buffer;
	DWORD_PTR	m_Size; 
	BYTE		m_Key[ES_ENCRYPT_KEYSIZE] = {0};
	bool		m_IsShellcodeEncrypted;
};

