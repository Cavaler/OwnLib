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

DWORD FarKeyState(DWORD dwWinKeyState)
{
	return 
		((dwWinKeyState & (LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED)) ? PKF_ALT : 0) |
		((dwWinKeyState & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)) ? PKF_CONTROL : 0) |
		((dwWinKeyState & SHIFT_PRESSED) ? PKF_SHIFT : 0);
}

void CPluginStartupInfo::operator =(const PluginStartupInfo &Info)
{
	(PluginStartupInfo &)(*this) = Info;

	wcscpy(RootKey, L"SOFTWARE\\Far3\\Plugins");
}

const wchar_t *CPluginStartupInfo::GetMsg(int MsgId)
{
	return __super::GetMsg(&m_GUID, MsgId);
}

intptr_t CPluginStartupInfo::Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	intptr_t ItemsNumber, intptr_t ButtonsNumber)
{
	return __super::Message(&m_GUID, NULL, Flags, HelpTopic, Items, ItemsNumber, ButtonsNumber);
}
/*
int CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		 const TCHAR *HelpTopic, const FarKey *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	return __super::Menu(&m_GUID, NULL, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, BreakKeys, BreakCode, Item, ItemsNumber);
}*/

intptr_t CPluginStartupInfo::Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		 const TCHAR *HelpTopic, const int *piBreakKeys, int *pBreakCode, const FarMenuItem *Item, size_t ItemsNumber)
{
	vector<FarKey> arrKeys = ConvertKeys(piBreakKeys);

	intptr_t nCode;
	intptr_t nResult = __super::Menu(&m_GUID, NULL, X, Y, MaxHeight, Flags, Title, Bottom, HelpTopic, &arrKeys[0], &nCode, Item, ItemsNumber);
	if (pBreakCode) *pBreakCode = nCode;
	return nResult;
}

intptr_t CPluginStartupInfo::Control(HANDLE hPanel, DWORD Command, int Param1, LONG_PTR Param2)
{
	FILE_CONTROL_COMMANDS fCommand = (FILE_CONTROL_COMMANDS)Command;

	switch (Command)
	{
	case FCTL_GETPANELDIRECTORY:{
		LONG_PTR nSize = __super::PanelControl(hPanel, fCommand, 0, NULL);
		vector<BYTE> arrBuffer(nSize);
		FarPanelDirectory *pDirectory = (FarPanelDirectory *)&arrBuffer[0];
		pDirectory->StructSize = sizeof(*pDirectory);
		LONG_PTR nRes = __super::PanelControl(hPanel, fCommand, nSize, &arrBuffer[0]);
		if (Param2 != NULL)
			wcsncpy_s((LPWSTR)Param2, Param1, pDirectory->Name, _TRUNCATE);
		return wcslen(pDirectory->Name)+1;
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
		GetItem.StructSize = sizeof(FarGetPluginPanelItem);
		GetItem.Size = __super::PanelControl(hPanel, fCommand, Param1, NULL);
		GetItem.Item = (PluginPanelItem *)Param2;
		return __super::PanelControl(hPanel, fCommand, Param1, (void *)&GetItem);
								   }

	case FCTL_SETVIEWMODE:
	case FCTL_SETSORTMODE:
	case FCTL_SETSORTORDER:
		return __super::PanelControl(hPanel, fCommand, Param1, NULL);
	case FCTL_CHECKPANELSEXIST:
	case FCTL_ISACTIVEPANEL:
	case FCTL_BEGINSELECTION:
	case FCTL_ENDSELECTION:
		return __super::PanelControl(hPanel, fCommand, 0, NULL);
	default:
		//assert(0);
		//return 0;
		break;
	}

	return __super::PanelControl(hPanel, (FILE_CONTROL_COMMANDS)Command, Param1, (void *)Param2);
}

intptr_t CPluginStartupInfo::SendDlgMessage(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
	switch (Msg)
	{
	case DM_LISTGETCURPOS:{
		FarListPos Pos;
		Pos.StructSize = sizeof(FarListPos);
		__super::SendDlgMessage(hDlg, Msg, Param1, &Pos);
		return Pos.SelectPos;
						  }

	case DM_LISTSETCURPOS:{
		FarListPos Pos;
		Pos.StructSize = sizeof(FarListPos);
		Pos.SelectPos = Param2;
		Pos.TopPos    = Param2;
		return __super::SendDlgMessage(hDlg, Msg, Param1, &Pos);
						  }

	default:
		return __super::SendDlgMessage(hDlg, Msg, Param1, (void *)Param2);
	}
}

