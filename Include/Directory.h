#ifndef __DIRECTORY_H
#define __DIRECTORY_H

BOOL DirectoryExists(const char *DirName);
char *AddSlash(char *Path);				// Returns same pointer
char *DelSlash(char *Path);				// Returns same pointer or NULL in case 'D:\' or '\'
BOOL CreateDirectories(const char *DirName);
BOOL CreateDirectoriesForFile(const char *FileName);

#ifdef _STRING_
std::string DelSlash(const std::string &strPath);
std::string AddSlash(const std::string &strPath);
std::string CatFile(const std::string &strPath, const std::string &strFile);
std::string GetFileName(const std::string &strPath);
std::string GetFullFileName(const std::string &strPath);
std::string GetFullFileName(const std::string &strPath, const std::string &strBase);
#endif

#endif
