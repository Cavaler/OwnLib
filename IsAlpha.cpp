#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <locale.h>
//#include "DOS866.c"

int main(int argc,char *argv[]) {
	int C;

	printf("Locale set to %s\n",setlocale(LC_ALL,(argc>1)?argv[1]:".1251"));
	for (C=0;C<256;C++) if (isupper(C)||islower(C)) printf("%c",C);
	printf("\n");
	for (C=0;C<256;C++) if (isalpha(C)) printf("%c",C);
	printf("\n");
	for (C=0;C<256;C++) {
		if (isalpha(C)) printf("%c",(isupper(C))?tolower(C):toupper(C));
	}
	printf("\n");
	return 0;
}
