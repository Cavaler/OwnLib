#undef   _UNIX_
#include <stdio.h>

#ifdef _UNIX_
#include <ncurses.h>
#include <unistd.h>
#define rnd(num) (random()%(num))
#define readchar() wgetch(stdscr)
#else
#include "ncurses.h"
#define  rnd(num) random(num)
#endif

#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SNAKES       16
#define MAX_SNAKE_LEN 10000
#define DEF_SNAKE_LEN 20
#define MaxX    100
#define MaxY    50
#define MaxSize (MaxX*MaxY)
#define RABBIT_STR "@~"
#define HEAD_STR "[]"
#define BODY_STR "()"

char *Msgs[]={
 "Goodbye, impatient!",
 "You are great rabbit-eater!",
 "Trying to eat your own tail, yeah?",
 "Don't break your teeth upon his body, guy!",
 "This wall was too hard for your teeth... Try another..."
};

int x[MAX_SNAKE_LEN],y[MAX_SNAKE_LEN],Result=0,len=2;
char Field[MaxX][MaxY];

int rx,ry,SNAKE_LEN=DEF_SNAKE_LEN;
int dx=0,dy=-1,dir=0,PlayerNum=1;

void initrabbit() {
int i;
 do{
  rx=rnd(COLS/2-2)+1;
  ry=rnd(LINES-2)+1;
  for (i=0;i<len;i++) if ((x[i]==rx)&&(y[i]==ry)) break;
 }while (i!=len);
 move(ry,rx*2);addstr(RABBIT_STR);
 move(0,2);
#ifdef _UNIX_
 wprintw(stdscr,"%d of %d eaten ",len-2,SNAKE_LEN);
 refresh();
#else
 printf("%d of %d eaten ",len-2,SNAKE_LEN);
#endif
}

void dir2d(int cdir) {
 switch (cdir) {
  case 0: dx=0;  dy=-1; break;
  case 1: dx=1;  dy=0;  break;
  case 2: dx=0;  dy=1;  break;
  case 3: dx=-1; dy=0;  break;
 }
}

void step() {
int i;
 move(y[len-1],x[len-1]*2);addstr("  ");
 Field[x[len-1]][y[len-1]]=0;
 for (i=len-1;i>0;i--) {x[i]=x[i-1];y[i]=y[i-1];}
 x[0]+=dx;y[0]+=dy;
 if (Result=Field[x[0]][y[0]]) return;
 move(y[1],x[1]*2);addstr(BODY_STR);
 Field[x[1]][y[1]]=PlayerNum;
 move(y[0],x[0]*2);addstr(HEAD_STR);
 if ((x[0]==rx)&&(y[0]==ry)) {len++;initrabbit();if (len==SNAKE_LEN+2) Result=0xFE;}
}

void ParseCmdLine(int argc,char *argv[]) {
int i;
 for (i=1;i<argc;i++) {
  if (argv[i][0]=='-') {
        switch (argv[i][1]) {
         case 'L':case 'l':
                 SNAKE_LEN=atoi(&(argv[i][2]));
                 if (SNAKE_LEN==0) SNAKE_LEN=DEF_SNAKE_LEN;
                 if (SNAKE_LEN>MAX_SNAKE_LEN) SNAKE_LEN=MAX_SNAKE_LEN;
                 break;
         case 'S':case 's':
                 len=atoi(&(argv[i][2]));break;
        }
  };
 }
}

void Initialize() {
int i;
#ifdef _UNIX_
 srandom(time(NULL));
 initscr();cbreak();noecho();
 nodelay(stdscr,TRUE);keypad(stdscr,TRUE);
 box(stdscr,'#','#');
#else
 randomize();textattr(7);clrscr();
 for (i=1;i<82;i++) putch('#');
 for (i=2;i<25;i++) {gotoxy(80,i);cprintf("##");}
 gotoxy(1,25);for (i=1;i<79;i++) putch('#');
#endif
 memset(Field,sizeof(Field),0);
 for (i=0;i<COLS/2;i++) {Field[i][0]=0xFF;Field[i][LINES-1]=0xFF;}
 for (i=0;i<LINES;i++) {Field[0][i]=0xFF;Field[COLS/2-1][i]=0xFF;}
 x[0]=COLS/4;y[0]=LINES/2;initrabbit();
}

void Finalize() {
 switch (Result) {
  case -1:Result=4;break;
  case -2:Result=1;break;
  default:Result=(Result=PlayerNum)?2:3;
 }
 move(12,(COLS-strlen(Msgs[Result]))/2);
 addstr(Msgs[Result]);
#ifdef _UNIX_
 nodelay(stdscr,FALSE);getch();
 clear();refresh();endwin();
#else
 getch();clrscr();
#endif
}
