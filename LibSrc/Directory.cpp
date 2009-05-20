#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

#include <Directory.h>

BOOL DirectoryExists(const TCHAR *DirName) {
	int Length=_tcslen(DirName);
	if (Length==0) return TRUE;
	if ((Length==1)&&(DirName[0]=='\\')) return TRUE;
	if (((Length==3)&&(DirName[1]==':')&&(DirName[2]=='\\'))||
		((Length==2)&&(DirName[1]==':'))) {
		DWORD Drives=GetLogicalDrives();
		TCHAR Drive=toupper(DirName[0]);
		return ((Drive>='A')&&(Drive<='Z'))?(Drives&(1<<(Drive-'A'))):FALSE;
	}

	tstring strPath = DirName;
	if (DirName[Length-1]=='\\') strPath.erase(strPath.end()-1);
	DWORD Attr=GetFileAttributes(strPath.c_str());
	return ((Attr!=0xFFFFFFFF)&&(Attr&FILE_ATTRIBUTE_DIRECTORY));
}

TCHAR *AddSlash(TCHAR *Path) {
	int Length=_tcslen(Path);
	if (!Length) return Path;
///	if (!Length) {strcpy(Path,".\\");return Path;}
	if ((Length==2)&&(Path[1]==':')) return Path;
	if (Path[Length-1]!='\\') {Path[Length]='\\';Path[Length+1]=0;}
	return Path;
}

TCHAR *DelSlash(TCHAR *Path) {
	int Length=_tcslen(Path);
	if ((Length==1)&&(Path[2]=='\\')) return NULL;
	if ((Length==3)&&(Path[1]==':')&&(Path[2]=='\\')) return NULL;

	if (Path[Length-1]=='\\') Path[Length]=0;
	return Path;
}

BOOL CreateDirectories(const TCHAR *DirName) {
	TCHAR FullName[MAX_PATH];
	if (DirName[0]==0) return TRUE;
	if (!GetFullPathName(DirName,MAX_PATH,FullName,NULL)) return FALSE;
	TCHAR *Rest=FullName;
	do {
		if (*Rest==0) return TRUE;
		Rest=_tcschr(Rest,'\\');
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

BOOL CreateDirectoriesForFile(const TCHAR *FileName) {
	TCHAR *FName=(TCHAR *)_tcsrchr(FileName,'\\');
	if (!FName) return TRUE;
	*FName=0;
	BOOL Result=CreateDirectories(FileName);
	*FName='\\';
	return Result;
}

tstring DelSlash(const tstring &strPath) {
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 3) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] == '\\') return strPath.substr(0, strPath.length()-1);
	return strPath;
}

tstring AddSlash(const tstring &strPath) {
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 2) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] != '\\') return strPath + _T('\\');
	return strPath;
}

tstring CatFile(const tstring &strPath, const tstring &strFile) {
	return AddSlash(strPath) + strFile;
}

tstring GetFileName(const tstring &strPath) {
	int nPosS = strPath.rfind('\\');
	int nPosC = strPath.rfind(':');
	int nPos = (nPosS == tstring::npos) ? nPosC :
		(nPosC == tstring::npos) ? nPosS :
		(nPosS > nPosC) ? nPosS : nPosC;

	return (nPos == tstring::npos) ? strPath : strPath.substr(nPos + 1);
}

tstring GetFullFileName(const tstring &strPath) {
	DWORD dwSize = GetFullPathName(strPath.c_str(), 0, NULL, NULL);
	if (!dwSize) return _T("");

	vector<TCHAR> arrFull(dwSize);
	GetFullPathName(strPath.c_str(), dwSize, &arrFull[0], NULL);
	return &arrFull[0];
}

tstring GetFullFileName(const tstring &strPath, const tstring &strBase) {
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);
	SetCurrentDirectory(strBase.c_str());
	tstring strFull = GetFullFileName(strPath);
	SetCurrentDirectory(szCurDir);
	return strFull;
}

tstring CleanFileName(const tstring &strPath) {
	tstring strResult = strPath;

	for (size_t nPos = 0; nPos < strResult.length(); nPos++) {
		if ((BYTE)(strResult[nPos]) < 0x20) { strResult[nPos] = '_';continue; }
		switch (strResult[nPos]) {
		case '<':	strResult[nPos] = '(';break;
		case '>':	strResult[nPos] = ')';break;
		case '"':	strResult[nPos] = '\'';break;

		case '?': 	case '|':
		case '\\':	case '/':
		case '$':	case '&':
		case '%':	case ':':
		case '*':	strResult[nPos] = '_';break;
		}
	}

	return strResult;
}
