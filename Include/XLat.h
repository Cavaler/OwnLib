#ifndef __XLAT_H
#define __XLAT_H

enum XLat_CodePage {CP_OEM,CP_ANSI,CP_KOI8};

void XLat_OEM2ANSI(char *From,char *To,int Length=0);
void XLat_ANSI2OEM(char *From,char *To,int Length=0);
void XLat_OEM2KOI8(char *From,char *To,int Length=0);
void XLat_KOI82OEM(char *From,char *To,int Length=0);
void XLat_KOI82ANSI(char *From,char *To,int Length=0);
void XLat_ANSI2KOI8(char *From,char *To,int Length=0);
void XLat(char *From,XLat_CodePage FromCP,char *To,XLat_CodePage ToCP,int Length=0);

#endif
