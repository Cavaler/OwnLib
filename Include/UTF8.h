#ifndef __UTF8_H
#define __UTF8_H

#include <string>

std::string EncodeUTF8(const wchar_t *pwszValue, int nLength = -1);
std::string EncodeUTF8(const std::wstring &wstrValue);
std::string EncodeUTF8(const char *pszValue, int nLength = -1, unsigned int uiCP = CP_ACP);
std::string EncodeUTF8(const std::string &strValue, unsigned int uiCP = CP_ACP);

std::wstring DecodeUTF8(const char *pszValue, int nLength = -1);
std::wstring DecodeUTF8(const std::string &strValue);
std::string DecodeUTF8A(const char *pszValue, int nLength = -1, unsigned int uiCP = CP_ACP);
std::string DecodeUTF8A(const std::string &strValue, unsigned int uiCP = CP_ACP);

#endif
