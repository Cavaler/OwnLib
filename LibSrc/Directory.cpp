#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace std;

#include <Directory.h>
#include <StringEx.h>

BOOL DirectoryExists(const TCHAR *DirName)
{
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

TCHAR *AddSlash(TCHAR *Path)
{
	int Length=_tcslen(Path);
	if (!Length) return Path;
///	if (!Length) {strcpy(Path,".\\");return Path;}
	if ((Length==2)&&(Path[1]==':')) return Path;
	if (Path[Length-1]!='\\') {Path[Length]='\\';Path[Length+1]=0;}
	return Path;
}

TCHAR *DelSlash(TCHAR *Path)
{
	int Length=_tcslen(Path);
	if ((Length==1)&&(Path[2]=='\\')) return NULL;
	if ((Length==3)&&(Path[1]==':')&&(Path[2]=='\\')) return NULL;

	if (Path[Length-1]=='\\') Path[Length]=0;
	return Path;
}

LPCTSTR GetNameOnly(LPCTSTR szPath)
{
	const TCHAR *szNameOnly = _tcsrchr(szPath, '\\');
	return (szNameOnly) ? szNameOnly+1 : szPath;
}

BOOL CreateDirectories(const TCHAR *DirName)
{
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

BOOL CreateDirectoriesForFile(const TCHAR *FileName)
{
	TCHAR *FName=(TCHAR *)_tcsrchr(FileName,'\\');
	if (!FName) return TRUE;
	*FName=0;
	BOOL Result=CreateDirectories(FileName);
	*FName='\\';
	return Result;
}

tstring DelSlash(const tstring &strPath)
{
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 3) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] == '\\') return strPath.substr(0, strPath.length()-1);
	return strPath;
}

tstring AddSlash(const tstring &strPath)
{
	if (strPath.length() == 0) return strPath;
	if ((strPath.length() == 2) && (strPath[1] == ':')) return strPath;
	if (strPath[strPath.length() - 1] != '\\') return strPath + _T('\\');
	return strPath;
}

tstring CatFile(const tstring &strPath, const tstring &strFile)
{
	return AddSlash(strPath) + strFile;
}

tstring GetFileName(const tstring &strPath)
{
	int nPosS = strPath.rfind('\\');
	int nPosC = strPath.rfind(':');
	int nPos = (nPosS == tstring::npos) ? nPosC :
		(nPosC == tstring::npos) ? nPosS :
		(nPosS > nPosC) ? nPosS : nPosC;

	return (nPos == tstring::npos) ? strPath : strPath.substr(nPos + 1);
}

tstring GetPath(const tstring &strPath)
{
	int nPosS = strPath.rfind('\\');
	int nPosC = strPath.rfind(':');
	int nPos = (nPosS == tstring::npos) ? nPosC :
		(nPosC == tstring::npos) ? nPosS :
		(nPosS > nPosC) ? nPosS : nPosC;

	return (nPos == tstring::npos) ? tstring() : strPath.substr(0, nPos);
}

tstring GetFullFileName(const tstring &strPath)
{
	wstring wstrPath = StrToUnicode(strPath, CP_OEMCP);

	DWORD dwSize = GetFullPathNameW(wstrPath.c_str(), 0, NULL, NULL);
	if (!dwSize) return _T("");

	vector<wchar_t> arrFull(dwSize);
	GetFullPathNameW(wstrPath.c_str(), dwSize, &arrFull[0], NULL);

#ifdef UNICODE
	return &arrFull[0];
#else
	return StrFromUnicode(&arrFull[0], CP_OEMCP);
#endif
}

tstring GetFullFileName(const tstring &strPath, const tstring &strBase)
{
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);
	SetCurrentDirectory(strBase.c_str());
	tstring strFull = GetFullFileName(strPath);
	SetCurrentDirectory(szCurDir);
	return strFull;
}

tstring CleanFileName(const tstring &strPath)
{
	tstring strResult = strPath;

	for (size_t nPos = 0; nPos < strResult.length(); nPos++) {
#ifdef UNICODE
		if ((WORD)(strResult[nPos]) < 0x20) { strResult[nPos] = '_';continue; }
#else
		if ((BYTE)(strResult[nPos]) < 0x20) { strResult[nPos] = '_';continue; }
#endif
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

const LPCWSTR g_szLongPrefix = L"\\\\?\\";
const LPCWSTR g_szUNCPrefix  = L"\\\\?\\UNC\\";

std::tstring ExtendedFileName(const std::tstring &strPath)
{
#ifdef UNICODE
	return (strPath.substr(0, 4) == g_szLongPrefix) ? strPath :
		   (strPath.substr(0, 2) == L"\\\\") ? g_szUNCPrefix + strPath.substr(2) : g_szLongPrefix + strPath;
#else
	return strPath;
#endif
}

std::tstring FullExtendedFileName(const std::tstring &strPath)
{
	return ExtendedFileName(GetFullFileName(strPath));
}

std::tstring ContractedFileName(const std::tstring &strPath)
{
#ifdef UNICODE
	return (strPath.substr(0, 8) == g_szUNCPrefix) ? L"\\\\" + strPath.substr(8) :
		   (strPath.substr(0, 4) == g_szLongPrefix) ? strPath.substr(4) : strPath;
#else
	return strPath;
#endif
}
