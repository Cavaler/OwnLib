#include <stdarg.h>
#undef _FAR_NO_NAMELESS_UNIONS
#include <FAR.h>

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

PluginStartupInfo StartupInfo;
const char *g_pszErrorTitle;
const char *g_pszErrorTopic;
const char *g_pszLastErrorTopic;
const char *g_pszOKButton;
HANDLE g_hSaveScreen;

const char *GetMsg(int MsgId) {return const_cast<char *>(StartupInfo.GetMsg(StartupInfo.ModuleNumber,MsgId));}
#define strccpy(to,from) strncpy(to,from,sizeof(to))

void InitDialogItems(struct InitDialogItem *Init,struct FarDialogItem *Item,int ItemsNumber) {
	for (int I=0;I<ItemsNumber;I++) {
		Item[I].Type=Init[I].Type;
		Item[I].X1=Init[I].X1;
		Item[I].Y1=Init[I].Y1;
		Item[I].X2=Init[I].X2;
		Item[I].Y2=Init[I].Y2;
		Item[I].Focus=Init[I].Focus;
		Item[I].Selected=Init[I].Selected;
		Item[I].Flags=Init[I].Flags;
		Item[I].DefaultButton=Init[I].DefaultButton;
		if (Init[I].Data==NULL) 
			Item[I].Data[0]=0;
		else if ((int)Init[I].Data<2000)
			strccpy(Item[I].Data,GetMsg((int)Init[I].Data));
		else
			strccpy(Item[I].Data,Init[I].Data);
	}
}

int WhichRadioButton(struct FarDialogItem *Item,int ItemsNumber) {
	for (int I=0;I<ItemsNumber;I++) if (Item[I].Selected) return I;
	return -1;
}

int ChooseMenu(int ItemCount, const char **ppszItems, const char *Title, const char *Bottom, const char *HelpTopic,
			int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode) {
	FarMenuItem *Items=new FarMenuItem[ItemCount];
	for (int I=0;I<ItemCount;I++) {
		Items[I].Checked=Items[I].Selected=FALSE;
		if (I == iDefault)
			Items[I].Selected = TRUE;
		strccpy(Items[I].Text,ppszItems[I]);
		Items[I].Separator=FALSE;
	}

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,uiFlags,Title,Bottom,
		HelpTopic,piBreakKeys,nBreakCode,Items,ItemCount);

	delete[] Items;
	return Result;
}

int  ChooseMenu(const char *Title, const char *Bottom, const char *HelpTopic, int ItemCount, ...) {
	FarMenuItem *Items=new FarMenuItem[ItemCount];
	va_list List;
	va_start(List,ItemCount);
	for (int I=0;I<ItemCount;I++) {
		const char *Item=va_arg(List,const char *);
		Items[I].Checked=Items[I].Selected=FALSE;
		if (Item) {
			strccpy(Items[I].Text,Item);
			Items[I].Separator=FALSE;
		} else {
			Items[I].Text[0]=0;
			Items[I].Separator=TRUE;
		}
	}
	va_end(List);

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,Title,Bottom,
		HelpTopic,NULL,NULL,Items,ItemCount);

	delete[] Items;
	return Result;
}

int  ChooseMenu(std::vector<std::string> &arrItems, const char *Title, const char *Bottom, const char *HelpTopic,
			 int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode) {
	FarMenuItem *Items=new FarMenuItem[arrItems.size()];
	for (int I = 0; I < (int)arrItems.size(); I++) {
		Items[I].Checked=Items[I].Selected=FALSE;
		if (I == iDefault)
			Items[I].Selected = TRUE;
		strccpy(Items[I].Text,arrItems[I].c_str());
		Items[I].Separator=FALSE;
	}

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,uiFlags,Title,Bottom,
		HelpTopic,piBreakKeys,nBreakCode,Items,arrItems.size());

	delete[] Items;
	return Result;
}

int  Message(UINT uiFlags, const char *szHelpTopic, int iItemsNumber, int iButtonsNumber, ...) {
	const char **ppszItems = new const char *[iItemsNumber];
	va_list List;
	va_start(List,iButtonsNumber);
	for (int I=0; I<iItemsNumber; I++)
		ppszItems[I]=va_arg(List, const char *);
	va_end(List);

	int Result = StartupInfo.Message(StartupInfo.ModuleNumber, uiFlags, szHelpTopic, ppszItems, iItemsNumber, iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

int  Message(UINT uiFlags, const char *szHelpTopic, int iButtonsNumber, std::vector<std::string> &arrItems) {
	const char **ppszItems = new const char *[arrItems.size()];
	for (int I=0; I < (int)arrItems.size(); I++)
		ppszItems[I]=arrItems[I].c_str();

	int Result = StartupInfo.Message(StartupInfo.ModuleNumber, uiFlags, szHelpTopic, ppszItems, arrItems.size(), iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

void ShowMessage(const char *pszTitle, const char *pszMessage1, const char *pszMessage2) {
	const char *ppszItems[] = {pszTitle, pszMessage1, pszMessage2, GetMsg(0)};
	if (pszMessage2) {
		StartupInfo.Message(StartupInfo.ModuleNumber, 0, "", ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, 0, "", ppszItems, 3, 1);
	}
}

void ShowError(const char *pszMessage1, const char *pszMessage2) {
	const char *ppszItems[] = {g_pszErrorTitle, pszMessage1, pszMessage2, GetMsg(0)};
	if (pszMessage2) {
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING, g_pszErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING, g_pszErrorTopic, ppszItems, 3, 1);
	}
}

void ShowLastError(const char *pszMessage, const char *pszFileName) {
	const char *ppszItems[] = {g_pszErrorTitle, pszMessage, pszFileName, GetMsg(0)};
	if (pszFileName) {
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 3, 1);
	}
}

void ShowWaitMessage(const char *pszTitle, const char *pszMessage1, const char *pszMessage2) {
	g_hSaveScreen = StartupInfo.SaveScreen(0, 0, -1, -1);
	const char *ppszItems[] = {pszTitle, pszMessage1, pszMessage2};
	StartupInfo.Message(StartupInfo.ModuleNumber, 0, "", ppszItems, pszMessage2?3:2, 0);
}

void HideWaitMessage() {
	StartupInfo.RestoreScreen(g_hSaveScreen);
}

#ifndef FAR_NO_NAMESPACE
};
#endif
