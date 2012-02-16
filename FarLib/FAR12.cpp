#include <stdarg.h>
#undef _FAR_NO_NAMELESS_UNIONS
#include <FAR.h>
#include <Pavel.h>

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

#ifndef FAR3

const wchar_t *CPluginStartupInfo::GetMsg(int MsgId)
{
	return __super::GetMsg(ModuleNumber, MsgId);
}

int CPluginStartupInfo::Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	int ItemsNumber, int ButtonsNumber)
{
	return __super::Message(ModuleNumber, Flags, HelpTopic, Items, ItemsNumber, ButtonsNumber);
}

int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
							 const TCHAR *HelpTopic, const int *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	return __super::Menu(ModuleNumber, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, BreakKeys, BreakCode, Item, ItemsNumber);
}

#pragma region("CFarMenuItem")

void CFarMenuItem::SetText(const TCHAR *szTitle) {
#ifdef UNICODE
	Text = _tcsdup(szTitle);
#else
	_tcscpy(Text, szTitle);
#endif
}

CFarMenuItem::CFarMenuItem() {
	Selected = Checked = Separator = 0;
	SetText(_T(""));
}

CFarMenuItem::CFarMenuItem(const TCHAR *szTitle) {
	Selected = Checked = Separator = 0;
	SetText(szTitle);
}

CFarMenuItem::CFarMenuItem(const tstring &strTitle) {
	Selected = Checked = Separator = 0;
	SetText(strTitle.c_str());
}

CFarMenuItem::CFarMenuItem(int nMsgID) {
	Selected = Checked = Separator = 0;
	SetText(GetMsg(nMsgID));
}

CFarMenuItem::CFarMenuItem(bool bSeparator) {
	Selected = Checked = 0;
	Separator = bSeparator ? 1 : 0;
	SetText(_T(""));
}

CFarMenuItem::CFarMenuItem(const CFarMenuItem &Item)
{
	*this = (const FarMenuItem &)Item;
}

CFarMenuItem::CFarMenuItem(const FarMenuItem &Item)
{
	*this = Item;
}

void CFarMenuItem::operator=(const FarMenuItem &Item)
{
	Selected = Item.Selected;
	Checked = Item.Checked;
	Separator = Item.Separator;
#ifdef UNICODE
	Text = _tcsdup(Item.Text);
#else
	_tcscpy(Text, Item.Text);
#endif
}

void CFarMenuItem::operator=(const CFarMenuItem &Item) {
	*this = (const FarMenuItem &)Item;
}

CFarMenuItem::~CFarMenuItem() {
#ifdef UNICODE
	if (Text) free((TCHAR *)Text);
#endif
}

#pragma endregion

void UpgradeMenuItemVector(const vector<CFarMenuItem> &arrSrc, vector<CFarMenuItemEx> &arrDst) {
	for (size_t nItem = 0; nItem < arrSrc.size(); nItem++) {
		arrDst.push_back(arrSrc[nItem]);
	}
}

int ChooseMenu(int ItemCount, const TCHAR **ppszItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode)
{
	if (ItemCount == 0) {
		while (ppszItems[ItemCount] != NULL) ItemCount++;
	}

	FarMenuItem *Items = new FarMenuItem[ItemCount];
	for (int I=0;I<ItemCount;I++) {
		Items[I].Checked=Items[I].Selected=FALSE;
		if (I == iDefault)
			Items[I].Selected = TRUE;
#ifdef _UNICODE
		Items[I].Text = _tcsdup(ppszItems[I]);
#else
		strccpy(Items[I].Text,ppszItems[I]);
#endif
		Items[I].Separator=FALSE;
	}

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,uiFlags,Title,Bottom,
		HelpTopic,piBreakKeys,nBreakCode,Items,ItemCount);

#ifdef _UNICODE
	for (int I=0;I<ItemCount;I++) if (Items[I].Text) free((void *)Items[I].Text);
#endif
	delete[] Items;

	return Result;
}

int  ChooseMenu(const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic, int ItemCount, ...) {
	FarMenuItem *Items=new FarMenuItem[ItemCount];
	va_list List;
	va_start(List,ItemCount);
	for (int I=0;I<ItemCount;I++) {
		const TCHAR *Item=va_arg(List,const TCHAR *);
		Items[I].Checked=Items[I].Selected=FALSE;
		if (Item) {
#ifdef _UNICODE
			Items[I].Text = _tcsdup(Item);
#else
			strccpy(Items[I].Text,Item);
#endif
			Items[I].Separator=FALSE;
		} else {
#ifdef _UNICODE
			Items[I].Text=NULL;
#else
			Items[I].Text[0]=0;
#endif
			Items[I].Separator=TRUE;
		}
	}
	va_end(List);

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,Title,Bottom,
		HelpTopic,NULL,NULL,Items,ItemCount);

#ifdef _UNICODE
	for (int I=0;I<ItemCount;I++) if (Items[I].Text) free((void *)Items[I].Text);
#endif
	delete[] Items;

	return Result;
}

int  ChooseMenu(std::vector<std::tstring> &arrItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			 int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode) {
	FarMenuItem *Items=new FarMenuItem[arrItems.size()];
	for (size_t I = 0; I < arrItems.size(); I++) {
		Items[I].Checked=Items[I].Selected=FALSE;
		if (I == iDefault)
			Items[I].Selected = TRUE;
#ifdef _UNICODE
		Items[I].Text = _tcsdup(arrItems[I].c_str());
#else
		_tcsccpy(Items[I].Text,arrItems[I].c_str());
#endif
		Items[I].Separator=FALSE;
	}

	int Result=StartupInfo.Menu(StartupInfo.ModuleNumber,-1,-1,0,uiFlags,Title,Bottom,
		HelpTopic,piBreakKeys,nBreakCode,Items,arrItems.size());

#ifdef _UNICODE
	for (size_t I=0; I<arrItems.size(); I++) if (Items[I].Text) free((void *)Items[I].Text);
#endif
	delete[] Items;
	return Result;
}

#endif

#ifndef FAR_NO_NAMESPACE
};
#endif
