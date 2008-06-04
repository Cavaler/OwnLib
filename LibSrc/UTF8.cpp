#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <vector>
#include <string>
using namespace std;
#include <UTF8.h>

string EncodeUTF8(const wchar_t *pwszValue, int nLength)
{
	if (nLength < 0) nLength = wcslen(pwszValue);

	string strResult;
	for (int nPos = 0; nPos<nLength; nPos++)
	{
		wchar_t ch = pwszValue[nPos];
		if (0 == (ch & 0xFF80))
			strResult += (char) ch;
		else if (0 == (ch & 0xF800))
		{
			strResult += (char) (((ch >> 6) & 0x1F) | 0xC0);
			strResult += (char) ((ch & 0x3F) | 0x80);
		}
		else
		{
			strResult += (char) (((ch >> 12) & 0x0F) | 0xE0);
			strResult += (char) (((ch >> 6) & 0x1F) | 0x80);
			strResult += (char) ((ch & 0x3F) | 0x80);
		}
	}
	return strResult;
}

string EncodeUTF8(const wstring &wstrValue)
{
	return EncodeUTF8(wstrValue.data(), wstrValue.length());
}

string EncodeUTF8(const char *pszValue, int nLength, unsigned int uiCP)
{
	if (nLength < 0) nLength = strlen(pszValue);
	if (nLength == 0) return "";

	vector<wchar_t> pwszValue(nLength);
	MultiByteToWideChar(uiCP, 0, pszValue, nLength, &pwszValue[0], nLength);
	return EncodeUTF8(&pwszValue[0], nLength);
}

string EncodeUTF8(const string &strValue, unsigned int uiCP)
{
	return EncodeUTF8(strValue.data(), strValue.length(), uiCP);
}

wstring DecodeUTF8(const char *pszValue, int nLength)
{
	if (nLength < 0) nLength = strlen(pszValue);

	vector<wchar_t> pwData(nLength);
	unsigned int nOutLen = 0;
	for (int nPos = 0; nPos < nLength; nPos++)
	{
		char ch = pszValue[nPos];
		wchar_t wch = 0;
		if (0 == (ch & 0x80))
			wch = ch;
		else if ((0xC0 == (ch & 0xE0)) && (nPos < nLength - 1) && (0x80 == (pszValue[nPos + 1] & 0xC0)))
		{
			wch = pszValue[nPos + 1] & 0x3F;
			wch |= (wchar_t(ch) & 0x1F) << 6;
			nPos++;
		}
		else if ((0xE0 == (ch & 0xF0)) && (nPos < nLength - 2) && (0x80 == (pszValue[nPos + 1] & 0xC0)) && (0x80 == (pszValue[nPos + 2] & 0xC0)))
		{
			wch = pszValue[nPos + 2] & 0x3F;
			wch |= wchar_t(pszValue[nPos + 1] & 0x3F) << 6;
			wch |= wchar_t(ch & 0x0F) << 12;
			nPos += 2;
		}
		else
			continue;
		pwData[nOutLen++] = wch;
	}

	return (nOutLen) ? wstring(&pwData[0], nOutLen) : L"";
}

wstring DecodeUTF8(const string &strValue)
{
	return DecodeUTF8(strValue.data(), strValue.length());
}

string DecodeUTF8A(const char *pszValue, int nLength, unsigned int uiCP)
{
	wstring wstrResult = DecodeUTF8(pszValue, nLength);
	if (wstrResult.empty()) return "";
	vector<char> pszResult(wstrResult.length());
	WideCharToMultiByte(uiCP, 0, wstrResult.data(), wstrResult.length(), &pszResult[0], wstrResult.length(), NULL, NULL);
	return string(&pszResult[0],wstrResult.length());
}

string DecodeUTF8A(const string &strValue, unsigned int uiCP)
{
	return DecodeUTF8A(strValue.data(), strValue.length(), uiCP);
}
