#include <stdarg.h>
#include <assert.h>
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

#ifdef FAR3

void CPluginStartupInfo::operator =(const PluginStartupInfo &Info)
{
	(PluginStartupInfo &)(*this) = Info;

	wcscpy(RootKey, L"SOFTWARE\\Far3\\Plugins");
}

const wchar_t *CPluginStartupInfo::GetMsg(int MsgId)
{
	return __super::GetMsg(&m_GUID, MsgId);
}

int CPluginStartupInfo::Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	int ItemsNumber, int ButtonsNumber)
{
	return __super::Message(&m_GUID, NULL, Flags, HelpTopic, Items, ItemsNumber, ButtonsNumber);
}
/*
int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		 const TCHAR *HelpTopic, const FarKey *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	return __super::Menu(&m_GUID, NULL, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, BreakKeys, BreakCode, Item, ItemsNumber);
}*/

int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		 const TCHAR *HelpTopic, const int *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	vector<FarKey> arrKeys = ConvertKeys(BreakKeys);

	return __super::Menu(&m_GUID, NULL, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, &arrKeys[0], BreakCode, Item, ItemsNumber);
}

INT_PTR CPluginStartupInfo::Control(HANDLE hPanel, DWORD Command, int Param1, LONG_PTR Param2)
{
	FILE_CONTROL_COMMANDS fCommand = (FILE_CONTROL_COMMANDS)Command;

	switch (Command)
	{
/*	
	case FCTL_GETCMDLINE:
	case FCTL_SETCMDLINE:
	case FCTL_SETSELECTION:
	case FCTL_INSERTCMDLINE:
	case FCTL_SETUSERSCREEN:
	case FCTL_SETPANELDIR:
	case FCTL_SETCMDLINEPOS:
	case FCTL_GETCMDLINEPOS:
	case FCTL_GETCMDLINESELECTEDTEXT:
	case FCTL_SETCMDLINESELECTION:
	case FCTL_GETCMDLINESELECTION:
	case FCTL_SETNUMERICSORT:
	case FCTL_GETUSERSCREEN:
	case FCTL_GETPANELITEM:
	case FCTL_GETSELECTEDPANELITEM:
	case FCTL_GETCURRENTPANELITEM:
	case FCTL_GETCOLUMNTYPES:
	case FCTL_GETCOLUMNWIDTHS:
	case FCTL_BEGINSELECTION:
	case FCTL_ENDSELECTION:
	case FCTL_CLEARSELECTION:
	case FCTL_SETDIRECTORIESFIRST:
	case FCTL_GETPANELFORMAT:
	case FCTL_GETPANELHOSTFILE:*/

	case FCTL_GETPANELDIRECTORY:{
		BYTE szBuffer[MAX_PATH*7];
		FarPanelDirectory *pDirectory = (FarPanelDirectory *)szBuffer;
		pDirectory->StructSize = sizeof(*pDirectory);
		LONG_PTR nRes = __super::PanelControl(hPanel, fCommand, MAX_PATH*7, szBuffer);
		wcscpy((LPWSTR)Param2, pDirectory->Name);
		return nRes;
								}

	case FCTL_CLOSEPLUGIN:
	case FCTL_GETPANELINFO:
	case FCTL_UPDATEPANEL:
	case FCTL_REDRAWPANEL:
	case FCTL_SETSELECTION:
		return __super::PanelControl(hPanel, fCommand, Param1, (void *)Param2);

	case FCTL_GETPANELITEM:
	case FCTL_GETSELECTEDPANELITEM:{
		FarGetPluginPanelItem GetItem;
		GetItem.Size = __super::PanelControl(hPanel, fCommand, Param1, NULL);
		GetItem.Item = (PluginPanelItem *)Param2;
		return __super::PanelControl(hPanel, fCommand, Param1, (void *)&GetItem);
								   }

	case FCTL_SETVIEWMODE:
	case FCTL_SETSORTMODE:
	case FCTL_SETSORTORDER:
	case FCTL_SETNUMERICSORT:
	case FCTL_SETCASESENSITIVESORT:
		return __super::PanelControl(hPanel, fCommand, Param1, NULL);
	case FCTL_CHECKPANELSEXIST:
	case FCTL_ISACTIVEPANEL:
	case FCTL_BEGINSELECTION:
	case FCTL_ENDSELECTION:
		return __super::PanelControl(hPanel, fCommand, 0, NULL);
	default:
		assert(0);
		return 0;
	}
	return __super::PanelControl(hPanel, (FILE_CONTROL_COMMANDS)Command, Param1, (void *)Param2);
}

