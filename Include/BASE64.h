#ifndef __BASE64_H
#define __BASE64_H

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

int Mul4Div3(int Size);
int BASE64Encode(const BYTE *In,int Length,BYTE *Out);
char *BASE64EncodeToStr(const BYTE *In,int Length);
char *BASE64EncodeStrToStr(const char *In);

int Mul3Div4(int Size);
int BASE64Decode(const BYTE *In,int Length,BYTE *Out);
int BASE64DecodeStr(const char *In,BYTE *Out);
char *BASE64DecodeStrToStr(const char *In);

#endif
