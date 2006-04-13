#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <Directory.h>
#include <vector>
#include <string>
using namespace std;

BOOL DirectoryExists(const char *DirName) {
	int Length=strlen(DirName);
	if (Length==0) return TRUE;
	if ((Length==1)&&(DirName[0]=='\\')) return TRUE;
	if (((Length==3)&&(DirName[1]==':')&&(DirName[2]=='\\'))||
		((Length==2)&&(DirName[1]==':'))) {
		DWORD Drives=GetLogicalDrives();
		char Drive=toupper(DirName[0]);
		return ((Drive>='A')&&(Drive<='Z'))?(Drives&(1<<(Drive-'A'))):FALSE;
	}

	string strPath = DirName;
	if (DirName[Length-1]=='\\') strPath.erase(strPath.end()-1);
	DWORD Attr=GetFileAttributes(strPath.c_str());
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

string DelSlash(const string &strPath) {
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 3) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] == '\\') return strPath.substr(0, strPath.length()-1);
	return strPath;
}

string AddSlash(const string &strPath) {
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 2) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] != '\\') return strPath + '\\';
	return strPath;
}

string CatFile(const string &strPath, const string &strFile) {
	return AddSlash(strPath) + strFile;
}

string GetFileName(const string &strPath) {
	int nPosS = strPath.rfind('\\');
	int nPosC = strPath.rfind(':');
	int nPos = (nPosS == string::npos) ? nPosC :
		(nPosC == string::npos) ? nPosS :
		(nPosS > nPosC) ? nPosS : nPosC;

	return (nPos == string::npos) ? strPath : strPath.substr(nPos + 1);
}

string GetFullFileName(const string &strPath) {
	DWORD dwSize = GetFullPathName(strPath.c_str(), 0, NULL, NULL);
	if (!dwSize) return "";

	vector<char> arrFull(dwSize);
	GetFullPathName(strPath.c_str(), dwSize, &arrFull[0], NULL);
	return &arrFull[0];
}

string GetFullFileName(const string &strPath, const string &strBase) {
	char szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);
	SetCurrentDirectory(strBase.c_str());
	string strFull = GetFullFileName(strPath);
	SetCurrentDirectory(szCurDir);
	return strFull;
}
