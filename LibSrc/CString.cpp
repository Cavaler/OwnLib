#include <string.h>
#include <iostream.h>
#include "CString.h"

istream &operator >>(istream &stm, CString &Str) {
	stm >> Str.S;
	return stm;
}

ostream &operator <<(ostream &stm, CString &Str) {
	stm << Str.S;
	return stm;
}

CString operator +(CString &S1, CString &S2) {
	CString SOut;
	char *SNew=(char *)malloc(strlen(S1.S)+strlen(S2.S)+1);
	strcat(strcpy(SNew,S1.S),S2.S);
	SOut=SNew;free(SNew);
	return SOut;
}

CString operator +(CString &S1, char *S2) {
	CString SOut;
	char *SNew=(char *)malloc(strlen(S1.S)+strlen(S2)+1);
	strcat(strcpy(SNew,S1.S),S2);
	SOut=SNew;free(SNew);
	return SOut;
}

CString operator +(char *S1, CString &S2) {
	CString SOut;
	char *SNew=(char *)malloc(strlen(S1)+strlen(S2.S)+1);
	strcat(strcpy(SNew,S1),S2.S);
	SOut=SNew;free(SNew);
	return SOut;
}

CString CString::operator =(char *Str) {
	if (S) free(S);
	S=_strdup(Str);
	return *this;
}

CString CString::operator =(CString &Str) {
	if (S) free(S);
	S=_strdup(Str.S);
	return *this;
}

CString CString::operator =(int I) {
	char Buf[17];
	return *this=itoa(I,Buf,10);
}

CString CString::operator =(long L) {
	char Buf[33];
	return *this=ltoa(L,Buf,10);
}

CString CString::operator =(unsigned long UL) {
	char Buf[33];
	return *this=ultoa(UL,Buf,10);
}

CString CString::operator +=(CString &Str) {return *this+Str;};

CString::~CString() {
	if (S) free(S);
}

