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
                        if (strcmp(b,"Agrave")==0) {fputc('À',Fout);break;}
                        if (strcmp(b,"Aacute")==0) {fputc('Á',Fout);break;}
                        if (strcmp(b,"Acirc")==0) {fputc('Â',Fout);break;}
                        if (strcmp(b,"Atilde")==0) {fputc('Ã',Fout);break;}
                        if (strcmp(b,"Auml")==0) {fputc('Ä',Fout);break;}
                        if (strcmp(b,"Aring")==0) {fputc('Å',Fout);break;}
                        if (strcmp(b,"AElig")==0) {fputc('Æ',Fout);break;}
                        if (strcmp(b,"Ccedil")==0) {fputc('Ç',Fout);break;}
                        if (strcmp(b,"Egrave")==0) {fputc('È',Fout);break;}
                        if (strcmp(b,"Eacute")==0) {fputc('É',Fout);break;}
                        if (strcmp(b,"Ecirc")==0) {fputc('Ê',Fout);break;}
                        if (strcmp(b,"Euml")==0) {fputc('Ë',Fout);break;}
                        if (strcmp(b,"Igrave")==0) {fputc('Ì',Fout);break;}
                        if (strcmp(b,"Iacute")==0) {fputc('Í',Fout);break;}
                        if (strcmp(b,"Icirc")==0) {fputc('Î',Fout);break;}
                        if (strcmp(b,"Iuml")==0) {fputc('Ï',Fout);break;}
                        if (strcmp(b,"ETH")==0) {fputc('Ð',Fout);break;}
                        if (strcmp(b,"Ntilde")==0) {fputc('Ñ',Fout);break;}
                        if (strcmp(b,"Ograve")==0) {fputc('Ò',Fout);break;}
                        if (strcmp(b,"Oacute")==0) {fputc('Ó',Fout);break;}
                        if (strcmp(b,"Ocirc")==0) {fputc('Ô',Fout);break;}
                        if (strcmp(b,"Otilde")==0) {fputc('Õ',Fout);break;}
                        if (strcmp(b,"Ouml")==0) {fputc('Ö',Fout);break;}
                        if (strcmp(b,"cross")==0) {fputc('×',Fout);break;}
                        if (strcmp(b,"Oslash")==0) {fputc('Ø',Fout);break;}
                        if (strcmp(b,"Ugrave")==0) {fputc('Ù',Fout);break;}
                        if (strcmp(b,"Uacute")==0) {fputc('Ú',Fout);break;}
                        if (strcmp(b,"Ucirc")==0) {fputc('Û',Fout);break;}
                        if (strcmp(b,"Uuml")==0) {fputc('Ü',Fout);break;}
                        if (strcmp(b,"Yacute")==0) {fputc('Ý',Fout);break;}
                        if (strcmp(b,"THORN")==0) {fputc('Þ',Fout);break;}
                        if (strcmp(b,"szlig")==0) {fputc('ß',Fout);break;}
                        if (strcmp(b,"agrave")==0) {fputc('à',Fout);break;}
                        if (strcmp(b,"aacute")==0) {fputc('á',Fout);break;}
                        if (strcmp(b,"acirc")==0) {fputc('â',Fout);break;}
                        if (strcmp(b,"atilde")==0) {fputc('ã',Fout);break;}
                        if (strcmp(b,"auml")==0) {fputc('ä',Fout);break;}
                        if (strcmp(b,"aring")==0) {fputc('å',Fout);break;}
                        if (strcmp(b,"aelig")==0) {fputc('æ',Fout);break;}
                        if (strcmp(b,"ccedil")==0) {fputc('ç',Fout);break;}
                        if (strcmp(b,"egrave")==0) {fputc('è',Fout);break;}
                        if (strcmp(b,"eacute")==0) {fputc('é',Fout);break;}
                        if (strcmp(b,"ecirc")==0) {fputc('ê',Fout);break;}
                        if (strcmp(b,"euml")==0) {fputc('ë',Fout);break;}
                        if (strcmp(b,"igrave")==0) {fputc('ì',Fout);break;}
                        if (strcmp(b,"iacute")==0) {fputc('í',Fout);break;}
                        if (strcmp(b,"icirc")==0) {fputc('î',Fout);break;}
                        if (strcmp(b,"iuml")==0) {fputc('ï',Fout);break;}
                        if (strcmp(b,"eth")==0) {fputc('ð',Fout);break;}
                        if (strcmp(b,"ntilde")==0) {fputc('ñ',Fout);break;}
                        if (strcmp(b,"ograve")==0) {fputc('ò',Fout);break;}
                        if (strcmp(b,"oacute")==0) {fputc('ó',Fout);break;}
                        if (strcmp(b,"ocirc")==0) {fputc('ô',Fout);break;}
                        if (strcmp(b,"otilde")==0) {fputc('õ',Fout);break;}
                        if (strcmp(b,"ouml")==0) {fputc('ö',Fout);break;}
                        if (strcmp(b,"divis")==0) {fputc('÷',Fout);break;}
                        if (strcmp(b,"oslash")==0) {fputc('ø',Fout);break;}
                        if (strcmp(b,"ugrave")==0) {fputc('ù',Fout);break;}
                        if (strcmp(b,"uacute")==0) {fputc('ú',Fout);break;}
                        if (strcmp(b,"ucirc")==0) {fputc('û',Fout);break;}
                        if (strcmp(b,"uuml")==0) {fputc('ü',Fout);break;}
                        if (strcmp(b,"yacute")==0) {fputc('ý',Fout);break;}
                        if (strcmp(b,"thorn")==0) {fputc('þ',Fout);break;}
                        if (strcmp(b,"yuml")==0) {fputc('ÿ',Fout);break;}

   default:fputc(c,Fout);
  }
 }
 fclose(Fin);fclose(Fout);
 return 0;
}

