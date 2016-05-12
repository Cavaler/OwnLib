#ifndef __DIRECTORY_H
#define __DIRECTORY_H

BOOL DirectoryExists(const TCHAR *DirName);
TCHAR *AddSlash(TCHAR *Path);				// Returns same pointer
TCHAR *DelSlash(TCHAR *Path);				// Returns same pointer or NULL in case 'D:\' or '\'
BOOL CreateDirectories(const TCHAR *DirName);
BOOL CreateDirectoriesForFile(const TCHAR *FileName);
LPCTSTR GetNameOnly(LPCTSTR szPath);

class CCurrentDirectoryBackup
{
public:
	CCurrentDirectoryBackup();
	CCurrentDirectoryBackup(const TCHAR *szNewDir);
	~CCurrentDirectoryBackup();
protected:
	TCHAR m_szCurDir[MAX_PATH];
};

__if_exists(std::string) {
#include "tstring.h"
std::tstring DelSlash(const std::tstring &strPath);
std::tstring AddSlash(const std::tstring &strPath);
std::tstring CatFile(const std::tstring &strPath, const std::tstring &strFile);

std::tstring GetFileName(const std::tstring &strPath);
std::tstring GetFileNameOnly(const std::tstring &strPath, bool bLongExt = true);
std::tstring GetFileExtension(const std::tstring &strPath, bool bLongExt = true);
std::tstring GetPath(const std::tstring &strPath);

std::tstring GetFullFileName(const std::tstring &strPath);
std::tstring GetFullFileName(const std::tstring &strPath, const std::tstring &strBase);

std::tstring CleanFileName(const std::tstring &strPath);

std::tstring ExtendedFileName(const std::tstring &strPath);
std::tstring FullExtendedFileName(const std::tstring &strPath);
std::tstring ContractedFileName(const std::tstring &strPath);
}

#endif
