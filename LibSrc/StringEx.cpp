#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#include <set>
using namespace std;

#include "StringEx.h"

const char *strechr(const char *str, int ch) {
	const char *sz = strchr(str, ch);
	return sz ? sz : strchr(str, 0);
}

const void *memechr(const void *buf, int c, size_t count) {
	const void *sz = memchr(buf, c, count);
	return sz ? sz : (char *)buf+count;
}

const char *NextWord(const char *&szLine, int &nLength, int &nCount)
{
	if (nLength == -1) nLength = strlen(szLine);
	while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
	if (!nLength) {if (&nCount) nCount = 0;return szLine;}

	const char *szStart = szLine;
	if ((szLine[0] == '"') || (szLine[0] == '\'')) {
		szStart++;
		const char *szEnd = (const char *)memchr(szLine+1, szLine[0], nLength);
		if (szEnd) {
			if (&nCount) nCount = szEnd-szStart;
			nLength -= szEnd-szStart+2;
			szLine = szEnd+1;
			while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
		} else {
			if (&nCount) nCount = nLength-1;
			szLine += nLength;
			nLength = 0;
		}
		return szStart;
	}

	while (nLength && !isspace(szLine[0])) {szLine++;nLength--;}
	if (&nCount) nCount = szLine-szStart;
	while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
	return szStart;
}

const wchar_t *NextWord(const wchar_t *&szLine, int &nLength, int &nCount)
{
	if (nLength == -1) nLength = wcslen(szLine);
	while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
	if (!nLength) {if (&nCount) nCount = 0;return szLine;}

	const wchar_t *szStart = szLine;
	if ((szLine[0] == '"') || (szLine[0] == '\'')) {
		szStart++;
		const wchar_t *szEnd = wmemchr(szLine+1, szLine[0], nLength);
		if (szEnd) {
			if (&nCount) nCount = szEnd-szStart;
			nLength -= szEnd-szStart+2;
			szLine = szEnd+1;
			while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
		} else {
			if (&nCount) nCount = nLength-1;
			szLine += nLength;
			nLength = 0;
		}
		return szStart;
	}

	while (nLength && !isspace(szLine[0])) {szLine++;nLength--;}
	if (&nCount) nCount = szLine-szStart;
	while (nLength && isspace(szLine[0])) {szLine++;nLength--;}
	return szStart;
}


template<class CHAR>
int  GetWord(basic_string<CHAR> strLine, basic_string<CHAR> &strWord, int iRetWhat) {
	const CHAR *szLine = strLine.c_str();
	int nLength = strLine.length(), nCount;
	const CHAR *szWord = NextWord(szLine, nLength, nCount);
	strWord = basic_string<CHAR>(szWord, nCount);
	return (iRetWhat == GW_SKIPLEN) ? szLine - strLine.c_str() : nCount;
}

template<class CHAR>
int  GetStripWord(basic_string<CHAR> &strLine, basic_string<CHAR> &strWord) {
	const CHAR *szLine = strLine.c_str();
	int nLength = strLine.length(), nCount;
	const CHAR *szWord = NextWord(szLine, nLength, nCount);
	strWord = basic_string<CHAR>(szWord, nCount);
	int nStrip = szLine - strLine.c_str();
	strLine.erase(0, nStrip);
	return nStrip;
}

int  GetWord(string strLine, string &strWord, int iRetWhat) {
	return GetWord<char>(strLine, strWord, iRetWhat);
}

int  GetStripWord(string &strLine, string &strWord) {
	return GetStripWord<char>(strLine, strWord);
}

int  GetWord(wstring strLine, wstring &strWord, int iRetWhat) {
	return GetWord<wchar_t>(strLine, strWord, iRetWhat);
}

int  GetStripWord(wstring &strLine, wstring &strWord) {
	return GetStripWord<wchar_t>(strLine, strWord);
}

string FormatStrA(const char *szFormat, ...) {
	va_list List;
	va_start(List, szFormat);
	vector<char> strBuffer(1024);
	while (_vsnprintf(&strBuffer[0], strBuffer.size(), szFormat, List) == -1) {
		strBuffer.resize(strBuffer.size()*2);
	}
	va_end(List);
	return &strBuffer[0];
}

wstring FormatStrW(const wchar_t *szFormat, ...) {
	va_list List;
	va_start(List, szFormat);
	vector<wchar_t> strBuffer(1024);
	while (_vsnwprintf(&strBuffer[0], strBuffer.size(), szFormat, List) == -1) {
		strBuffer.resize(strBuffer.size()*2);
	}
	va_end(List);
	return &strBuffer[0];
}

