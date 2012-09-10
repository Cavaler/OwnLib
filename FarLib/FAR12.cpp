#include <stdarg.h>
#undef _FAR_NO_NAMELESS_UNIONS
#include <FAR.h>
#include <Pavel.h>

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;

#define strccpy(to, from) strncpy(to, from, sizeof(to))
#define _tcsccpy(to, from) _tcsncpy(to, from, sizeof(to)/sizeof(to[0]))

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

#ifndef FAR3

void CPluginStartupInfo::operator =(const PluginStartupInfo &Info)
{
	(PluginStartupInfo &)(*this) = Info;
}

const TCHAR *CPluginStartupInfo::GetMsg(int MsgId)
{
	return __super::GetMsg(ModuleNumber, MsgId);
}

int CPluginStartupInfo::Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	int ItemsNumber, int ButtonsNumber)
{
	return __super::Message(ModuleNumber, Flags, HelpTopic, Items, ItemsNumber, ButtonsNumber);
}

int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, DWORD Flags, const TCHAR *Title, const TCHAR *Bottom,
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

	int Result=StartupInfo.Menu(-1,-1,0,uiFlags,Title,Bottom,
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

	int Result=StartupInfo.Menu(-1,-1,0,FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,Title,Bottom,
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

	int Result=StartupInfo.Menu(-1,-1,0,uiFlags,Title,Bottom,
		HelpTopic,piBreakKeys,nBreakCode,Items,arrItems.size());

#ifdef _UNICODE
	for (size_t I=0; I<arrItems.size(); I++) if (Items[I].Text) free((void *)Items[I].Text);
#endif
	delete[] Items;
	return Result;
}

//////////////////////////////////////////////////////////////////////////

CFarSettingsKey::CFarSettingsKey()
: m_pHandle(NULL)
{
}

CFarSettingsKey::CFarSettingsKey(const CFarSettingsKey &Key)
{
	*this = Key;
}

void CFarSettingsKey::operator =(const CFarSettingsKey &Key)
{
	m_pHandle = Key.m_pHandle;
	m_pHandle->m_dwRef++;
}

CFarSettingsKey::~CFarSettingsKey()
{
	Close();
}

bool CFarSettingsKey::OpenRoot(LPCTSTR szRootKey)
{
	Close();

	TCHAR szCurrentKey[512];
	_tcscat(_tcscat(_tcscpy(szCurrentKey, StartupInfo.RootKey), _T("\\")), szRootKey);

	HKEY hKey = RegCreateSubkey(HKEY_CURRENT_USER, szCurrentKey);
	if (hKey == NULL) return false;

	m_pHandle = new sHandle(hKey);
	return true;
}

bool CFarSettingsKey::Open(CFarSettingsKey &Key, LPCTSTR szSubKey, bool bCreate)
{
	Close();

	HKEY hKey = (bCreate) ? RegCreateSubkey(Key, szSubKey) : RegOpenSubkey(Key, szSubKey);
	if (hKey == NULL) return false;

	m_pHandle = new sHandle(hKey);
	return true;
}

bool CFarSettingsKey::Valid() const
{
	return (m_pHandle != NULL) && (m_pHandle->m_Key.Valid());
}

CFarSettingsKey CFarSettingsKey::Open(LPCTSTR szSubKey, bool bCreate)
{
	CFarSettingsKey Key;
	Key.Open(*this, szSubKey, bCreate);
	return Key;
}

void CFarSettingsKey::Close()
{
	if (m_pHandle) {
		if (--m_pHandle->m_dwRef == 0) {
			m_pHandle->m_Key.Close();
			delete m_pHandle;
		}
		m_pHandle = NULL;
	}
}

void CFarSettingsKey::SetStringValue(LPCTSTR pszKeyName, LPCTSTR szValue)
{
	SetRegStringValue(m_pHandle->m_Key, pszKeyName, szValue);
}

void CFarSettingsKey::SetIntValue(LPCTSTR pszKeyName, __int64 nValue)
{
	SetRegIntValue(m_pHandle->m_Key, pszKeyName, (int)nValue);
}
/*
void CFarSettingsKey::StartEnumKeys()
{
}

void CFarSettingsKey::StartEnumValues()
{
}

tstring CFarSettingsKey::GetNextEnum()
{
	return L"";
}*/

bool CFarSettingsKey::DeleteKey(LPCTSTR szSubKey)
{
	return RegDeleteKey(m_pHandle->m_Key, szSubKey) == ERROR_SUCCESS;
}

bool CFarSettingsKey::DeleteValue(LPCTSTR szSubKey)
{
	return RegDeleteValue(m_pHandle->m_Key, szSubKey) == ERROR_SUCCESS;
}

void CFarSettingsKey::DeleteAllKeys()
{
	RegDeleteAllSubkeys(m_pHandle->m_Key);
}

void CFarSettingsKey::DeleteAllValues()
{
	RegDeleteAllValues(m_pHandle->m_Key);
}

#endif

#ifndef FAR_NO_NAMESPACE
};
#endif
