#include <stdio.h>
#include <string.h>

int main (int argc,char *argv[]) {
 FILE *Fin,*Fout;
 char c,b[256];int n;

#ifdef _Windows
 _InitEasyWin();
#endif
 if (argc!=3) {printf("2 arguments required\n");return 1;}
 if ((Fin=fopen(argv[1],"rb"))==NULL) {
  printf("Cannot open%s\n",argv[1]);return 2;
 }
 if ((Fout=fopen(argv[2],"wb"))==NULL) {
  printf("Cannot create%s\n",argv[2]);fclose(Fin);return 3;
 }
 while (!feof(Fin)) {
  switch (c=fgetc(Fin)) {
   case EOF:if (!feof(Fin)) fputc(c,Fout);break;
   case '<':while (fgetc(Fin)!='>');break;
   case '&':for (n=0;b[n-1]!=';';b[n++]=fgetc(Fin));
                        b[n-1]=0;
                        if (b[0]=='#') {sscanf(&b[1],"%d",&b[0]);fputc(b[0],Fout);break;}
                        if (strcmp(b,"nbsp")==0) {fputc(' ',Fout);break;}
                        if (strcmp(b,"amp")==0) {fputc('&',Fout);break;}
                        if (strcmp(b,"lt")==0) {fputc('<',Fout);break;}
                        if (strcmp(b,"gt")==0) {fputc('>',Fout);break;}
                        if (strcmp(b,"Agrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Aacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Acirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Atilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Auml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Aring")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"AElig")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ccedil")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Egrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Eacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ecirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Euml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Igrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Iacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Icirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Iuml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ETH")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ntilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ograve")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Oacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ocirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Otilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ouml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"cross")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Oslash")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ugrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Uacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Ucirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Uuml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"Yacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"THORN")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"szlig")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"agrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"aacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"acirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"atilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"auml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"aring")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"aelig")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ccedil")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"egrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"eacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ecirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"euml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"igrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"iacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"icirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"iuml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"eth")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ntilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ograve")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"oacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ocirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"otilde")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ouml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"divis")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"oslash")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ugrave")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"uacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"ucirc")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"uuml")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"yacute")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"thorn")==0) {fputc('�',Fout);break;}
                        if (strcmp(b,"yuml")==0) {fputc('�',Fout);break;}

   default:fputc(c,Fout);
  }
 }
 fclose(Fin);fclose(Fout);
 return 0;
}