int  IsWildcard(char *WildCard,char *Name) {
	if (*WildCard==0) return *Name==0;
	if (*Name==0) return (*WildCard==0)||((*WildCard=='*')&&(!WildCard[1]));
	switch (*WildCard) {
	case '*':if (IsWildcard(WildCard+1,Name)) return 1;
										 else return IsWildcard(WildCard,Name+1);
	case '?':return IsWildcard(WildCard+1,Name+1);
	default:if (toupper(*WildCard)==toupper(*Name)) return IsWildcard(WildCard+1,Name+1); else return 0;
	}
}

BOOL g_bUsedDefaultChar = FALSE;

//	The following 'combo' allows NRVO to kick in
//	It's bad to edit basic_string in-place, but efficient

void StrToUnicode(const string &strMBCS, UINT nCP, wstring &wstrResult) throw() {
	wstrResult.resize(strMBCS.length());
	if (strMBCS.empty()) return;

	int nCount = MultiByteToWideChar(nCP, 0, strMBCS.c_str(), strMBCS.length(), (LPWSTR)wstrResult.data(), wstrResult.size());
	wstrResult.erase(nCount);
};

wstring StrToUnicode(const string &strMBCS, UINT nCP) throw() {
	wstring wstrResult;
	StrToUnicode(strMBCS, nCP, wstrResult);
	return wstrResult;
}

wstring OEMToUnicode (const string &strOEM)  { return StrToUnicode(strOEM,  CP_OEMCP); }
wstring ANSIToUnicode(const string &strANSI) { return StrToUnicode(strANSI, CP_ACP  ); }
wstring UTF8ToUnicode(const string &strUTF8) { return StrToUnicode(strUTF8, CP_UTF8 ); }

void StrFromUnicode(const wstring &wstrUnicode, UINT nCP, string &strResult) throw() {
	strResult.resize(wstrUnicode.length()*4);
	if (wstrUnicode.empty()) return;
	int nCount = WideCharToMultiByte(nCP, 0, wstrUnicode.c_str(), wstrUnicode.length(), (LPSTR)strResult.data(), strResult.size(), NULL, &g_bUsedDefaultChar);

	if ((nCount == 0) && (GetLastError() == ERROR_INVALID_PARAMETER))
		nCount = WideCharToMultiByte(nCP, 0, wstrUnicode.c_str(), wstrUnicode.length(), (LPSTR)strResult.data(), strResult.size(), NULL, NULL);

	strResult.erase(nCount);
}

string StrFromUnicode(const wstring &wstrUnicode, UINT nCP) throw() {
	string strResult;
	StrFromUnicode(wstrUnicode, nCP, strResult);
	return strResult;
}

string OEMFromUnicode (const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_OEMCP); }
string ANSIFromUnicode(const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_ACP  ); }
string UTF8FromUnicode(const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_UTF8 ); }

string StrFromUnicode(const string &strMBCS, UINT nCP) { return strMBCS; }
wstring StrToUnicode(const wstring &wstrUnicode, UINT nCP) { return wstrUnicode; }

void   StrToStr(const  string &strFrom,  string &strTo, UINT nCP) { strTo = strFrom; }
void   StrToStr(const  string &strFrom, wstring &strTo, UINT nCP) { strTo = StrToUnicode  (strFrom, nCP); }
void   StrToStr(const wstring &strFrom,  string &strTo, UINT nCP) { strTo = StrFromUnicode(strFrom, nCP); }
void   StrToStr(const wstring &strFrom, wstring &strTo, UINT nCP) { strTo = strFrom; }

bool DefCharFromUnicode() {
	return g_bUsedDefaultChar != 0;
}

string URLEncode(const char *szString)
{
	return URLEncode(string(szString));
}

string URLEncode(const string &strString)
{
	string strResult;

	for each (BYTE c in strString)
	{
		if (
			((c >= 'a') && (c <= 'z')) ||
			((c >= 'A') && (c <= 'Z')) ||
			((c >= '0') && (c <= '9')) ||
			(c == '-') || (c == '.') || (c == '_') || (c == '~')
			)
		{
			strResult += (char)c;
		} else {
			strResult += FormatStrA("%%%02X", c);
		}
	}

	return strResult;
}
string URLEncodeANSI(const string &strOEMString)
{
	return URLEncode(ANSIFromUnicode(OEMToUnicode(strOEMString)).c_str());
}

string URLEncodeUTF8(const string &strOEMString)
{
	return URLEncode(UTF8FromUnicode(OEMToUnicode(strOEMString)).c_str());
}

wstring URLEncodeANSI(const wstring &strString)
{
	return ANSIToUnicode(URLEncode(ANSIFromUnicode(strString).c_str()));
}

wstring URLEncodeUTF8(const wstring &strString)
{
	return ANSIToUnicode(URLEncode(UTF8FromUnicode(strString).c_str()));
}
