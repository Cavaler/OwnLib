 /* ansi.h header file. Copyright(C) 1998,Kostromitinov Pavel */
#ifndef __ANSI_H
#define __ANSI_H

void ansiClrScr(void);
void ansiClrEol(void);

void ansiGotoXY(int,int);
void ansiMoveUp(int);
void ansiMoveDown(int);
void ansiMoveRight(int);
void ansiMoveLeft(int);

void ansiOutputPosition(void);
void ansiSavePosition(void);
void ansiRestorePosition(void);
void ansiStopWrap(void);
#endif
