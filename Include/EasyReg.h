#ifndef __EASYREG_H
#define __EASYREG_H

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, char *pszBuffer, DWORD dwBufSize, const char *pszDefault = NULL);
void AllocAndQueryRegStringValue(HKEY hKey, const char *pszKeyName, char **ppszBuffer, DWORD *pwdBufSize = NULL, const char *pszDefault = NULL);
void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wchar_t *pwszBuffer, DWORD dwBufSize, const wchar_t *pwszDefault = NULL);
void AllocAndQueryRegStringValue(HKEY hKey, const char *pszKeyName, wchar_t **ppwszBuffer, DWORD *pdwBufSize = NULL, const wchar_t *pwszDefault = NULL);

template<class IntType>
void QueryRegIntValue(HKEY hKey, const char *pszKeyName, IntType *piBuffer, int iDefault = 0) {
	DWORD dwBufSize=sizeof(int),dwType,dwResult;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)&dwResult,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_DWORD))
		*piBuffer=(IntType)iDefault;
	else
		*piBuffer=(IntType)dwResult;
}

template<class IntType>
void QueryRegIntValue(HKEY hKey, const char *pszKeyName, IntType *piBuffer, int iDefault, int iMin, int iMax=0x7FFFFFFF) {
	DWORD dwBufSize=sizeof(int),dwType,dwResult;
	LONG lRes=RegQueryValueEx(hKey,pszKeyName,NULL,&dwType,(LPBYTE)&dwResult,&dwBufSize);
	if ((lRes!=ERROR_SUCCESS) || (dwType!=REG_DWORD) || (dwResult<(DWORD)iMin) || (dwResult>(DWORD)iMax))
		*piBuffer=(IntType)iDefault;
	else
		*piBuffer=(IntType)dwResult;
}

void QueryRegIntValue(HKEY hKey, const char *pszKeyName, __int64 *piBuffer, __int64 iDefault);
void QueryRegIntValue(HKEY hKey, const char *pszKeyName, __int64 *piBuffer, __int64 iDefault, __int64 iMin, __int64 iMax);

void QueryRegBoolValue(HKEY hKey, const char *pszKeyName, BOOL *pbBuffer, BOOL bDefault);
void QueryRegBoolValue(HKEY hKey, const char *pszKeyName, bool *pbBuffer, bool bDefault);

void SetRegStringValue(HKEY hKey, const char *pszKeyName, const char *pwszValue);
void SetRegStringValue(HKEY hKey, const char *pszKeyName, const wchar_t *pwszValue);
void SetRegIntValue(HKEY hKey, const char *pszKeyName, int iValue);
#define SetRegBoolValue(hKey, pszKeyName, bValue) SetRegIntValue(hKey, pszKeyName, (bValue)?1:0)
void SetRegBinaryValue(HKEY hKey, const char *pszKeyName, const void *pData,  int nLength);

#if (defined _XSTRING_) || (defined _STLP_STRING)
void QueryRegStringValue(HKEY hKey, const char *pszKeyName, string &strBuffer, const char *pszDefault);
void QueryRegStringValue(HKEY hKey, const char *pszKeyName, string &strBuffer, const string &strDefault);
void SetRegStringValue(HKEY hKey, const char *pszKeyName, const string &strValue);

void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wstring &wstrBuffer, const wchar_t *pwszDefault);
void QueryRegStringValue(HKEY hKey, const char *pszKeyName, wstring &wstrBuffer, const wstring &wstrDefault);
void SetRegStringValue(HKEY hKey, const char *pszKeyName, const wstring &wstrValue);
#endif

#endif
