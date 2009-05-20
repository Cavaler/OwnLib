#ifndef __DIRECTORY_H
#define __DIRECTORY_H

BOOL DirectoryExists(const TCHAR *DirName);
TCHAR *AddSlash(TCHAR *Path);				// Returns same pointer
TCHAR *DelSlash(TCHAR *Path);				// Returns same pointer or NULL in case 'D:\' or '\'
BOOL CreateDirectories(const TCHAR *DirName);
BOOL CreateDirectoriesForFile(const TCHAR *FileName);

__if_exists(std::string) {
#include "tstring.h"
std::tstring DelSlash(const std::tstring &strPath);
std::tstring AddSlash(const std::tstring &strPath);
std::tstring CatFile(const std::tstring &strPath, const std::tstring &strFile);
std::tstring GetFileName(const std::tstring &strPath);
std::tstring GetFullFileName(const std::tstring &strPath);
std::tstring GetFullFileName(const std::tstring &strPath, const std::tstring &strBase);

std::tstring CleanFileName(const std::tstring &strPath);
}

#endif
