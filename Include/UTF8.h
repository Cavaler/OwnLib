#ifndef __UTF8_H
#define __UTF8_H

string EncodeUTF8(const wchar_t *pwszValue, int nLength = -1);
string EncodeUTF8(const wstring &wstrValue);
string EncodeUTF8(const char *pszValue, int nLength = -1, unsigned int uiCP = CP_ACP);
string EncodeUTF8(const string &strValue, unsigned int uiCP = CP_ACP);

wstring DecodeUTF8(const char *pszValue, int nLength = -1);
wstring DecodeUTF8(const string &strValue);
string DecodeUTF8A(const char *pszValue, int nLength = -1, unsigned int uiCP = CP_ACP);
string DecodeUTF8A(const string &strValue, unsigned int uiCP = CP_ACP);

#endif
