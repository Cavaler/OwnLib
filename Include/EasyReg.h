#ifndef __EASYREG_H
#define __EASYREG_H

void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, TCHAR *pszBuffer, DWORD dwBufSize, const TCHAR *pszDefault = NULL);
void AllocAndQueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, TCHAR **ppszBuffer, DWORD *pwdBufSize = NULL, const TCHAR *pszDefault = NULL);
#ifndef _UNICODE
void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wchar_t *pwszBuffer, DWORD dwBufSize, const wchar_t *pwszDefault = NULL);
void AllocAndQueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, wchar_t **ppwszBuffer, DWORD *pdwBufSize = NULL, const wchar_t *pwszDefault = NULL);
#endif

template<class IntType>
void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, IntType *piBuffer, int iDefault = 0) {
	DWORD dwBufSize=sizeof(int),dwType,dwResult;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)&dwResult,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_DWORD))
		*piBuffer=(IntType)iDefault;
	else
		*piBuffer=(IntType)dwResult;
}

template<class IntType>
void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, IntType *piBuffer, int iDefault, int iMin, int iMax=0x7FFFFFFF) {
	DWORD dwBufSize=sizeof(int),dwType,dwResult;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)&dwResult,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_DWORD) || (dwResult<(DWORD)iMin) || (dwResult>(DWORD)iMax))
		*piBuffer=(IntType)iDefault;
	else
		*piBuffer=(IntType)dwResult;
}

void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, __int64 *piBuffer, __int64 iDefault);
void QueryRegIntValue(HKEY hKey, const TCHAR *pszKeyName, __int64 *piBuffer, __int64 iDefault, __int64 iMin, __int64 iMax);

void QueryRegBoolValue(HKEY hKey, const TCHAR *pszKeyName, BOOL *pbBuffer, BOOL bDefault);
void QueryRegBoolValue(HKEY hKey, const TCHAR *pszKeyName, bool *pbBuffer, bool bDefault);

void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const TCHAR *pwszValue);
#ifndef _UNICODE
void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const wchar_t *pwszValue);
#endif
void SetRegIntValue(HKEY hKey, const TCHAR *pszKeyName, int iValue);
#define SetRegBoolValue(hKey, pszKeyName, bValue) SetRegIntValue(hKey, pszKeyName, (bValue)?1:0)
void SetRegBinaryValue(HKEY hKey, const TCHAR *pszKeyName, const void *pData,  int nLength);

__if_exists (std::string) {
#include "tstring.h"
void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, std::tstring &strBuffer, const TCHAR *pszDefault);
void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, std::tstring &strBuffer, const std::tstring &strDefault);
void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const std::tstring &strValue);

#ifndef _UNICODE
void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, std::wstring &wstrBuffer, const wchar_t *pwszDefault);
void QueryRegStringValue(HKEY hKey, const TCHAR *pszKeyName, std::wstring &wstrBuffer, const std::wstring &wstrDefault);
void SetRegStringValue(HKEY hKey, const TCHAR *pszKeyName, const std::wstring &wstrValue);
#endif
}

HKEY RegOpenSubkey(HKEY hKey, const TCHAR *pszKeyName);
HKEY RegCreateSubkey(HKEY hKey, const TCHAR *pszKeyName);
void RegDeleteAllSubkeys(HKEY hKey);
void RegDeleteAllValues(HKEY hKey);

#endif
