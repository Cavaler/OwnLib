#include "CMapping.h"

CFileMapping::CFileMapping() : m_hFile(INVALID_HANDLE_VALUE), m_hMapping(NULL), m_pData(NULL) {
}

void *CFileMapping::Open(const char *szFileName) {
	m_hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) return NULL;

	m_hMapping = CreateFileMapping(m_hFile, NULL, PAGE_READONLY|SEC_COMMIT, 0, 0, NULL);
	if (m_hMapping == NULL) {
		CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;
		return NULL;
	}
	m_pData = MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!m_pData) {
		CloseHandle(m_hMapping);m_hMapping = NULL;
		CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;
		return NULL;
	}
	return m_pData;
}

CFileMapping::operator const char *() {
	return (const char *)m_pData;
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

void CFileMapping::Close() {
	if (m_pData) {UnmapViewOfFile(m_pData);m_pData = NULL;}
	if (m_hMapping != NULL) {CloseHandle(m_hMapping);m_hMapping = NULL;}
	if (m_hFile != INVALID_HANDLE_VALUE) {CloseHandle(m_hFile);m_hFile = INVALID_HANDLE_VALUE;}
}

CFileMapping::~CFileMapping() {
	Close();
}
