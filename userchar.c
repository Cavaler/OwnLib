#include <graphics.h>
#include <conio.h>
#define Str "Triplex"
 int xs,ys;
 struct textsettingstype TS;
void GetSizes() {
 xs=textwidth(Str);ys=textheight(Str);
 gettextsettings(&TS);outtext(Str);
}

void main () {
 xs=0;initgraph(&xs,&ys,"G:\\TP70\\BGI");
 settextstyle(1,0,0);GetSizes();
 settextstyle(1,0,1);GetSizes();
 setusercharsize(1,xs,1,xs);GetSizes();
 setusercharsize(50,1,50,1);GetSizes();
 setusercharsize(xs,50,ys,50);GetSizes();
 settextstyle(1,0,0);GetSizes();
 settextstyle(1,0,1);GetSizes();
 getch();closegraph();
}
