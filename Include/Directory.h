#ifndef __DIRECTORY_H
#define __DIRECTORY_H

BOOL DirectoryExists(char *DirName);
char *AddSlash(char *Path);				// Returns same pointer
char *DelSlash(char *Path);				// Returns same pointer or NULL in case 'D:\' or '\'
BOOL CreateDirectories(const char *DirName);
BOOL CreateDirectoriesForFile(const char *FileName);

#endif
