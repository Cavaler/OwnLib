#include <stdio.h>
#include <windows.h>
#include <winsock.h>

extern HANDLE __hInstance;

void ShowError(char *Message) {
 char Buffer[256];
 LoadString(__hInstance,WSAGetLastError(),Buffer,256);
 printf("%s: %s\n",Message,Buffer);
}

void ShowCriticalError(char *Message) {
 ShowError(Message);WSACleanup();
 scanf("%*c");_DoneEasyWin();
}

void PrintHostEnt(struct hostent FAR *HostEnt) {
 int I;
 printf("Official host name: %s\n",HostEnt->h_name);
 for (I=0;HostEnt->h_aliases[I];I++) printf("Alias: %s",HostEnt->h_aliases[I]);
 printf("Address type: %d",HostEnt->h_addrtype);
 for (I=0;HostEnt->h_addr_list[I];I++) {
  printf("Address: %s",HostEnt->h_addr_list[I]);
 }
}

int main(int argc,char *argv[]) {
 WSADATA Data;
 SOCKET ListenSocket;
 struct sockaddr_in S_in;
 struct hostent FAR *HostEnt;
 char HostName[128];
 long HostAddr;
 _InitEasyWin();
 if (WSAStartup(0x0101,&Data)) {
  ShowCriticalError("WSAStartup error");return 1;
 }
 printf("Winsock version %d.%d: %s\n",LOBYTE(Data.wVersion),HIBYTE(Data.wVersion),
 Data.szDescription);
 printf("System status: %s\n",Data.szSystemStatus);
 printf("Max sockets: %d, max UDP datagram: %d\n",Data.iMaxSockets,Data.iMaxUdpDg);
 if (gethostname(HostName,128)==0) {
  printf("This host is: %s\n",HostName);
  HostEnt=gethostbyname(HostName);
  if (HostEnt) {printf("gethostbyname() returned: \n");PrintHostEnt(HostEnt);
  } else ShowError("Cannot get local host IP");
 } else ShowError("Cannot get local host name");

 HostAddr=inet_addr("127.0.0.1");
 HostEnt=gethostbyaddr((char*)HostAddr,4,PF_INET);
 if (HostEnt) {printf("gethostbyaddr() returned: \n");PrintHostEnt(HostEnt);
 } else ShowError("Cannot get 127.0.0.1 host");

 ListenSocket=socket(AF_INET,SOCK_STREAM,0);
 if (ListenSocket==INVALID_SOCKET) {
  ShowCriticalError("Cannot create socket");return 2;
 };
 S_in.sin_family=AF_INET;
 S_in.sin_port=htons(20601);
 S_in.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
 if (bind(ListenSocket,(struct sockaddr*)&S_in,sizeof(S_in))) {
  ShowCriticalError("Cannot BIND socket");return 3;
 };
 ShowCriticalError("All went OK");
 return 0;
}
