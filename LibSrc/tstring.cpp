#include <tchar.h>
#include <stdarg.h>
#include <windows.h>

#include "tstring.h"
using namespace std;
#include "StringEx.h"

template<> static int CStringT<char>   ::ctoi(const char    *sz) { return  atoi(sz); }
template<> static int CStringT<wchar_t>::ctoi(const wchar_t *sz) { return _wtoi(sz); }

template<> static char *    CStringT<char>   ::ctoa(int n, char    *sz) { return _itoa(n, sz, 10); }
template<> static wchar_t * CStringT<wchar_t>::ctoa(int n, wchar_t *sz) { return _itow(n, sz, 10); }

template<> static int CStringT<char>   ::cstrlen(const char   *sz)  { return strlen(sz); }
template<> static int CStringT<wchar_t>::cstrlen(const wchar_t *sz) { return wcslen(sz); }

template<> static       char    *CStringT<char   >::cstrchr(      char    *sz, char    c) { return strchr(sz, c); }
template<> static const char    *CStringT<char   >::cstrchr(const char    *sz, char    c) { return strchr(sz, c); }
template<> static       wchar_t *CStringT<wchar_t>::cstrchr(      wchar_t *sz, wchar_t c) { return wcschr(sz, c); }
template<> static const wchar_t *CStringT<wchar_t>::cstrchr(const wchar_t *sz, wchar_t c) { return wcschr(sz, c); }

template<> static int CStringT<char>   ::csprintf_s(char    *sz, size_t count, const char    *szFormat, ...) {
	va_list vaList;
	va_start(vaList, szFormat);
	return vsprintf_s(sz, count, szFormat, vaList);
}
template<> static int CStringT<wchar_t>::csprintf_s(wchar_t *sz, size_t count, const wchar_t *szFormat, ...) {
	va_list vaList;
	va_start(vaList, szFormat);
	return vswprintf_s(sz, count, szFormat, vaList);
}

template<> static char    * CStringT<char>   ::__T2(char *sz, wchar_t *wsz) { return  sz; }
template<> static wchar_t * CStringT<wchar_t>::__T2(char *sz, wchar_t *wsz) { return wsz; }

template<class CHAR>
typename CStringT<CHAR>::cstring
CStringT<CHAR>::Convert(const std:: string &str, UINT nCP) {
	cstring cstr;
	StrToStr(str, cstr, nCP);
	return cstr;
}

template<class CHAR>
typename CStringT<CHAR>::cstring
CStringT<CHAR>::Convert(const std::wstring &str, UINT nCP) {
	cstring cstr;
	StrToStr(str, cstr, nCP);
	return cstr;
}

template<> static char    CStringT<char>   ::ConvertCase(char    ch, ECaseConvert Cvt)
{
	if (Cvt == CCV_NONE) return ch;

	char cAnsi, cOem;
	OemToCharBuffA(&ch,   &cAnsi, 1);
	CharToOemBuffA(&cAnsi, &cOem, 1);
	if (cOem != ch) return ch;

	char cUpper = (char)CharUpperA((char *)(BYTE)cAnsi);
	char cLower = (char)CharLowerA((char *)(BYTE)cAnsi);

	switch (Cvt) {
	case CCV_UPPER:
		cAnsi = cUpper;
		break;
	case CCV_LOWER:
		cAnsi = cLower;
		break;
	case CCV_FLIP:
		cAnsi = (cLower == cAnsi) ? cUpper : cLower;
		break;
	}

	CharToOemBuffA(&cAnsi, &ch, 1);
	return ch;
}

template<> static wchar_t CStringT<wchar_t>::ConvertCase(wchar_t ch, ECaseConvert Cvt)
{
	if (Cvt == CCV_NONE) return ch;

	wchar_t cUpper = (wchar_t)CharUpperW((wchar_t *)(USHORT)ch);
	wchar_t cLower = (wchar_t)CharLowerW((wchar_t *)(USHORT)ch);

	switch (Cvt) {
	case CCV_UPPER:
		return cUpper;
	case CCV_LOWER:
		return cLower;
	case CCV_FLIP:
		return (cLower == ch) ? cUpper : cLower;
	default:
		return ch;
	}
}

template class CStringT<char>;
#ifdef UNICODE
template class CStringT<wchar_t>;
#endif
