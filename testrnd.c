 /* testrnd.c program. Copyright (C) 1996,Kostromitinov Pavel */
#pragma hdrfile
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#pragma hdrstop
#define MaxX 300
void main() {
 int gd=0,x,a;

 char y[MaxX];
 initgraph(&gd,&x,"C:\\BC31\\BGI\\");
 for (gd=0;gd<MaxX;gd++) y[gd]=0;
 randomize();
 for (gd=0;gd<30000;gd++) {
  a=random(MaxX);
  putpixel(a,y[a]++,1);
 }
 getch();closegraph();
}
