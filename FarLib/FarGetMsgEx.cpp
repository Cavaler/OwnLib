#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <EasyReg.h>

#undef _FAR_NO_NAMELESS_UNIONS
#include <FAR.h>

#ifndef _UNICODE

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

enum GetMsgResult {GM_OK,GM_NO_LANGUAGE,GM_NO_MODULE,GM_NO_MESSAGE};

struct SectionInformation {
	void AddLine(char *Line);
	int StartNumber;
	int Length;
	char **Lines;
};

class CModuleInformation {
public:
	CModuleInformation(char *&Text,int TextLen,TCHAR *&Language);
	BOOL HasModule(TCHAR *Module);
	GetMsgResult GetMsg(int MsgId,TCHAR *&Message);
	~CModuleInformation();
private:
	TCHAR *ModuleName;
	int SectionCount;
	SectionInformation *Sections;
};

class CLanguagePack {
public:
	CLanguagePack(TCHAR *Lang);
	~CLanguagePack();
	BOOL HasLanguage(TCHAR *Lang);
	void AddModule(CModuleInformation *Module);
	GetMsgResult GetMsg(int MsgId, TCHAR *Module, char *&Message);
private:
	TCHAR *Language;
	CModuleInformation **Modules;
	int ModuleCount;
};

BOOL Loaded=FALSE;
CLanguagePack **Languages=NULL;
int LanguageCount=0;
char **LoadedFiles=NULL;
int LoadedFileCount=0;

TCHAR *CurrentLanguage=NULL;

void SectionInformation::AddLine(char *Line) {
	Lines=(char **)realloc(Lines,(Length+1)*sizeof(char *));
	Lines[Length++]=Line;
}

CModuleInformation::CModuleInformation(char *&Text,int TextLen,TCHAR *&Language):
ModuleName(NULL),SectionCount(1),Sections((SectionInformation *)malloc(sizeof(SectionInformation))) {
	char *CurLine=Text;
	BOOL GotLanguage=FALSE;
	BOOL GotModule=FALSE;

	Sections[0].StartNumber=0;
	Sections[0].Length=0;
	Sections[0].Lines=NULL;

	while (CurLine-Text<TextLen) {
		int LineLen=TextLen-(CurLine-Text);
		char *LineEnd=(char *)memchr(CurLine,'\n',LineLen);
		if (LineEnd) {
			LineLen=LineEnd-CurLine;
			if ((LineEnd>CurLine)&&(*(LineEnd-1)=='\r')) LineLen--;
		}
		if (LineLen==0) {CurLine=LineEnd+1;continue;}
		CurLine[LineLen]=0;		// For simplicity
		if (*CurLine=='.') {
			char *Equal=strchr(CurLine,'=');
			if (Equal) {
				*Equal=0;Equal++;
				if (!_stricmp(CurLine,".Language")) {
					char *Comma=strchr(Equal,',');
					if (Comma) *Comma=0;
					if (GotLanguage||GotModule) {Text=CurLine;*(Equal-1)='=';Language=Equal;return;}
					Language=Equal;GotLanguage=TRUE;
				}
				if (!_stricmp(CurLine,".Module")) {
					if (GotModule) {Text=CurLine;*(Equal-1)='=';return;}
					ModuleName=Equal;GotModule=TRUE;
				}
				if (!_stricmp(CurLine,".Offset")) {
					Sections=(SectionInformation *)realloc(Sections,++SectionCount*sizeof(SectionInformation));
					sscanf(Equal,"%d",&Sections[SectionCount-1].StartNumber);
					Sections[SectionCount-1].Length=0;
					Sections[SectionCount-1].Lines=NULL;
				}
			}
		} else {
			while ((*CurLine==' ')||(*CurLine=='\t')) {CurLine++;LineLen--;}
			if (*CurLine == '"') {
				CurLine++;LineLen--;
				if (CurLine[LineLen-1]=='"') CurLine[LineLen-1]=0;
			}
			Sections[SectionCount-1].AddLine(CurLine);
		}
		CurLine=LineEnd+1;
	}
	Text=CurLine;
}

BOOL CModuleInformation::HasModule(TCHAR *Module) {
	if (ModuleName==NULL) {
		return Module==NULL;
	}
	if (Module==NULL) return FALSE;
	return (_stricmp(ModuleName,Module)==0);
}

GetMsgResult CModuleInformation::GetMsg(int MsgId,char *&Message) {
	for (int I=0;I<SectionCount;I++) {
		if ((MsgId>=Sections[I].StartNumber)&&(MsgId<Sections[I].StartNumber+Sections[I].Length)) {
			Message=Sections[I].Lines[MsgId-Sections[I].StartNumber];
			return GM_OK;
		}
	}
	return GM_NO_MESSAGE;
}

CModuleInformation::~CModuleInformation() {
//	if (ModuleName) free(ModuleName);
	for (int I=0;I<SectionCount;I++) free(Sections[I].Lines);
	free(Sections);
}

