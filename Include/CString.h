#ifndef __CSTRING_H
#define __CSTRING_H

#include <string.h>
#include <stdlib.h>
#include <iostream.h>

class CString {
public:
	CString() {S=(char *)malloc(1);*S=0;};
	CString(const CString &Str) {S=_strdup(Str.S);};
	CString(char *Str) {S=_strdup(Str);};
	friend CString operator +(CString &S1, CString &S2);
	friend CString operator +(CString &S1, char *S2);
	friend CString operator +(char *S1, CString &S2);
	char operator [](int Index) {return S[Index];};
	friend istream &operator >>(istream &stm, CString &Str);
	friend ostream &operator <<(ostream &stm, CString &Str);
	CString operator =(CString &Str);
	CString operator =(char *Str);
	CString operator =(int I);
	CString operator =(long L);
	CString operator =(unsigned long UL);
	CString operator +=(CString &Str);
	CString operator +=(char *Str) {return *this+Str;};
	operator char *() const {return S;};
	operator int() const {return atoi(S);};
	operator long() const {return atol(S);};
	operator float() const {return (float)atof(S);};
	operator double() const {return atof(S);};
	~CString();
protected:
	char *S;
};

istream &operator >>(istream &stm, CString Str);
ostream &operator <<(ostream &stm, CString Str);

#endif
