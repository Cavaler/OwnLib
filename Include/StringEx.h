#ifndef __STRINGEX_H
#define __STRINGEX_H

const char *strechr(const char *str, int ch);
const void *memechr(const void *buf, int c, size_t count);

enum {GW_WORDLEN=0,GW_SKIPLEN};

int  GetWord(char *Line,char *Word,int RetWhat=GW_SKIPLEN);
int  GetStripWord(char *Line,char *Word);
int  IsWildcard(char *WildCard,char *Name);

const char *NextWord(const char *&szLine, int &nLength, int &nCount);

#ifdef __COLLECT_H
void ParseWordsTo(char *Line,CStringCollection &Coll);
#endif

char *FormatSz(const char *szFormat, ...);
#if (defined _XSTRING_) || (defined _STLP_STRING)
int  GetWord(string Line,string &Word,int RetWhat=GW_SKIPLEN);
int  GetStripWord(string &Line,string &Word);
string FormatStr(const char *szFormat, ...);

wstring ToUnicode(const string &strOEM);
wstring ANSIToUnicode(const string &strANSI);
string FromUnicode(const wstring &wstrUnicode);
string ANSIFromUnicode(const wstring &wstrUnicode);

#if (defined _VECTOR_) || (defined _STLP_VECTOR)
void ParseWordsTo(string Line,vector<string> &Coll);
#endif

#endif

#endif  __STRINGEX_H
