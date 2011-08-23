#ifndef __STRINGEX_H
#define __STRINGEX_H

const char *strechr(const char *str, int ch);
const void *memechr(const void *buf, int c, size_t count);

enum {GW_WORDLEN=0,GW_SKIPLEN};

int  GetWord(char *Line,char *Word,int RetWhat=GW_SKIPLEN);
int  GetStripWord(char *Line,char *Word);
int  IsWildcard(char *WildCard,char *Name);

const char *NextWord(const char *&szLine, int &nLength, int &nCount);
const wchar_t *NextWord(const wchar_t *&szLine, int &nLength, int &nCount);

#ifdef __COLLECT_H
void ParseWordsTo(char *Line,CStringCollection &Coll);
#endif

char *FormatSz(const char *szFormat, ...);
#if (defined _XSTRING_) || (defined _STLP_STRING)
int  GetWord(string Line,string &Word,int RetWhat=GW_SKIPLEN);
int  GetStripWord(string &Line,string &Word);
int  GetWord(wstring Line,wstring &Word,int RetWhat=GW_SKIPLEN);
int  GetStripWord(wstring &Line,wstring &Word);
string FormatStrA(const char *szFormat, ...);
wstring FormatStrW(const wchar_t *szFormat, ...);
#ifdef UNICODE
#define FormatStr FormatStrW
#else
#define FormatStr FormatStrA
#endif

wstring StrToUnicode(const string &strMBCS, UINT nCP);
wstring OEMToUnicode(const string &strOEM);
wstring ANSIToUnicode(const string &strANSI);
wstring UTF8ToUnicode(const string &strUTF8);
string StrFromUnicode(const wstring &wstrUnicode, UINT nCP);
string OEMFromUnicode(const wstring &wstrUnicode);
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

#include <tstring.h>
tstring URLEncode(const TCHAR *szString);

#if (defined _VECTOR_) || (defined _STLP_VECTOR)
void ParseWordsTo(string Line,vector<string> &Coll);
#endif

#endif

#endif  __STRINGEX_H
