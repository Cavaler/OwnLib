/* Copyright (C) 2000,Kostromitinov Pavel */
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdio.h>

BYTE FindWhat[]={0,'5',0,'.',0,'0',0,'0',0,'.',0,'2',0,'9',0,'1',0,'9'};
BYTE ReplaceWith[]={0,'5',0,'.',0,'0',0,'0',0,'.',0,'2',0,'0',0,'0',0,'0'};

void ScanFile(char *FileName) {
	HANDLE hFile=CreateFile(FileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD Error=GetLastError();
	if (hFile==INVALID_HANDLE_VALUE) return;
	HANDLE hMap=CreateFileMapping(hFile,NULL,PAGE_READWRITE|SEC_COMMIT,0,0,NULL);
	if (hMap==NULL) {CloseHandle(hFile);return;}
	BYTE *FileData=(BYTE *)MapViewOfFile(hMap,FILE_MAP_WRITE,0,0,0);
	if (FileData==NULL) {CloseHandle(hMap);CloseHandle(hFile);return;}

	DWORD FileSize=GetFileSize(hFile,NULL);

	BYTE *CurPtr=FileData;
	while (CurPtr<FileData+FileSize-sizeof(FindWhat)) {
		CurPtr=(BYTE *)memchr(CurPtr,0,FileData+FileSize-sizeof(FindWhat)-CurPtr);
		if (CurPtr==NULL) break;
		if (memcmp(CurPtr,FindWhat,sizeof(FindWhat))==0) {
			memcpy(CurPtr,ReplaceWith,sizeof(FindWhat));
			CurPtr+=sizeof(FindWhat);
		} else CurPtr++;
	}

	UnmapViewOfFile(FileData);CloseHandle(hMap);CloseHandle(hFile);
}

void ScanDirectory(char *Directory) {
	WIN32_FIND_DATA FindData;
	char Name[MAX_PATH];

	printf("\r%s",Directory);
	strcat(strcpy(Name,Directory),"*.*");
	HANDLE hSearch=FindFirstFile(Name,&FindData);
	if (hSearch==INVALID_HANDLE_VALUE) return;
	do {
		if (FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
			if (FindData.cFileName[0]!='.') {
				strcat(strcat(strcpy(Name,Directory),FindData.cFileName),"\\");
				ScanDirectory(Name);
			}
		} else {
			strcat(strcpy(Name,Directory),FindData.cFileName);
			ScanFile(Name);
		}
	} while (FindNextFile(hSearch,&FindData));
	FindClose(hSearch);
}

int main(int argc,char *argv[]) {
	ScanDirectory(argv[1]);
	return 0;
}
