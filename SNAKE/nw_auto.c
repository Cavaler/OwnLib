#include "nwclient.h"

int autorun() {
int qux[MaxSize], quy[MaxSize], head, tail;
int dx[4]={0, -1, 0, 1};
int dy[4]={1, 0, -1, 0};
int was[MaxX][MaxY];
int tx, ty, i, j, pc, pm;
 head=2; tail=1; qux[1]=rx; quy[1]=ry;
 memset(was,0,sizeof(was));
 was[rx][ry]=1;
 while (head!=tail)
 {
  tx=qux[tail]; ty=quy[tail]; tail++;
  if (tail>MaxSize) tail=0;
  for (i=0; i<4; i++)
	if ( (was[tx+dx[i]][ty+dy[i]]==0) && (Field[tx+dx[i]][ty+dy[i]]==0) ) {
		was[tx+dx[i]][ty+dy[i]]=1;
		qux[head] = tx + dx[i];
		quy[head] = ty + dy[i]; head++;
		if (head>MaxSize) head=0;
		if ((tx+dx[i]==x[0]) && (ty+dy[i]==y[0]))
		 return i;
	 }
 }
/*	No path found...*/
 pm=0;j=0;
 for (i=0;i<4;i++) {
  tx=x[0];ty=y[0];pc=0;
  while (Field[tx][ty]==0) {tx-=dx[i];ty-=dy[i];pc++;};
  if (pc>pm) {pm=pc;j=i;}
 }
 return j;
}

void main(int argc,char *argv[]) {
int ch;
 ParseCmdLine(argc,argv);Initialize();
 do {
  step();move(0,0);refresh();
  usleep(15000-10000*len/SNAKE_LEN);
  ch=readchar();
  dir2d(autorun());
 } while ((ch!='Q')&&(Result==0));
 Finalize();
}
