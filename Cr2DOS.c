/* Copyright (C) 1999,Kostromitinov Pavel */
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
	FILE *F;
	char *Data,*Name,*Cur;
	size_t DataSize;

    if (!(F=fopen(argv[1],"rb"))) {
    	printf("Cannot open %s.\n",argv[1]);return 1;
    }
	fseek(F,0,SEEK_END);
    DataSize=ftell(F);
    rewind(F);
    Data=(char *)malloc(DataSize+1);
    if (fread(Data,1,DataSize,F)!=DataSize) {
    	printf("Cannot read %s.\n",argv[1]);
        free(Data);fclose(F);
        return 2;
    };
    fclose(F);

	if (argc==2) Name=argv[1]; else Name=argv[2];
	if (!(F=fopen(Name,"wb"))) {
    	printf("Cannot create %s.\n",Name);
        free(Data);
		return 3;
	}

	Data[DataSize]=0;Cur=Data;
	while (*Cur) {
		if (*Cur=='\r') {
            fwrite(Data,Cur-Data,1,F);
            fprintf(F,"\r\n");
			do Cur++; while (*Cur=='\r');
			while (*Cur=='\n') Cur++;
			strcpy(Data,Cur);Cur=Data;
		} else Cur++;
	}
	if (Cur!=Data) fwrite(Data,strlen(Data),1,F);
	free(Data);fclose(F);
	return 0;
}
