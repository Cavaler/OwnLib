#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


#define ADDRESS "127.0.0.1"
#define TEMPLATE "Data received from %u server...\n"
#define SOCKET int
#define PORT 1357

int main(int argn, char *argv[])
{
 SOCKET SocketForWaiting, SocketForCommunication;
 struct sockaddr_in *MyAddress, *HisAddress;
 int HisAddressLen;
 char Buffer[1000];
 char HostName[63];
 struct hostent *Host;
 pid_t pID;
 long i;
 double DBL;
 u_long Command;
 FILE *LogFile;


 if((LogFile = fopen("ss.log","a")) == NULL)
        {
                printf("File error.\n");
        }

 MyAddress = (struct sockaddr_in*)malloc(1000);
 HisAddress = (struct sockaddr_in*)malloc(1000);
 HisAddressLen = 1000;

/* if (strcmp(argv[1],"-l")) */
 if (argn!=1) gethostname(HostName,63);
              else strcpy(HostName,"localhost");
 Host = gethostbyname(HostName);

 fprintf(LogFile,"Start first copy of Socket Server on %s.\n",HostName);
 if(!Host)
 {
  fprintf(LogFile,"Host is NULL\n");
  exit(1);
 }
 fprintf(LogFile,"Host name: %s\n",Host->h_name);
 fprintf(LogFile,"Host address: %u\n",Host->h_addr);

 MyAddress->sin_family = AF_INET;
 MyAddress->sin_port = htons(PORT);
 MyAddress->sin_addr.s_addr = *(u_long *)Host->h_addr_list[0];

 fprintf(LogFile,"Address complete...\n");

 if((SocketForWaiting = socket(PF_INET,SOCK_STREAM,0)) == -1)
 {
   fprintf(LogFile,"SOCKET error, %u\n",errno);
   exit(1);
 }

 fprintf(LogFile,"SOCKET complete...\n");

 if(bind(SocketForWaiting,(struct sockaddr*)MyAddress,sizeof(*MyAddress)) != 0)
 {
  fprintf(LogFile,"BIND error, %u...\n",errno);
  exit(1);
 }

 fprintf(LogFile,"BIND complete...\n");

 if(listen(SocketForWaiting,16) !=0 )
 {
  fprintf(LogFile,"LISTEN error, &u...\n",errno);
  exit(1);
 }

 daemon();

 fprintf(LogFile,"LISTEN complete...\n");

 while(SocketForCommunication = accept(SocketForWaiting,(struct sockaddr*)HisAddress,&HisAddressLen))
 {
   if(!fork())
   {
     fprintf(LogFile,"New client, child started...\n");
     if(SocketForCommunication == -1)
     {
      fprintf(LogFile,"Child: accept error...\n");
      exit(1);
     };

    fprintf(LogFile,"Receiving command...\n");
    recv(SocketForCommunication,&Command,sizeof(Command),0);
    Command = ntohl(Command);
    if(0x12345678 == Command)
    {
     fprintf(LogFile,"Shutting down...\n");
     if(kill(getppid(),SIGKILL) !=0 )
     {
       fprintf(LogFile,"I can not kill parent: %u...\n",errno);
     }
     exit(0);
    }
    else
    {
     fprintf(LogFile,"Unlnown command: %u\n",Command);
    }

    pID = getpid();
    sprintf(Buffer,TEMPLATE,pID);
    fprintf(LogFile,"\"Calculating\"...\n");
    sleep(5);
    send(SocketForCommunication,Buffer,strlen(Buffer),0);
    fprintf(LogFile,"Data sent...\n");
    shutdown(SocketForCommunication,2);
    close(SocketForCommunication);
    fprintf(LogFile,"Child exited...\n");
    exit(0);
   }
   else
   {
        fprintf(LogFile,">>> Main server in While\n");
   }
 }
 fprintf(LogFile,"Main server exiting...\n");
 fprintf(LogFile,"Error: %u\n",errno);
 fclose(LogFile);
 close(SocketForWaiting);
 free(MyAddress);
 free(HisAddress);
 return 0;
}

