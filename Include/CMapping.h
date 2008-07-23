#ifndef __CMAPPING_H
#define __CMAPPING_H

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

class CFileMapping {
public:
	CFileMapping();
	CFileMapping(const char *szFileName);
	operator bool();
	bool operator !();

	void *Open(const char *szFileName);
	operator void *();
	operator BYTE *();
	operator char *();
	HANDLE GetFileHandle();
	HANDLE GetMappingHandle();
	DWORD Size();
	void Close();
	~CFileMapping();

	DWORD Pos();
	DWORD Left();
	bool  Seek(DWORD dwPos);
	bool  Forward(DWORD dwPos);

	bool Read(void *pValue, DWORD dwCount);
	template<class T> bool Read(T &Value) {
		return Read(&Value, sizeof(Value));
	}
	int ReadInt();

protected:
	HANDLE m_hFile;
	HANDLE m_hMapping;
	void *m_pData;

	DWORD  m_dwPos;
};

#endif
