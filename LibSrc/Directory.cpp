#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <Directory.h>

BOOL DirectoryExists(char *DirName) {
	int Length=strlen(DirName);
	if (Length==0) return TRUE;
	if ((Length==1)&&(DirName[0]=='\\')) return TRUE;
	if (((Length==3)&&(DirName[1]==':')&&(DirName[2]=='\\'))||
		((Length==2)&&(DirName[1]==':'))) {
		DWORD Drives=GetLogicalDrives();
		char Drive=toupper(DirName[0]);
		return ((Drive>='A')&&(Drive<='Z'))?(Drives&(1<<(Drive-'A'))):FALSE;
	}

	BOOL Slashed=FALSE;
	if (DirName[Length-1]=='\\') {Slashed=TRUE;DirName[Length-1]=0;}
	DWORD Attr=GetFileAttributes(DirName);
	if (Slashed) DirName[Length-1]='\\';
	return ((Attr!=0xFFFFFFFF)&&(Attr&FILE_ATTRIBUTE_DIRECTORY));
}

char *AddSlash(char *Path) {
	int Length=strlen(Path);
	if (!Length) return Path;
///	if (!Length) {strcpy(Path,".\\");return Path;}
	if ((Length==2)&&(Path[1]==':')) return Path;
	if (Path[Length-1]!='\\') {Path[Length]='\\';Path[Length+1]=0;}
	return Path;
}

char *DelSlash(char *Path) {
	int Length=strlen(Path);
	if ((Length==1)&&(Path[2]=='\\')) return NULL;
	if ((Length==3)&&(Path[1]==':')&&(Path[2]=='\\')) return NULL;

	if (Path[Length-1]=='\\') Path[Length]=0;
	return Path;
}

BOOL CreateDirectories(const char *DirName) {
	char FullName[MAX_PATH];
	if (DirName[0]==0) return TRUE;
	if (!GetFullPathName(DirName,MAX_PATH,FullName,NULL)) return FALSE;
	char *Rest=FullName;
	do {
		if (*Rest==0) return TRUE;
		Rest=strchr(Rest,'\\');
		if (!Rest) {
			if (DirectoryExists(FullName)) return TRUE;
			if (!CreateDirectory(FullName,NULL)) return FALSE;
			return TRUE;
		} else {
			*Rest=0;
			if (!DirectoryExists(FullName)) {
				if (!CreateDirectory(FullName,NULL)) {*Rest='\\';return FALSE;}
			}
			*Rest='\\';Rest++;
		}
	} while (Rest);
	return TRUE;
}

BOOL CreateDirectoriesForFile(const char *FileName) {
	char *FName=strrchr(FileName,'\\');
	if (!FName) return TRUE;
	*FName=0;
	BOOL Result=CreateDirectories(FileName);
	*FName='\\';
	return Result;
}
