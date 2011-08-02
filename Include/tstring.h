#ifndef __TSTRING_H
#define __TSTRING_H

#include <string>
#include "tchar.h"

namespace std {
#ifdef _UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
};

#ifdef UNICODE
typedef unsigned short UTCHAR;
#define _ttoa _itow
#else
typedef unsigned char UTCHAR;
#define _ttoa _itoa
#endif

enum ECaseConvert {CCV_NONE, CCV_UPPER, CCV_LOWER, CCV_FLIP};

template<class CHAR>
class CStringT {
public:
	typedef std::basic_string<CHAR> cstring;

	static int ctoi(const CHAR *sz);
	static CHAR *ctoa(int n, CHAR *sz);
	static int cstrlen(const CHAR *sz);
	static CHAR *cstrchr(CHAR *sz, CHAR c);
	static const CHAR *cstrchr(const CHAR *sz, CHAR c);
	static int csprintf_s(CHAR *sz, size_t count, const CHAR *szFormat, ...);

	static CHAR *__T2(char *sz, wchar_t *wsz);

	static CHAR ConvertCase(CHAR ch, ECaseConvert Cvt);
};

#define _T2(sz) __T2(sz, L ## sz)

#endif
