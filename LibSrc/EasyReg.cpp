#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <EasyReg.h>

#include <vector>
#include <string>
using namespace std;

void QueryRegStringValue(HKEY hKey, const char *pszKeyName,char *pszBuffer,DWORD dwBufSize,const char *pszDefault) {
	DWORD Type;
	LONG Res=RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(LPBYTE)pszBuffer,&dwBufSize);
	if ((Res!=ERROR_SUCCESS)||(Type!=REG_SZ)) {
		if (pszDefault) strncpy(pszBuffer, pszDefault, dwBufSize); else *pszBuffer=0;
	}
}

void AllocAndQueryRegStringValue(HKEY hKey, const char *pszKeyName,char **ppszBuffer,DWORD *BufSize,const char *pszDefault) {
	DWORD Type;
	DWORD Size;
	LONG Res=RegQueryValueEx(hKey,pszKeyName,NULL,&Type,NULL,&Size);
	if ((Res!=ERROR_SUCCESS)||(Type!=REG_SZ)) {
		if (pszDefault) {
			*ppszBuffer=_strdup(pszDefault);
			if (BufSize) *BufSize=strlen(pszDefault);
		} else {
			*ppszBuffer=NULL;
			if (BufSize) *BufSize=0;
		}
		return;
	}
	(*ppszBuffer)=(char *)malloc(Size);
	RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppszBuffer,&Size);
	if (BufSize) *BufSize=Size;
}

typedef LONG ( APIENTRY* QueryValueExW)(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
typedef LONG ( APIENTRY* SetValueExW)(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData);

QueryValueExW g_QueryValueExW = NULL;
SetValueExW g_SetValueExW = NULL;
HMODULE g_hAdvAPI32DLL = NULL;

void LoadProcs() {
	if (!g_hAdvAPI32DLL)
		g_hAdvAPI32DLL = LoadLibrary("advapi32.dll");
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

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wchar_t *pwszBuffer, DWORD dwBufSize, const wchar_t *pwszDefault) {
	dwBufSize *= 2;
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		DWORD Type;
		LONG Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, (LPBYTE)pwszBuffer, &dwBufSize);
		if ((Res!=ERROR_SUCCESS) || (Type!=REG_SZ)) {
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

void AllocAndQueryRegStringValue(HKEY hKey, const char *pszKeyName, wchar_t **ppwszBuffer, DWORD *pdwBufSize, const wchar_t *pwszDefault) {
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		DWORD Type;
		DWORD Size;
		LONG Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, NULL, &Size);
		if ((Res!=ERROR_SUCCESS) || (Type!=REG_SZ)) {
			if (pwszDefault) {
				*ppwszBuffer=_wcsdup(pwszDefault);
				if (pdwBufSize) *pdwBufSize=wcslen(pwszDefault);
			} else {
				*ppwszBuffer=NULL;
				if (pdwBufSize) *pdwBufSize=0;
			}
			return;
		}

		*ppwszBuffer = (wchar_t *)malloc(Size*2);
		Res=g_QueryValueExW(hKey, wstrfromsz(pszKeyName).c_str(), NULL, &Type, (LPBYTE)*ppwszBuffer, &Size);
		if (pdwBufSize) *pdwBufSize=Size;
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
		(*ppwszBuffer)=(wchar_t *)malloc(Size*2);
		RegQueryValueEx(hKey,pszKeyName,NULL,&Type,(BYTE *)*ppwszBuffer,&Size);
		if (pdwBufSize) *pdwBufSize=Size;
	}
}

void QueryRegBoolValue(HKEY hKey, const char *pszKeyName, BOOL *pbBuffer, BOOL bDefault) {
	QueryRegIntValue(hKey, pszKeyName, pbBuffer, bDefault, 0, 1);
}

void QueryRegIntValue(HKEY hKey, const char *pszKeyName, __int64 *piBuffer, __int64 iDefault) {
	DWORD dwBufSize=sizeof(__int64),dwType;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)piBuffer,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_BINARY)) *piBuffer=iDefault;
}

void QueryRegIntValue(HKEY hKey, const char *pszKeyName, __int64 *piBuffer, __int64 iDefault, __int64 iMin, __int64 iMax) {
	DWORD dwBufSize=sizeof(__int64),dwType;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)piBuffer,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_BINARY) || (*piBuffer<(DWORD)iMin) || (*piBuffer>(DWORD)iMax)) *piBuffer=iDefault;
}

void QueryRegBoolValue(HKEY hKey, const char *pszKeyName, bool *pbBuffer, bool bDefault) {
	int iValue;
	QueryRegIntValue(hKey, pszKeyName, &iValue, bDefault?1:0, 0, 1);
	*pbBuffer = iValue!=0;
}

void SetRegStringValue(HKEY hKey,const char *pszKeyName,const char *szValue) {
	RegSetValueEx(hKey,pszKeyName,0,REG_SZ,(LPBYTE)szValue,strlen(szValue)+1);
}

void SetRegStringValue(HKEY hKey, const char *pszKeyName, const wchar_t *pwszValue) {
	if (GetVersion() < 0x80000000) {	// Windows NT/2K/XP
		LoadProcs();
		g_SetValueExW(hKey,wstrfromsz(pszKeyName).c_str(),0,REG_SZ,(LPBYTE)pwszValue,wcslen(pwszValue)*2+2);
	} else {							// Windows 95/98/ME
		RegSetValueEx(hKey,pszKeyName,0,REG_BINARY,(LPBYTE)pwszValue,wcslen(pwszValue)*2+2);
	}
}

void SetRegIntValue(HKEY hKey,const char *pszKeyName,int iValue) {
	RegSetValueEx(hKey,pszKeyName,0,REG_DWORD,(LPBYTE)&iValue,sizeof(int));
}

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, string &strBuffer, const char *pszDefault) {
	char *szValue = NULL;
	AllocAndQueryRegStringValue(hKey, pszKeyName, &szValue, NULL, pszDefault);
	strBuffer = szValue ? szValue : "";
	free(szValue);
}

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, string &strBuffer, const string &strDefault) {
	QueryRegStringValue(hKey, pszKeyName, strBuffer, strDefault.c_str());
}

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wstring &wstrBuffer, const wchar_t *pwszDefault) {
	wchar_t *wszValue = NULL;
	AllocAndQueryRegStringValue(hKey, pszKeyName, &wszValue, NULL, pwszDefault);
	wstrBuffer = wszValue ? wszValue : L"";
	free(wszValue);
}

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wstring &wstrBuffer, const wstring &wstrDefault) {
	QueryRegStringValue(hKey, pszKeyName, wstrBuffer, wstrDefault.c_str());
}

void SetRegStringValue(HKEY hKey, const char *pszKeyName, const string &strValue) {
	SetRegStringValue(hKey, pszKeyName, strValue.c_str());
}

void SetRegStringValue(HKEY hKey, const char *pszKeyName, const wstring &wstrValue) {
	SetRegStringValue(hKey, pszKeyName, wstrValue.c_str());
}
