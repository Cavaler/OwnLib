#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <vector>
#include <string>
using namespace std;
#include <tchar.h>

#include <EasyReg.h>

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName,TCHAR *pszBuffer,DWORD dwBufSize,const TCHAR *pszDefault) {
	DWORD Type;
	LONG Res=RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(LPBYTE)pszBuffer,&dwBufSize);
	if ((Res!=ERROR_SUCCESS)||(Type!=REG_SZ)) {
		if (pszDefault) _tcsncpy(pszBuffer, pszDefault, dwBufSize/sizeof(TCHAR)); else *pszBuffer=0;
	}
}

DWORD QueryRegSizeType(HKEY hKey, const TCHAR *pszKeyName, DWORD *pdwType = NULL) {
	DWORD Type;
	DWORD Size;
	LONG Res = RegQueryValueEx(hKey,pszKeyName,NULL,&Type,NULL,&Size);
	if (Res != ERROR_SUCCESS) {
		if (pdwType) *pdwType = REG_NONE;
		return 0;
	}
	
	if (pdwType) *pdwType = Type;
	return Size;
}

bool IsStringType(DWORD dwType) {
	return (dwType == REG_SZ) || (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ) || (dwType == REG_BINARY);
}

void AllocAndQueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName,TCHAR **ppszBuffer,DWORD *BufSize,const TCHAR *pszDefault) {
	DWORD Type;
	DWORD Size = QueryRegSizeType(hKey, pszKeyName, &Type);
	if (!IsStringType(Type)) {
		if (pszDefault) {
			*ppszBuffer=_tcsdup(pszDefault);
			if (BufSize) *BufSize=_tcslen(pszDefault)*sizeof(TCHAR);
		} else {
			*ppszBuffer=NULL;
			if (BufSize) *BufSize=0;
		}
		return;
	}

	if (Type == REG_BINARY) {
		(*ppszBuffer)=(TCHAR *)malloc(Size+sizeof(TCHAR));
		RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppszBuffer,&Size);
		(*ppszBuffer)[Size/sizeof(TCHAR)] = 0;
		if (BufSize) *BufSize=Size+1;
	} else {
		(*ppszBuffer)=(TCHAR *)malloc(Size);
		RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppszBuffer,&Size);
		if (BufSize) *BufSize=Size;
	}
}

#ifndef _UNICODE

typedef LONG ( APIENTRY* QueryValueExW)(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
typedef LONG ( APIENTRY* SetValueExW)(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

QueryValueExW g_QueryValueExW = NULL;
SetValueExW g_SetValueExW = NULL;
HMODULE g_hAdvAPI32DLL = NULL;

void LoadProcs() {
	if (!g_hAdvAPI32DLL)
		g_hAdvAPI32DLL = LoadLibraryA("advapi32.dll");
	if (!g_QueryValueExW)
		g_QueryValueExW = (QueryValueExW)GetProcAddress(g_hAdvAPI32DLL, "RegQueryValueExW");
	if (!g_SetValueExW)
		g_SetValueExW = (SetValueExW)GetProcAddress(g_hAdvAPI32DLL, "RegSetValueExW");
}

wstring wstrfromsz(const char *szKeyName) {
	wstring wstr(strlen(szKeyName)+1, 0);
	MultiByteToWideChar(CP_ACP, 0, szKeyName, -1, (wchar_t *)wstr.data(), wstr.length());
	return wstr;
}

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wchar_t *pwszBuffer, DWORD dwBufSize, const wchar_t *pwszDefault) {
	dwBufSize *= 2;
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		DWORD Type;
		LONG Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, (LPBYTE)pwszBuffer, &dwBufSize);
		if ((Res!=ERROR_SUCCESS) || !IsStringType(Type)) {
			if (pwszDefault) wcsncpy(pwszBuffer, pwszDefault, dwBufSize); else *pwszBuffer=0;
		}
	} else {							// Windows 95/98/ME
		DWORD Type;
		LONG Res=RegQueryValueEx(hKey, pszKeyName, NULL, &Type, (LPBYTE)pwszBuffer, &dwBufSize);
		if ((Res!=ERROR_SUCCESS) || (Type!=REG_BINARY)) {
			if (pwszDefault) wcsncpy(pwszBuffer, pwszDefault, dwBufSize); else *pwszBuffer=0;
		}
	}
}

void AllocAndQueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wchar_t **ppwszBuffer, DWORD *pdwBufSize, const wchar_t *pwszDefault) {
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		DWORD Type;
		DWORD Size;
		LONG Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, NULL, &Size);
		if ((Res!=ERROR_SUCCESS) || !IsStringType(Type)) {
			if (pwszDefault) {
				*ppwszBuffer=_wcsdup(pwszDefault);
				if (pdwBufSize) *pdwBufSize=wcslen(pwszDefault);
			} else {
				*ppwszBuffer=NULL;
				if (pdwBufSize) *pdwBufSize=0;
			}
			return;
		}

		if (Type == REG_BINARY) {
			*ppwszBuffer = (wchar_t *)malloc(Size*2+2);
			Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, (LPBYTE)*ppwszBuffer, &Size);
			(*ppwszBuffer)[Size] = 0;
			if (pdwBufSize) *pdwBufSize=Size+1;
		} else {
			*ppwszBuffer = (wchar_t *)malloc(Size*2);
			Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, (LPBYTE)*ppwszBuffer, &Size);
			if (pdwBufSize) *pdwBufSize=Size;
		}
	} else {							// Windows 95/98/ME
		DWORD Type;
		DWORD Size;
		LONG Res=RegQueryValueEx(hKey,pszKeyName,NULL,&Type,NULL,&Size);
		if ((Res!=ERROR_SUCCESS) || (Type!=REG_BINARY)) {
			if (pwszDefault) {
				*ppwszBuffer=_wcsdup(pwszDefault);
				if (pdwBufSize) *pdwBufSize=wcslen(pwszDefault);
			} else {
				*ppwszBuffer=NULL;
				if (pdwBufSize) *pdwBufSize=0;
			}
			return;
		}

		if (Type == REG_BINARY) {
			*ppwszBuffer = (wchar_t *)malloc(Size*2+2);
			RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppwszBuffer,&Size);
			(*ppwszBuffer)[Size] = 0;
			if (pdwBufSize) *pdwBufSize=Size+1;
		} else {
			*ppwszBuffer = (wchar_t *)malloc(Size*2);
			RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppwszBuffer,&Size);
			if (pdwBufSize) *pdwBufSize=Size;
		}
	}
}

#endif

void QueryRegBoolValue(HKEY hKey, const TCHAR *pszKeyName, BOOL *pbBuffer, BOOL bDefault) {
	QueryRegIntValue(hKey, pszKeyName, pbBuffer, bDefault, 0, 1);
}

void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, __int64 *piBuffer, __int64 iDefault) {
	DWORD dwBufSize=sizeof(__int64),dwType;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)piBuffer,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_BINARY)) *piBuffer=iDefault;
}

void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, __int64 *piBuffer, __int64 iDefault, __int64 iMin, __int64 iMax) {
	DWORD dwBufSize=sizeof(__int64),dwType;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)piBuffer,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_BINARY) || (*piBuffer<(DWORD)iMin) || (*piBuffer>(DWORD)iMax)) *piBuffer=iDefault;
}

void QueryRegBoolValue(HKEY hKey, const TCHAR *pszKeyName, bool *pbBuffer, bool bDefault) {
	int iValue;
	QueryRegIntValue(hKey, pszKeyName, &iValue, bDefault?1:0, 0, 1);
	*pbBuffer = iValue!=0;
}

void SetRegStringValue(HKEY hKey,const TCHAR *pszKeyName,const TCHAR *szValue) {
	RegSetValueEx(hKey,pszKeyName,0,REG_SZ,(LPBYTE)szValue,(_tcslen(szValue)+1)*sizeof(TCHAR));
}

#ifndef _UNICODE
void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const wchar_t *pwszValue) {
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		g_SetValueExW(hKey,wstrfromsz(pszKeyName).c_str(),0,REG_SZ,(LPBYTE)pwszValue,wcslen(pwszValue)*2+2);
	} else {							// Windows 95/98/ME
		RegSetValueEx(hKey,pszKeyName,0,REG_BINARY,(LPBYTE)pwszValue,wcslen(pwszValue)*2+2);
	}
}
#endif

