#include "nwclient.h"

int reld=0;
char Player[40];

void main(int argc,char *argv[]) {
int ch;
 ParseCmdLine(argc,argv);Initialize();
 move(0,(COLS-strlen(Player))/2-1);
 addch('-');addstr(Player);addch('-');
 do {
  step();move(0,0);refresh();
  usleep(150000-100000*len/SNAKE_LEN);
  ch=readchar();

  switch (ch) {
	case KEY_HOME: reld=(!reld); break;
  }
  if (reld) {
   switch (ch) {
    case KEY_LEFT: dir--; break;
    case KEY_RIGHT: dir++;break;
   }
   dir=(dir+4)%4;
  } else {
   switch (ch) {
    case KEY_UP: dir=0;break;
    case KEY_DOWN: dir=2;break;
    case KEY_LEFT: dir=3;break;
    case KEY_RIGHT: dir=1;break;
   }
  }
  dir2d(dir);
 } while ((ch!='Q')&&(Result==0));
 Finalize();
}
