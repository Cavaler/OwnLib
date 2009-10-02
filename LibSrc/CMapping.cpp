#include "CMapping.h"

CFileMapping::CFileMapping() : m_hFile(INVALID_HANDLE_VALUE), m_hMapping(NULL), m_pData(NULL), m_dwPos(0) {
}

CFileMapping::CFileMapping(const TCHAR *szFileName) : m_hMapping(NULL), m_pData(NULL), m_dwPos(0) {
	Open(szFileName);
}

CFileMapping::operator bool() {
	return m_pData != NULL;
}

bool CFileMapping::operator !() {
	return m_pData == NULL;
}

void *CFileMapping::Open(const TCHAR *szFileName, bool bWriteable, DWORD dwResize) {
	if (dwResize > 0) bWriteable = true;

	m_hFile = CreateFile(szFileName,
		bWriteable ? GENERIC_READ|GENERIC_WRITE : GENERIC_READ,
		bWriteable ? 0 : FILE_SHARE_READ, NULL,
		bWriteable ? OPEN_ALWAYS : OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hFile == INVALID_HANDLE_VALUE) return NULL;

	if (dwResize > 0) {
		SetFilePointer(m_hFile, dwResize, NULL, FILE_BEGIN);
		SetEndOfFile(m_hFile);
		SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
	}

	if (GetFileSize(m_hFile, NULL) == 0) {		// Cannot map, emulating
		m_pData = (void *)!NULL;
		return m_pData;
	}

	m_hMapping = CreateFileMapping(m_hFile, NULL, (bWriteable ? PAGE_READWRITE : PAGE_READONLY)|SEC_COMMIT, 0, 0, NULL);
	if (m_hMapping == NULL) {
		CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;
		return NULL;
	}
	m_pData = MapViewOfFile(m_hMapping, bWriteable ? FILE_MAP_WRITE : FILE_MAP_READ, 0, 0, 0);
	if (!m_pData) {
		CloseHandle(m_hMapping);m_hMapping = NULL;
		CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;
		return NULL;
	}

	m_dwPos = 0;
	return m_pData;
}

CFileMapping::operator void *() {
	return m_pData;
}

CFileMapping::operator BYTE *() {
	return (BYTE *)m_pData;
}

CFileMapping::operator char *() {
	return (char *)m_pData;
}

CFileMapping::operator wchar_t *() {
	return (wchar_t *)m_pData;
}

HANDLE CFileMapping::GetFileHandle() {
	return m_hFile;
}

HANDLE CFileMapping::GetMappingHandle() {
	return m_hMapping;
}

DWORD CFileMapping::Size() {
	return (m_hFile) ? GetFileSize(m_hFile, NULL) : 0;
}

DWORD CFileMapping::Pos() {
	return m_dwPos;
}

DWORD CFileMapping::Left() {
	return Size()-m_dwPos;
}

bool CFileMapping::Seek(DWORD dwPos) {
	if (dwPos > Size()) return false;
	m_dwPos = dwPos;
	return true;
}

bool CFileMapping::Forward(DWORD dwPos) {
	if (dwPos > Left()) return false;
	m_dwPos += dwPos;
	return true;
}

bool CFileMapping::Read(void *pValue, DWORD dwCount) {
	if (dwCount > Left()) return false;

	if (pValue) memmove(pValue, (BYTE *)m_pData + m_dwPos, dwCount);
	m_dwPos += dwCount;

	return true;
}

int CFileMapping::ReadInt() {
	int nResult = 0;
	Read(nResult);
	return nResult;
}

void CFileMapping::Close() {
	if (m_pData) {UnmapViewOfFile(m_pData);m_pData = NULL;}
	if (m_hMapping != NULL) {CloseHandle(m_hMapping);m_hMapping = NULL;}
	if (m_hFile != INVALID_HANDLE_VALUE) {CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;}
	m_dwPos = 0;
}

CFileMapping::~CFileMapping() {
	Close();
}