void SetRegIntValue(HKEY hKey,const TCHAR *pszKeyName,int iValue) {
	RegSetValueEx(hKey,pszKeyName,0,REG_DWORD,(LPBYTE)&iValue,sizeof(int));
}

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, tstring &strBuffer, const TCHAR *pszDefault) {
	TCHAR *szValue = NULL;
	DWORD dwSize;
	AllocAndQueryRegStringValue(hKey, pszKeyName, &szValue, &dwSize, pszDefault);
	strBuffer = (szValue && (dwSize > 0)) ? tstring(szValue, dwSize-1) : _T("");
	free(szValue);
}

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, tstring &strBuffer, const tstring &strDefault) {
	QueryRegStringValue(hKey, pszKeyName, strBuffer, strDefault.c_str());
}

void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const tstring &strValue) {
	SetRegStringValue(hKey, pszKeyName, strValue.c_str());
}

#ifndef _UNICODE

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wstring &wstrBuffer, const wchar_t *pwszDefault) {
	wchar_t *wszValue = NULL;
	DWORD dwSize;
	AllocAndQueryRegStringValue(hKey, pszKeyName, &wszValue, &dwSize, pwszDefault);
	wstrBuffer = (wszValue && (dwSize > 0)) ? wstring(wszValue, dwSize-1) : L"";
	free(wszValue);
}

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wstring &wstrBuffer, const wstring &wstrDefault) {
	QueryRegStringValue(hKey, pszKeyName, wstrBuffer, wstrDefault.c_str());
}

void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const wstring &wstrValue) {
	SetRegStringValue(hKey, pszKeyName, wstrValue.c_str());
}

#endif

void QueryRegBinaryValue(HKEY hKey, const TCHAR *pszKeyName, std::vector<BYTE> &arrData) {
	DWORD dwType, dwBufSize=0;
	LONG lRes = RegQueryValueEx(hKey, pszKeyName, NULL, &dwType, NULL, &dwBufSize);
	if (((lRes!=ERROR_SUCCESS) && (lRes!=ERROR_MORE_DATA)) || (dwType!=REG_BINARY) || (dwBufSize == 0)) {
		arrData.clear();
		return;
	}

	arrData.resize(dwBufSize);
	RegQueryValueEx(hKey, pszKeyName, NULL, &dwType, &arrData[0], &dwBufSize);
}

void SetRegBinaryValue(HKEY hKey, const TCHAR *pszKeyName, const std::vector<BYTE> &arrData) {
	RegSetValueEx(hKey, pszKeyName, 0, REG_BINARY, arrData.empty() ? NULL : &arrData[0], arrData.size());
}

void SetRegBinaryValue(HKEY hKey, const TCHAR *pszKeyName, const void *pData,  int nLength) {
	RegSetValueEx(hKey, pszKeyName, 0, REG_BINARY, (LPBYTE)pData, nLength);
}

HKEY RegOpenSubkey(HKEY hKey, const TCHAR *pszKeyName) {
	HKEY hSubKey;
	if (RegOpenKeyEx(hKey, pszKeyName, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS) return (HKEY)NULL;
	return hSubKey;
}

HKEY RegCreateSubkey(HKEY hKey, const TCHAR *pszKeyName) {
	HKEY hSubKey;
	if (RegCreateKeyEx(hKey, pszKeyName, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, NULL) != ERROR_SUCCESS) return (HKEY)NULL;
	return hSubKey;
}

void RegDeleteAllSubkeys(HKEY hKey) {
	TCHAR szCurrentKey[256];

	DWORD dwIndex = 0;
	do {
//		FILETIME ftTime;
		DWORD dwcbCurrentKey = sizeof(szCurrentKey);
		if (RegEnumKeyEx(hKey, dwIndex, szCurrentKey, &dwcbCurrentKey, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) break;
		if (SHDeleteKey(hKey, szCurrentKey) != ERROR_SUCCESS) dwIndex++;
	} while (TRUE);
}

void RegDeleteAllValues(HKEY hKey) {
	TCHAR szCurrentKey[256];

	DWORD dwIndex = 0;
	do {
		DWORD dwcbCurrentKey = sizeof(szCurrentKey);
		if (RegEnumValue(hKey, dwIndex, szCurrentKey, &dwcbCurrentKey, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) break;
		if (RegDeleteValue(hKey, szCurrentKey) != ERROR_SUCCESS) dwIndex++;
	} while (TRUE);
}
