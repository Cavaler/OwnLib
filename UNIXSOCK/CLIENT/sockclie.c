#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>


#define SOCKET int
#define PORT 1357

int main(int argn, char *argv[])
{
 SOCKET SocketForCommunication;
 struct sockaddr_in *HisAddress;
 int HisAddressLen;
 char Buffer[1000];
 char HostName[63];
 struct hostent *Host;
 int Count;
 u_long Command = 0;

 if(argn != 2 && argn != 3)
 {
  printf("Syntax: %s <HOST> [+]\n\t+ - shutdown server.\n",argv[0]);
  exit(1);
 }

 if(3 == argn && '+' == argv[2][0])
 {
  printf("I'll shut down server\n");
  Command = htonl(0x12345678);
 }

 HisAddress = (struct sockaddr_in*)malloc(1000);
 HisAddressLen = 1000;
 

 /*gethostname(HostName,63);*/
 strcpy(HostName,argv[1]);  
 Host = gethostbyname(HostName); 

 printf("Start Socket client for %s.\n",HostName);
 if(!Host)
 {
  printf("Host is NULL\n");
  exit(1);
 }
 printf("Host name: %s\n",Host->h_name);
 printf("Host address: %u\n",Host->h_addr); 

 HisAddress->sin_family = AF_INET;
 HisAddress->sin_port = htons(PORT);
 HisAddress->sin_addr.s_addr = *(u_long *)Host->h_addr_list[0];
 
 printf("Address complete...\n");

 if((SocketForCommunication = socket(PF_INET,SOCK_STREAM,0)) == -1)
 {
   printf("SOCKET error, %u\n",errno);
   exit(1);
 } 

 printf("SOCKET complete...\n");

 if(connect(SocketForCommunication,(struct sockaddr*)HisAddress,sizeof(*HisAddress)) != 0)
 {
   printf("CONNECT error, %u\n",errno);
   exit(1);
 }

 printf("CONNECT complete...\n");

 Count = 0;

 if(send(SocketForCommunication,&Command,sizeof(Command),0) != sizeof(Command))
 {
   printf("Oops.. I can not sent command...\n");
   exit(1);
 }
   printf("SEND succesfull...\n");

 if(0 == Command)
 {
  while(recv(SocketForCommunication,&Buffer[Count],1,0) == 1)
  {
   printf("%c",Buffer[Count]);
   Count++;
  }
  printf("Received from server: \"%s\"",Buffer);
 }
 
 close(SocketForCommunication);
 free(HisAddress);
 return 0;
}

