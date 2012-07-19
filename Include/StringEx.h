#ifndef __STRINGEX_H
#define __STRINGEX_H

#include <tstring.h>
#include <vector>
#include <set>
#include <iterator>

const char *strechr(const char *str, int ch);
const void *memechr(const void *buf, int c, size_t count);

enum {GW_WORDLEN=0, GW_SKIPLEN};

int  IsWildcard(char *WildCard,char *Name);

const char *NextWord(const char *&szLine, int &nLength, int &nCount);
const wchar_t *NextWord(const wchar_t *&szLine, int &nLength, int &nCount);

int  GetWord(string Line, string &Word, int RetWhat=GW_SKIPLEN);
int  GetStripWord(string &Line, string &Word);
int  GetWord(wstring Line, wstring &Word, int RetWhat=GW_SKIPLEN);
int  GetStripWord(wstring &Line, wstring &Word);

string FormatStrA(const char *szFormat, ...);
wstring FormatStrW(const wchar_t *szFormat, ...);

#ifdef UNICODE
#define FormatStr FormatStrW
#else
#define FormatStr FormatStrA
#endif

wstring StrToUnicode   (const  string &strMBCS, UINT nCP);
wstring OEMToUnicode   (const  string &strOEM);
wstring ANSIToUnicode  (const  string &strANSI);
wstring UTF8ToUnicode  (const  string &strUTF8);
 string StrFromUnicode (const wstring &wstrUnicode, UINT nCP);
 string OEMFromUnicode (const wstring &wstrUnicode);
 string ANSIFromUnicode(const wstring &wstrUnicode);
 string UTF8FromUnicode(const wstring &wstrUnicode);
bool   DefCharFromUnicode();

//	These two are blanks to avoid #ifdef UNICODE sometimes
string StrFromUnicode(const string &strMBCS, UINT nCP);
wstring StrToUnicode(const wstring &wstrUnicode, UINT nCP);

void   StrToStr(const  string &strFrom,  string &strTo, UINT nCP);
void   StrToStr(const  string &strFrom, wstring &strTo, UINT nCP);
void   StrToStr(const wstring &strFrom,  string &strTo, UINT nCP);
void   StrToStr(const wstring &strFrom, wstring &strTo, UINT nCP);

string URLEncode(const char *szString);

template<class CHAR, class Iterator>
void ParseWordsToIter(const CHAR *szLine, Iterator &iter)
{
	int nLength = -1;

	do {
		int nCount;
		const CHAR *szWord = NextWord(szLine, nLength, nCount);
		if ((nLength == 0) && (nCount == 0)) break;
		*iter = basic_string<CHAR>(szWord, nCount);
	} while (true);
}

template<class CHAR>
void ParseWordsTo(const CHAR *szLine, vector<basic_string<CHAR> > &arrLines)
{
	ParseWordsToIter(szLine, back_inserter(arrLines));
}

template<class CHAR>
void ParseWordsTo(const basic_string<CHAR> &strLine, vector<basic_string<CHAR> > &arrLines)
{
	ParseWordsToIter(strLine.c_str(), back_inserter(arrLines));
}

template<class CHAR>
void ParseWordsTo(const CHAR *szLine, set<basic_string<CHAR> > &setLines)
{
	ParseWordsToIter(szLine, insert_iterator<set<basic_string<CHAR> > >(setLines, setLines.end()));
}

#endif  __STRINGEX_H
