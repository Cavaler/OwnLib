#ifndef __CMAPPING_H
#define __CMAPPING_H

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

class CFileMapping {
public:
	CFileMapping();
	void *Open(const char *szFileName);
	operator const char *();
	HANDLE GetFileHandle();
	HANDLE GetMappingHandle();
	DWORD Size();
	void Close();
	~CFileMapping();
protected:
	HANDLE m_hFile;
	HANDLE m_hMapping;
	void *m_pData;
};

#endif
