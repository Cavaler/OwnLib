#include <ansi.h>
#include <stdio.h>

void ansiClrScr(void) {
 printf("\x1b[2J");
}

void ansiClrEol(void) {
 printf("\x1b[K");
}

void ansiGotoXY(int x,int y) {
 printf("\x1b[%d;%dH",y,x);
}

void ansiMoveUp(int n) {
 printf("\x1b[%dA",n);
}

void ansiMoveDown(int n) {
 printf("\x1b[%dB",n);
}

void ansiMoveRight(int n) {
 printf("\x1b[%dC",n);
}

void ansiMoveLeft(int n) {
 printf("\x1b[%dD",n);
}

void ansiOutputPosition(void) {
 printf("\x1b[6n");
}

void ansiSavePosition(void) {
 printf("\x1b[s");
}

void ansiRestorePosition(void) {
 printf("\x1b[u");
}

void ansiStopWrap(void) {
 printf("\x1b[=7l");
}
