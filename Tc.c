/* Copyright (C) 1999,Kostromitinov Pavel */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARATOR '\\'

int main(int argc,char *argv[]) {
	FILE *F;
    unsigned char Table[256];
    char *Data,*Name;
	size_t DataSize,I;

	if ((argc!=3)&&(argc!=4)) {
	    printf("Usage: TC codetable File_In [File_Out]\n");
        return 1;
    }

    strcpy(Name=(char *)malloc(strlen(argv[0])+strlen(argv[1])+4),argv[0]);
    strcat(strcpy(strrchr(Name,SEPARATOR)+1,argv[1]),".xlt");
    if (!(F=fopen(Name,"rb"))) {
    	printf("Cannot open %s.\n",Name);return 2;
    }
    I=fread(Table,1,256,F);
    if (I==128) {
    	for (I=0;I<128;I++) {Table[I+128]=Table[I];Table[I]=I;}
    } else {
    	if (I!=256) {
    		printf("Cannot read %s.\n",Name);
        	free(Name);fclose(F);
        	return 3;
        }
    }
    free(Name);fclose(F);
    if (!(F=fopen(argv[2],"rb"))) {
    	printf("Cannot open %s.\n",argv[2]);return 4;
    }
	fseek(F,0,SEEK_END);
    DataSize=ftell(F);
    rewind(F);
    Data=(char *)malloc(DataSize);
    if (fread(Data,1,DataSize,F)!=DataSize) {
    	printf("Cannot read %s.\n",argv[2]);
        free(Data);fclose(F);
        return 5;
    };
    fclose(F);

	if (argc==3) Name=argv[2]; else Name=argv[3];
	if (!(F=fopen(Name,"wb"))) {
    	printf("Cannot create %s.\n",Name);
		return 6;
	}
	for (I=0;I<DataSize;I++) Data[I]=Table[(unsigned char)Data[I]];
    if (fwrite(Data,1,DataSize,F)!=DataSize) {
    	printf("Cannot write %s.\n",Name);
		free(Data);fclose(F);
		return 7;
	}

	free(Data);fclose(F);
	return 0;
}
