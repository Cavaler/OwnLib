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
#else
typedef unsigned char UTCHAR;
#endif

#endif
