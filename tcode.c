#include "altcode.h"
#include <string.h>
#include <stdio.h>

int const X='.';

int UnixFlag=0;
int FATFlag=1;
int NTFSFlag=0;
int HPFSFlag=0;

int change_flag=0;

int MaxLenth=254;
int MaxExt=1;

int i,j,lenth,step=0,names=0;
int cht,ch;


enum CODEPAGE {ALT866,WIN,KOI8,KOI7,SISO,DKOI,MIC,IBM,VAX,ISO,MAC,NUL};
enum OTHERS {UPPER,LOWER,CYR,LAT,TS,NL};
enum CODEPAGE cpIN=NUL,cpOUT=NUL;
enum OTHERS oth[3]={NL,NL,NL};


FILE* src;
FILE* dst;

static char Name[2][255];

static char* String1="Control print 1.";
static char* String2="WinCode/Encode programm \n(c) Sergey Y. Shilov. 1995-1999\nv1.2\n";
static char* String3="Usage: altcode {-|+}switches InputFileName.Ext [OutputFileName.Ext]\nUse ? switch for more help.\n\0";
static char* String4="Error oqured.\n\0";
static char* String5="No such file in directory or duplicate existing file.\n\0";
static char* String6="Unrecognized switch is ignored.\n\0";
static char* String7="Abnormal combination of switches is ignored.\n\0";

static char* String8="Usage: altcode {-|+}switches InputFileName.Ext [OutputFileName.Ext].\n\
Use - sign for input codepage, + sign for output.\n\n\
Emplemented switches are:\n\
d - DOS (866, \"Alt\")          w - MS Windows 1251\n\
8 - KOI8                      7 - KOI7\n\
s - KOI7SISO                  o - MAC\n\
i - USSR GOST                 m - MIC (\"Bulgarian\")\n\
e - EBCDIC (DKOI) codepage    v - VAX (Extended ASCII)\n";

static char* String9="\nIf no filenames specified stdin/stdout will be used instead.\n";
static char* String10="-c - use MS-DOS 8+3 compatibility name format.\n";
static char* String11="-h This help screen.\n";

static char* String12="Unrecognized tag - ignored.\n";

void CreateBackUpName(char*,char*,int,int);
int CreateBackUpFile(char*,char*);
int code (FILE*,FILE*,enum CODEPAGE,enum CODEPAGE);

void Error(int);