intptr_t CPluginStartupInfo::DefDlgProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2)
{
	return __super::DefDlgProc(hDlg, Msg, Param1, (void *)Param2);
}

intptr_t CPluginStartupInfo::EditorControl(int Command, void *Param)
{
	EDITOR_CONTROL_COMMANDS fCommand = (EDITOR_CONTROL_COMMANDS)Command;

	switch (Command)
	{
	case ECTL_GETINFO:
	case ECTL_GETFILENAME:
	case ECTL_GETSTRING:
	case ECTL_SETSTRING:
	case ECTL_SETPOSITION:
	case ECTL_SELECT:
	case ECTL_REALTOTAB:
	case ECTL_REDRAW:
	case ECTL_UNDOREDO:
	case ECTL_INSERTSTRING:
	case ECTL_INSERTTEXT:
	case ECTL_DELETESTRING:
	case ECTL_DELETEBLOCK:
	case ECTL_PROCESSINPUT:
		return __super::EditorControl(-1, fCommand, 0, Param);
	default:
		assert(0);
		return __super::EditorControl(-1, fCommand, 0, Param);
	}
}

intptr_t CPluginStartupInfo::ViewerControl(int Command, void *Param)
{
	VIEWER_CONTROL_COMMANDS fCommand = (VIEWER_CONTROL_COMMANDS)Command;

	switch (Command)
	{
	case VCTL_GETFILENAME:
		return __super::ViewerControl(-1, fCommand, MAX_PATH, Param);
	case VCTL_QUIT:
	case VCTL_REDRAW:
		return __super::ViewerControl(-1, fCommand, 0, 0);
	case VCTL_GETINFO:
	case VCTL_SETPOSITION:
	case VCTL_SELECT:
		return __super::ViewerControl(-1, fCommand, 0, Param);
	default:
		assert(0);
		return __super::ViewerControl(-1, fCommand, 0, Param);
	}
}

intptr_t CPluginStartupInfo::MacroControl(int Command, intptr_t Param1, void* Param2)
{
	return __super::MacroControl(&m_GUID, (FAR_MACRO_CONTROL_COMMANDS)Command, Param1, Param2);
}

//////////////////////////////////////////////////////////////////////////

WIN32_FIND_DATA PanelToWFD(const PluginPanelItem &Item)
{
	WIN32_FIND_DATA fd;

	fd.dwFileAttributes = Item.FileAttributes;
	fd.ftCreationTime   = Item.CreationTime;
	fd.ftLastAccessTime = Item.LastAccessTime;
	fd.ftLastWriteTime  = Item.LastWriteTime;
	fd.nFileSizeHigh    = (DWORD)(Item.FileSize >> 32);
	fd.nFileSizeLow     = Item.FileSize && 0xFFFFFFFF;
	wcsncpy_s(fd.cFileName, MAX_PATH, Item.FileName, _TRUNCATE);
	wcsncpy_s(fd.cAlternateFileName, 14, Item.AlternateFileName, _TRUNCATE);

	return fd;
}

FIND_DATA PanelToFD(const PluginPanelItem &Item)
{
	FIND_DATA fd;

	fd.dwFileAttributes = Item.FileAttributes;
	fd.ftCreationTime   = Item.CreationTime;
	fd.ftLastAccessTime = Item.LastAccessTime;
	fd.ftLastWriteTime  = Item.LastWriteTime;
	fd.nFileSize        = Item.FileSize;
	fd.strFileName      = Item.FileName;

	return fd;
}

vector<FarKey> ConvertKeys(const int *piBreakKeys)
{
	vector<FarKey> arrKeys;
	if (piBreakKeys == NULL) {
		FarKey Key;
		Key.VirtualKeyCode = 0;
		Key.ControlKeyState = 0;
		arrKeys.push_back(Key);
		return arrKeys;
	}

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

	return ReturnMenu(StartupInfo.Menu(-1, -1, 0, uiFlags&FMENU_NORETURNCODE, Title, Bottom,
		HelpTopic, piBreakKeys, nBreakCode, &arrItems[0], arrItems.size()), uiFlags, piBreakKeys, nBreakCode);
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

	return ReturnMenu(StartupInfo.Menu(-1, -1, 0, uiFlags&FMENU_NORETURNCODE, Title, Bottom,
		HelpTopic, piBreakKeys, nBreakCode, &arrItems[0], arrItems.size()), uiFlags, piBreakKeys, nBreakCode);
}

