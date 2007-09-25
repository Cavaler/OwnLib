#include <stdlib.h>
#include <BASE64.h>

int Mul4Div3(int Size) {return (Size%3==0)?4*Size/3:4*(Size/3+1);}
int Mul3Div4(int Size) {return (Size%4==0)?3*Size/4:3*(Size/4+1);}

BYTE CodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int BASE64Encode(const BYTE *In,int Length,BYTE *Out) {
	int I, Pos=0;
	for (I=0;I<Length/3;I++) {
		Out[Pos++]=CodeTable[(In[I*3]>>2)&0x3F];
		Out[Pos++]=CodeTable[((In[I*3]<<4)&0x30)+((In[I*3+1]>>4)&0x0F)];
		Out[Pos++]=CodeTable[((In[I*3+1]<<2)&0x3C)+((In[I*3+2]>>6)&0x03)];
		Out[Pos++]=CodeTable[In[I*3+2]&0x3F];
	}
	if (Length-I*3==1) {
		Out[Pos++]=CodeTable[(In[I*3]>>2)&0x3F];
		Out[Pos++]=CodeTable[(In[I*3]<<4)&0x30];
		Out[Pos++]='=';
		Out[Pos++]='=';
	}
	if (Length-I*3==2) {
		Out[Pos++]=CodeTable[(In[I*3]>>2)&0x3F];
		Out[Pos++]=CodeTable[((In[I*3]<<4)&0x30)+((In[I*3+1]>>4)&0x0F)];
		Out[Pos++]=CodeTable[((In[I*3+1]<<2)&0x3C)+((In[I*3+2]>>6)&0x03)];
		Out[Pos++]='=';
	}
	return Pos;
}

char *BASE64EncodeToStr(const BYTE *In,int Length) {
	char *Result=(char *)malloc(Mul4Div3(Length)+1);
	int ResLength=BASE64Encode(In,Length,(BYTE *)Result);
	Result[ResLength]=0;
	return Result;
}

char *BASE64EncodeStrToStr(const char *In) {
	return BASE64EncodeToStr((BYTE *)In,strlen(In));
}

int Decoded(BYTE Char) {
	if ((Char>='A')&&(Char<='Z')) return Char-'A';
	if ((Char>='a')&&(Char<='z')) return Char-'a'+26;
	if ((Char>='0')&&(Char<='9')) return Char-'0'+52;
	if (Char=='+') return 62;
	if (Char=='/') return 63;
	if (Char=='=') return 0;
	return -1;
}

int BASE64Decode(const BYTE *In,int Length,BYTE *Out) {
	if (Length%4!=0) return -1;

	int Pos=0;
	for (int I=0;I<Length/4;I++) {
		int Codes[4];
		if ((Codes[0]=Decoded(In[I*4]))<0) return -1;
		if ((Codes[1]=Decoded(In[I*4+1]))<0) return -1;
		if ((Codes[2]=Decoded(In[I*4+2]))<0) return -1;
		if ((Codes[3]=Decoded(In[I*4+3]))<0) return -1;
		Out[Pos++]=((Codes[0]<<2)&0xFC)+((Codes[1]>>4)&0x03);
		Out[Pos++]=((Codes[1]<<4)&0xF0)+((Codes[2]>>2)&0x0F);
		Out[Pos++]=((Codes[2]<<6)&0xC0)+((Codes[3]   )&0x3F);
	}
	if (In[Length-1]=='=') {
		Pos--;
		if (In[Length-2]=='=') Pos--;
	}
	return Pos;
}

int BASE64DecodeStr(const char *In,BYTE *Out) {
	return BASE64Decode((BYTE *)In,strlen(In),Out);
}

char *BASE64DecodeStrToStr(const char *In) {
	char *Result=(char *)malloc(Mul3Div4(strlen(In))+1);
	int Length=BASE64DecodeStr(In,(BYTE *)Result);
	if (Length==-1) {free(Result);return NULL;}
	Result[Length]=0;return Result;
}
