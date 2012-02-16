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

#ifdef FAR3

const wchar_t *CPluginStartupInfo::GetMsg(int MsgId)
{
	return __super::GetMsg(&m_GUID, MsgId);
}

int CPluginStartupInfo::Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	int ItemsNumber, int ButtonsNumber)
{
	return __super::Message(&m_GUID, NULL, Flags, HelpTopic, Items, ItemsNumber, ButtonsNumber);
}

int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		 const TCHAR *HelpTopic, const FarKey *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	return __super::Menu(&m_GUID, NULL, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, BreakKeys, BreakCode, Item, ItemsNumber);
}

INT_PTR CPluginStartupInfo::PanelControl(HANDLE hPanel, DWORD Command, int Param1, LONG_PTR Param2)
{
	return __super::PanelControl(hPanel, (FILE_CONTROL_COMMANDS)Command, Param1, (void *)Param2);
}

INT_PTR CPluginStartupInfo::SendDlgMessage(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
	return __super::SendDlgMessage(hDlg, Msg, Param1, (void *)Param2);
}

//////////////////////////////////////////////////////////////////////////

vector<FarKey> ConvertKeys(const int *piBreakKeys)
{
	vector<FarKey> arrKeys;
	do {
		FarKey Key;
		Key.VirtualKeyCode = *piBreakKeys;
		Key.ControlKeyState = 0;
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

	vector<FarKey> arrKeys = ConvertKeys(piBreakKeys);

	return StartupInfo.Menu(-1, -1, 0, uiFlags, Title, Bottom,
		HelpTopic, &arrKeys[0], nBreakCode, &arrItems[0], arrItems.size());
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

	vector<FarKey> arrKeys = ConvertKeys(piBreakKeys);

	return StartupInfo.Menu(-1, -1, 0, uiFlags, Title, Bottom,
		HelpTopic, &arrKeys[0], nBreakCode, &arrItems[0], arrItems.size());
}

#endif

#ifndef FAR_NO_NAMESPACE
};
#endif