INT_PTR CPluginStartupInfo::SendDlgMessage(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
	switch (Msg)
	{
	case DM_LISTGETCURPOS:{
		FarListPos Pos;
		__super::SendDlgMessage(hDlg, Msg, Param1, &Pos);
		return Pos.SelectPos;
						  }

	case DM_LISTSETCURPOS:{
		FarListPos Pos;
		Pos.SelectPos = Param2;
		Pos.TopPos    = Param2;
		return __super::SendDlgMessage(hDlg, Msg, Param1, &Pos);
						  }

	default:
		return __super::SendDlgMessage(hDlg, Msg, Param1, (void *)Param2);
	}
}

LONG_PTR CPluginStartupInfo::DefDlgProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
	return __super::DefDlgProc(hDlg, Msg, Param1, (void *)Param2);
}

int CPluginStartupInfo::EditorControl(int Command, void *Param)
{
	EDITOR_CONTROL_COMMANDS fCommand = (EDITOR_CONTROL_COMMANDS)Command;

	switch (Command)
	{
	default:
		assert(0);
		return __super::EditorControl(-1, fCommand, 0, Param);
	}
}

int CPluginStartupInfo::ViewerControl(int Command, void *Param)
{
	VIEWER_CONTROL_COMMANDS fCommand = (VIEWER_CONTROL_COMMANDS)Command;

	switch (Command)
	{
	default:
		assert(0);
		return __super::ViewerControl(-1, fCommand, 0, Param);
	}
}

//////////////////////////////////////////////////////////////////////////

vector<FarKey> ConvertKeys(const int *piBreakKeys)
{
	vector<FarKey> arrKeys;
	do {
		FarKey Key;
		Key.VirtualKeyCode = (*piBreakKeys) & 0xFFFF;
		Key.ControlKeyState = 0;
		if ((*piBreakKeys) & (PKF_SHIFT  <<16)) Key.ControlKeyState |= SHIFT_PRESSED;
		if ((*piBreakKeys) & (PKF_CONTROL<<16)) Key.ControlKeyState |= LEFT_CTRL_PRESSED;
		if ((*piBreakKeys) & (PKF_ALT    <<16)) Key.ControlKeyState |= LEFT_ALT_PRESSED;
		arrKeys.push_back(Key);
	} while (*(piBreakKeys++));

	return arrKeys;
}

int ChooseMenu(int ItemCount, const TCHAR **ppszItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode)
{
	if (ItemCount == 0) {
		while (ppszItems[ItemCount] != NULL) ItemCount++;
	}

	vector<CFarMenuItemEx> arrItems(ItemCount);
	for (int I=0; I<ItemCount; I++) {
		if (I == iDefault)
			arrItems[I].Flags |= MIF_SELECTED;
		arrItems[I].Text = _wcsdup(ppszItems[I]);
	}

	return StartupInfo.Menu(-1, -1, 0, uiFlags, Title, Bottom,
		HelpTopic, piBreakKeys, nBreakCode, &arrItems[0], arrItems.size());
}

int  ChooseMenu(const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic, int ItemCount, ...)
{
	vector<CFarMenuItemEx> arrItems(ItemCount);

	va_list List;
	va_start(List,ItemCount);
	for (int I=0;I<ItemCount;I++) {
		const TCHAR *Item=va_arg(List,const TCHAR *);
		if (Item) {
			arrItems[I].Text = _wcsdup(Item);
		} else {
			arrItems[I].Flags |= MIF_SEPARATOR;
		}
	}
	va_end(List);

	return StartupInfo.Menu(-1, -1, 0, FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT, Title, Bottom,
		HelpTopic, NULL, NULL, &arrItems[0], arrItems.size());
}

int  ChooseMenu(std::vector<std::tstring> &arrText, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			 int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode)
{
	vector<CFarMenuItemEx> arrItems(arrText.size());

	for (size_t I = 0; I < arrText.size(); I++) {
		if (I == iDefault)
			arrItems[I].Flags |= MIF_SELECTED;
		arrItems[I].Text = _wcsdup(arrText[I].c_str());
	}

	return StartupInfo.Menu(-1, -1, 0, uiFlags, Title, Bottom,
		HelpTopic, piBreakKeys, nBreakCode, &arrItems[0], arrItems.size());
}

#endif

#ifndef FAR_NO_NAMESPACE
};
#endif