CLanguagePack::CLanguagePack(TCHAR *Lang):Language(_tcsdup(Lang)),Modules(NULL),ModuleCount(0) {
}

CLanguagePack::~CLanguagePack() {
	free(Language);
	if (Modules) {
		for (int I=0;I<ModuleCount;I++) delete Modules[I];
		free(Modules);
	}
}

BOOL CLanguagePack::HasLanguage(TCHAR *Lang) {return _tcsicmp(Language,Lang)==0;}

void CLanguagePack::AddModule(CModuleInformation *Module) {
	Modules=(CModuleInformation **)realloc(Modules,(ModuleCount+1)*sizeof(CModuleInformation *));
	Modules[ModuleCount++]=Module;
}

GetMsgResult CLanguagePack::GetMsg(int MsgId,TCHAR *Module,TCHAR *&Message) {
	for (int I=0;I<ModuleCount;I++) {
		if (Modules[I]->HasModule(Module)) return Modules[I]->GetMsg(MsgId,Message);
	}
	return GM_NO_MODULE;
}

// functions

void LoadLanguageFile(TCHAR *FileName) {
	HANDLE hFile=CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile==INVALID_HANDLE_VALUE) return;
	DWORD Size=GetFileSize(hFile,NULL);
	char *Text=(char *)malloc(Size);
	if (Text) {
		DWORD Read;
		if (ReadFile(hFile,Text,Size,&Read,NULL)) {
			LoadedFiles=(char **)realloc(LoadedFiles,(LoadedFileCount+1)*sizeof(char *));
			LoadedFiles[LoadedFileCount++]=Text;
			char *ModuleText=Text;
			char *Language=NULL;
			while (ModuleText<Text+Read) {
				CModuleInformation *Module=new CModuleInformation(ModuleText,Read-(ModuleText-Text),Language);
				int I;
				for (I=0;I<LanguageCount;I++) {
					if (Languages[I]->HasLanguage(Language)) break;
				}
				if (I==LanguageCount) {
					Languages=(CLanguagePack **)realloc(Languages,(LanguageCount+1)*sizeof(CLanguagePack *));
					Languages[LanguageCount++]=new CLanguagePack(Language);
				}
				Languages[I]->AddModule(Module);
			}
		}
	}
	CloseHandle(hFile);
}

void InitLanguageFiles() {
	TCHAR Current[MAX_PATH];
	HKEY Key;

	if (Loaded) FreeLanguageFiles();
	_tcscpy(Current, StartupInfo.RootKey);
	_tcscpy(_tcsrchr(Current, '\\')+1, _T("Language"));
	RegCreateKeyEx(HKEY_CURRENT_USER,Current,0,NULL,0,KEY_ALL_ACCESS,NULL,&Key,NULL);
	AllocAndQueryRegStringValue(Key,_T("Main"),&CurrentLanguage,NULL,_T("English"));
	RegCloseKey(Key);
	_tcscpy(Current, StartupInfo.ModuleName);
	_tcscpy(_tcsrchr(Current,'\\')+1, _T("*.ln?"));

	WIN32_FIND_DATA FindData;
	HANDLE Srch=FindFirstFile(Current,&FindData);
	while (Srch!=INVALID_HANDLE_VALUE) {
		_tcscpy(_tcsrchr(Current,'\\')+1, FindData.cFileName);
		LoadLanguageFile(Current);
		if (FindNextFile(Srch,&FindData)==FALSE) break;
	}
	FindClose(Srch);Loaded=TRUE;
}

void FreeLanguageFiles() {
	if (Languages) {
		for (int I=0;I<LanguageCount;I++) delete Languages[I];
		free(Languages);Languages=NULL;
	}
	LanguageCount=0;
	if (LoadedFiles) {
		for (int I=0;I<LoadedFileCount;I++) free(LoadedFiles[I]);
		free(LoadedFiles);LoadedFiles=NULL;
	}
	LoadedFileCount=0;
	if (CurrentLanguage) {free(CurrentLanguage);CurrentLanguage=NULL;}
	Loaded=FALSE;
}

const char *GetMsgEx(int MsgId,const char *Module) {
	GetMsgResult Result;
	char *Message;

	if (!Loaded) InitLanguageFiles();
	int I;
	for (I=0;I<LanguageCount;I++) {
		if (Languages[I]->HasLanguage(CurrentLanguage)) {
			Result=Languages[I]->GetMsg(MsgId,(char *)Module,Message);
			if (Result==GM_OK) return Message;
			break;
		}
	}
	for (I=0;I<LanguageCount;I++) {
		if (Languages[I]->HasLanguage("English")) {
			Result=Languages[I]->GetMsg(MsgId,(char *)Module,Message);
			if (Result==GM_OK) return Message;
			break;
		}
	}
	if (LanguageCount) {
		Result=Languages[0]->GetMsg(MsgId,(char *)Module,Message);
		if (Result==GM_OK) return Message;
	}
	return NULL;
}

#ifndef FAR_NO_NAMESPACE
};
#endif

#endif	// no unicode for now