int  ChooseMenu(unsigned int uiFlags, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
				const int *piBreakKeys, int *nBreakCode, const FarMenuItem *pItems, int nItemCount)
{
	return ReturnMenu(StartupInfo.Menu(-1, -1, 0, uiFlags&FMENU_NORETURNCODE, Title, Bottom,
		HelpTopic, piBreakKeys, nBreakCode, pItems, nItemCount), uiFlags, piBreakKeys, nBreakCode);
}

//////////////////////////////////////////////////////////////////////////

CFarSettingsKey::CFarSettingsKey()
: m_Key(0)
, m_pHandle(NULL)
{
}

CFarSettingsKey::CFarSettingsKey(LPCTSTR szRootKey)
: m_pHandle(NULL)
{
	OpenRoot(szRootKey);
}

CFarSettingsKey::CFarSettingsKey(const CFarSettingsKey &Key)
{
	*this = Key;
}

void CFarSettingsKey::operator =(const CFarSettingsKey &Key)
{
	m_Key = Key.m_Key;
	m_pHandle = Key.m_pHandle;
	if (m_pHandle) m_pHandle->m_dwRef++;
}

CFarSettingsKey::~CFarSettingsKey()
{
	Close();
}

bool CFarSettingsKey::OpenRoot(LPCTSTR szRootKey)
{
	Close();

	FarSettingsCreate Fsc;
	Fsc.StructSize = sizeof(FarSettingsCreate);
	Fsc.Guid = StartupInfo.m_GUID;

	if (!StartupInfo.SettingsControl(INVALID_HANDLE_VALUE, SCTL_CREATE, PSL_ROAMING, &Fsc)) return false;

	m_pHandle = new sHandle(Fsc.Handle);
	m_Key = 0;

	return true;
}

bool CFarSettingsKey::Open(CFarSettingsKey &Key, LPCTSTR szSubKey, bool bCreate)
{
	Close();

	FarSettingsValue Fsv;
	Fsv.StructSize = sizeof(FarSettingsValue);
	Fsv.Root  = Key.m_Key;
	Fsv.Value = szSubKey;

	m_Key = StartupInfo.SettingsControl(Key.m_pHandle->m_Handle, bCreate ? SCTL_CREATESUBKEY : SCTL_OPENSUBKEY, 0, &Fsv);
	if (m_Key == 0) return false;

	m_pHandle = Key.m_pHandle;
	m_pHandle->m_dwRef++;

	return true;
}

bool CFarSettingsKey::Valid() const
{
	return m_pHandle != NULL;
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
			StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_FREE, 0, 0);
			delete m_pHandle;
		}
		m_pHandle = NULL;
	}
}

bool CFarSettingsKey::QueryStringValue(LPCTSTR pszKeyName, tstring &strValue)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_STRING;
	if (!StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_GET, 0, &Fsi)) return false;

	strValue = Fsi.String;
	return true;
}

bool CFarSettingsKey::QueryInt64Value(LPCTSTR pszKeyName, __int64 &nValue)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_QWORD;
	if (!StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_GET, 0, &Fsi)) return false;

	nValue = Fsi.Number;
	return true;
}

bool CFarSettingsKey::QueryBoolValue(LPCTSTR pszKeyName, bool &bValue)
{
	__int64 nValue64;
	if (!QueryInt64Value(pszKeyName, nValue64)) return false;
	bValue = (nValue64 != 0);
	return true;
}

void CFarSettingsKey::SetStringValue(LPCTSTR pszKeyName, LPCTSTR szValue)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_STRING;
	Fsi.String = szValue;
	StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_SET, 0, &Fsi);
}

void CFarSettingsKey::SetIntValue(LPCTSTR pszKeyName, __int64 nValue)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_QWORD;
	Fsi.Number = nValue;
	StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_SET, 0, &Fsi);
}

void CFarSettingsKey::StartEnumKeys()
{
	m_Enum.StructSize = sizeof(FarSettingsEnum);
	m_Enum.Root = m_Key;
	StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_ENUM, 0, &m_Enum);
	m_nEnum = 0;
	m_bKeys = true;
}

