#include <conio.h>
#include <dos.h>
#define move(y,x) gotoxy(x+1,y+1)
#define addstr(str) cprintf(str)
#define addch(char) putch(char)
int COLS=80;
int LINES=25;
#define refresh()
#define readchar() (kbhit()?getcharex():0)
#define usleep(micro) delay((micro)/1000)
#define KEY_HOME  71*0x100
#define KEY_END   79*0x100
#define KEY_UP    72*0x100
#define KEY_DOWN  80*0x100
#define KEY_LEFT  75*0x100
#define KEY_RIGHT 77*0x100
int getcharex() {
char ch;
 ch=getch();if (ch==0) {
  ch=getch();return ch*0x100;
 } else return ch;
}
