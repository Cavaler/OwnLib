#include "StringEx.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>

#include <vector>
#include <string>
using namespace std;

const char *strechr(const char *str, int ch) {
	const char *sz = strchr(str, ch);
	return sz ? sz : strchr(str, 0);
}

const void *memechr(const void *buf, int c, size_t count) {
	const void *sz = memchr(buf, c, count);
	return sz ? sz : (char *)buf+count;
}

const char *NextWord(const char *&szLine, int &nLength, int &nCount) {
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

const wchar_t *NextWord(const wchar_t *&szLine, int &nLength, int &nCount) {
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

int GetWord(char *Line,char *Word,int RetWhat) {
	int I=0,J=0;
	char EC1,EC2;
	while (((Line[I]==' ')||(Line[I]=='\t'))&&(Line[I])) I++;
	switch (Line[I]) {
	case '"':EC1=EC2='"';I++;break;
	case '\'':EC1=EC2='\'';I++;break;
	case '`':EC1=EC2='`';I++;break;
	default:EC1=' ';EC2='\t';
	}
	while ((Line[I]!=EC1)&&(Line[I]!=EC2)&&(Line[I])) {
		if (Word) Word[J++]=Line[I++]; else {I++;J++;}
	}
	if (Word) Word[J]=0;
	if (Line[I]==EC1) I++;

	while (((Line[I]==' ')||(Line[I]=='\t'))&&(Line[I])) I++;
	return (RetWhat==GW_WORDLEN)?J:I;
}

int GetStripWord(char *Line,char *Word) {
	int I=GetWord(Line, Word, GW_SKIPLEN);
	strcpy(Line, Line+I);
	return I;
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

char *FormatSz(const char *szFormat, ...) {
	va_list List;
	va_start(List, szFormat);
	vector<char> strBuffer(1024);
	while (_vsnprintf(&strBuffer[0], strBuffer.size(), szFormat, List) == -1) {
		strBuffer.resize(strBuffer.size()*2);
	}
	va_end(List);
	return _strdup(&strBuffer[0]);
}

void ParseWordsTo(string Line,vector<string> &Coll);

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

wstring StrToUnicode(const string &strMBCS, UINT nCP) {
	vector<wchar_t> wszBuffer(strMBCS.length()+1);
	MultiByteToWideChar(nCP, 0, strMBCS.c_str(), -1, &wszBuffer[0], wszBuffer.size());
	return &wszBuffer[0];
}

wstring OEMToUnicode(const string &strOEM) { return StrToUnicode(strOEM, CP_OEMCP); }
wstring ANSIToUnicode(const string &strANSI) { return StrToUnicode(strANSI, CP_ACP); }
wstring UTF8ToUnicode(const string &strUTF8) { return StrToUnicode(strUTF8, CP_UTF8); }

string StrFromUnicode(const wstring &wstrUnicode, UINT nCP) {
	vector<char> szBuffer(wstrUnicode.length()*4+4);

	if (WideCharToMultiByte(nCP, 0, wstrUnicode.c_str(), -1, &szBuffer[0], szBuffer.size(), NULL, &g_bUsedDefaultChar) == 0) {
		if (GetLastError() == ERROR_INVALID_PARAMETER)
			WideCharToMultiByte(nCP, 0, wstrUnicode.c_str(), -1, &szBuffer[0], szBuffer.size(), NULL, NULL);
		else
			return string();
	}

	return &szBuffer[0];
}

string OEMFromUnicode(const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_OEMCP); }
string ANSIFromUnicode(const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_ACP); }
string UTF8FromUnicode(const wstring &wstrUnicode) { return StrFromUnicode(wstrUnicode, CP_UTF8); }

bool DefCharFromUnicode() {
	return g_bUsedDefaultChar != 0;
}