void CFarSettingsKey::StartEnumValues()
{
	m_Enum.StructSize = sizeof(FarSettingsEnum);
	m_Enum.Root = m_Key;
	StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_ENUM, 0, &m_Enum);
	m_nEnum = 0;
	m_bKeys = false;
}

bool CFarSettingsKey::GetNextEnum(tstring &strName)
{
	do {
		if (m_nEnum >= m_Enum.Count) return false;
		if (m_bKeys == (m_Enum.Items[m_nEnum].Type == FST_SUBKEY)) {
			strName = m_Enum.Items[m_nEnum].Name;
			m_nEnum++;
			return true;
		}
		m_nEnum++;
	} while (true);
}

bool CFarSettingsKey::DeleteKey(LPCTSTR szSubKey)
{
	CFarSettingsKey hKey = Open(szSubKey, false);
	if (!hKey.Valid()) return false;
	return hKey.DeleteValue(NULL);
}

bool CFarSettingsKey::DeleteValue(LPCTSTR szSubKey)
{
	FarSettingsValue Fsv;
	Fsv.StructSize = sizeof(FarSettingsValue);
	Fsv.Root = m_Key;
	Fsv.Value = szSubKey;
	return StartupInfo.SettingsControl(m_pHandle->m_Handle, SCTL_DELETE, 0, &Fsv) != 0;
}

void CFarSettingsKey::DeleteAllKeys()
{
	do {
		StartEnumKeys();
		tstring strName;
		if (!GetNextEnum(strName)) return;
		DeleteKey(strName.c_str());
	} while (true);
}

void CFarSettingsKey::DeleteAllValues()
{
	do {
		StartEnumValues();
		tstring strName;
		if (!GetNextEnum(strName)) return;
		DeleteValue(strName.c_str());
	} while (true);
}

void QuerySettingsStringValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, std::tstring &strBuffer, const TCHAR *pszDefault)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = Key.m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_STRING;
	if (!StartupInfo.SettingsControl(Key.m_pHandle->m_Handle, SCTL_GET, 0, &Fsi)) {
		strBuffer = pszDefault;
		return;
	}
	strBuffer = Fsi.String;
}

__int64 QuerySettingsInt64Value   (CFarSettingsKey &Key, const TCHAR *pszKeyName, __int64 nDefault)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = Key.m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_QWORD;
	if (!StartupInfo.SettingsControl(Key.m_pHandle->m_Handle, SCTL_GET, 0, &Fsi)) {
		return nDefault;
	}

	return Fsi.Number;
}

void QuerySettingsBoolValue  (CFarSettingsKey &Key, const TCHAR *pszKeyName, bool *bValue, bool bDefault)
{
	int nValue;
	QuerySettingsIntValue(Key, pszKeyName, &nValue, bDefault ? 1 : 0);
	*bValue = (nValue != 0);
}

void QuerySettingsBinaryValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, std::vector<BYTE> &arrData)
{
	arrData.clear();

	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = Key.m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_DATA;
	if (!StartupInfo.SettingsControl(Key.m_pHandle->m_Handle, SCTL_GET, 0, &Fsi))
		return;

	if (Fsi.Data.Size == 0)
		return;

	arrData.resize(Fsi.Data.Size);
	memmove(&arrData[0], Fsi.Data.Data, Fsi.Data.Size);
}

void SetSettingsStringValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, const std::tstring &strBuffer)
{
	Key.SetStringValue(pszKeyName, strBuffer);
}

void SetSettingsIntValue   (CFarSettingsKey &Key, const TCHAR *pszKeyName, __int64 nValue)
{
	Key.SetIntValue(pszKeyName, nValue);
}

void SetSettingsBinaryValue(CFarSettingsKey &Key, const TCHAR *pszKeyName, const void *pData, int nLength)
{
	FarSettingsItem Fsi;
	Fsi.StructSize = sizeof(FarSettingsItem);
	Fsi.Root = Key.m_Key;
	Fsi.Name = pszKeyName;
	Fsi.Type = FST_DATA;
	Fsi.Data.Data = pData;
	Fsi.Data.Size = nLength;
	StartupInfo.SettingsControl(Key.m_pHandle->m_Handle, SCTL_SET, 0, &Fsi);
}

#endif

#ifndef FAR_NO_NAMESPACE
};
#endif