int main (int argc, char* argv[])
{
  printf("%s",String2);
 for (i=argc-1;i>=0;i--)
 {
     if((argv[i][0]!='-')&&(argv[i][0]!='+'))
    {
       if(names<=2)
        {
       strcpy(Name[1-names],argv[i]);
       names++;
        }
     else{printf("Abnormal number of filnames.");return 1;}
      }
  else
   {
  switch (argv[i][0]){
   case '-':   //¡“„Œžš
  for(j=1;j<strlen(argv[i]);j++)
     {
     switch (argv[i][j]) {
        case 'h':
        case 'H':
        case '?':
       {printf("%s%s%s%s",String8,String10,String11,String9);return 0;}
        case 'c':
       {MaxLenth=12;MaxExt=4;}
       break;
        case 'd':
      {if (cpIN==NUL|cpIN==ALT866){cpIN=ALT866;} else printf("%s",String7);}
      break;
        case 'w':
      {if (cpIN==NUL|cpIN==WIN){cpIN=WIN;} else printf("%s",String7);}
      break;
        case '7':
      {if (cpIN==NUL|cpIN==KOI7){cpIN=KOI7;} else printf("%s",String7);}
      break;
        case '8':
      {if (cpIN==NUL|cpIN==KOI8){cpIN=KOI8;} else printf("%s",String7);}
      break;
        case 's':
      {if (cpIN==NUL|cpIN==SISO){cpIN=SISO;} else printf("%s",String7);}
      break;
        case 'o':
      {if (cpIN==NUL|cpIN==MAC){cpIN=MAC;} else printf("%s",String7);}
      break;
        case 'm':
      {if (cpIN==NUL|cpIN==MIC){cpIN=MIC;} else printf("%s",String7);}
      break;
        case 'i':
      {if (cpIN==NUL|cpIN==IBM){cpIN=IBM;} else printf("%s",String7);}
      break;
        case 'e':
      {if (cpIN==NUL|cpIN==DKOI){cpIN=DKOI;} else printf("%s",String7);}
      break;
        case 'v':
      {if (cpIN==NUL|cpIN==VAX){cpIN=VAX;} else printf("%s",String7);}
      break;
 /*                     case 'u':
      {if (cpIN==NUL|cpIN==UCODE){cpIN=UCODE;} else printf("%s",String7);}
      break;*/
        default:
      printf("%s",String6);
      break;
        }
     } break;

  case '+': //¡˜“„Œžš
  for(j=1;j<strlen(argv[i]);j++)
     {
        switch (argv[i][j]) {
        case 'h':
        case 'H':
        case '?':
       {printf("%s%s%s%s",String8,String10,String11,String9);return 0;}
        case 'a':
      {if (cpOUT==NUL|cpOUT==ALT866){cpOUT=ALT866;} else printf("%s",String7);}
      break;
        case 'w':
      {if (cpOUT==NUL|cpOUT==WIN){cpOUT=WIN;} else printf("%s",String7);}
      break;
        case '7':
      {if (cpOUT==NUL|cpOUT==KOI7){cpOUT=KOI7;} else printf("%s",String7);}
      break;
        case '8':
      {if (cpOUT==NUL|cpOUT==KOI8){cpOUT=KOI8;} else printf("%s",String7);}
      break;
        case 's':
      {if (cpOUT==NUL|cpOUT==SISO){cpOUT=SISO;} else printf("%s",String7);}
      break;
        case 'o':
      {if (cpOUT==NUL|cpOUT==MAC){cpOUT=MAC;} else printf("%s",String7);}
      break;
        case 'm':
      {if (cpOUT==NUL|cpOUT==MIC){cpOUT=MIC;} else printf("%s",String7);}
      break;
        case 'i':
      {if (cpOUT==NUL|cpOUT==IBM){cpOUT=IBM;} else printf("%s",String7);}
      break;
        case 'd':
      {if (cpOUT==NUL|cpOUT==DKOI){cpOUT=DKOI;} else printf("%s",String7);}
      break;
        case 'v':
      {if (cpOUT==NUL|cpOUT==VAX){cpOUT=VAX;} else printf("%s",String7);}
      break;
        /*case 'u':
      {if (cpOUT==NUL|cpOUT==UCODE){cpOUT=UCODE;} else printf("%s",String7);}
      break;*/
        default:
      printf("%s",String6);
      break;
        }
     } break;
  default:
     printf("%s",String12);
     break;
  }
      }
 }

   if (cpIN==NUL) {cpIN=KOI8;}
   if (cpOUT==NUL) {cpOUT=KOI8;}

//if(names==0){printf("%s",String3);return 1;}

//printf("\n\n## %d ##\n\n",names);

   if(names==3)
   {
 if((src=fopen(Name[0],"rb"))==NULL) {/*printf("#2\n")*/;return 1;}
 if((dst=fopen(Name[1],"wb"))==NULL) {/*printf("#3\n")*/;return 1;}
//      printf ("%s => %s",Name[0],Name[1]);
 }


   if(names==2)
   {
   CreateBackUpName(Name[1],Name[0],MaxLenth,MaxExt);
   rename(Name[1],Name[0]);
 if((src=fopen(Name[0],"rb"))==NULL) {/*printf("#2\n")*/;return 1;}
 if((dst=fopen(Name[1],"wb"))==NULL) {/*printf("#3\n")*/;return 1;}
//      printf ("%s => %s",Name[0],Name[1]);
 }

   if(names==1){src=stdin;dst=stdout;}

//printf("\n\n## %d => %d ##\n\n",cpIN, cpOUT);

code (src,dst,cpIN,cpOUT);

fclose(src);
fclose(dst);



return 0;
}

void Error(int sStep)
{
/*   switch(sStep)
      {
 case 0: break;
 case 1: printf("%s",String5);break;
 case 2: printf("%s",String5);break;

 default: printf("%s",String4);break;
      }
   return 0;
*/
}



void CreateBackUpName(char* Name,char* BackName,int MaxNameLenth,int MaxExtLenth)
{
 int lenth,i,res;
 lenth=strlen(Name);
 i=lenth;

 for (i=lenth;i>=0;i--)
  {
  if (Name[i]==X) break;
  }
 if (i==-1) i=lenth;
 res=MaxNameLenth-MaxExtLenth;

//        printf("#1 i=%d,res=%d\n",i,res);

 if      (i>res) {i=res;}

//        printf("#1 i=%d,res=%d\n",i,res);

 strcpy(BackName,Name);

 if (i<=MaxNameLenth-3) {BackName[i]='.';BackName[i+1]='b';BackName[i+2]='k';BackName[i+3]='p';BackName[i+4]=0;}
 if (i==MaxNameLenth) BackName[i]='-';
 if (i==MaxNameLenth-1) {BackName[i]='.';BackName[i+1]='b';}
 if (i==MaxNameLenth-2) {BackName[i]='.';BackName[i+1]='b';BackName[i+2]='k';}

//        printf("%s ==> %s",BackName,Name);
}

int code (FILE* sr,FILE* ds,enum CODEPAGE IN,enum CODEPAGE OUT)
{

while((ch=fgetc(sr))!=EOF){
      //ch=cht;
      //cht=ch-altcode[0][IN][ch-1]+altcode[1][OUT][ch-1];

  ch-=altcode[0][IN][ch];
  ch+=altcode[1][OUT][ch];


  fputc(ch,ds);}


return 0;
}
