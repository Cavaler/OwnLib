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

CPluginStartupInfo StartupInfo;
const TCHAR *g_pszErrorTitle;
const TCHAR *g_pszErrorTopic;
const TCHAR *g_pszLastErrorTopic;
const TCHAR *g_pszOKButton;
HANDLE g_hSaveScreen;
bool g_bInterrupted;

const TCHAR *GetMsg(int MsgId) {
	return const_cast<TCHAR *>(StartupInfo.GetMsg(MsgId));
}

int WhichRadioButton(struct FarDialogItem *Item,int ItemsNumber) {
	for (int I=0;I<ItemsNumber;I++) if (Item[I].Selected) return I;
	return -1;
}

#pragma region("CFarMenuItemEx")

void CFarMenuItemEx::SetText(const TCHAR *szTitle) {
#ifdef UNICODE
	Text = _tcsdup(szTitle);
#else
	strncpy(Text.Text, szTitle, sizeof(Text));
#endif
}

CFarMenuItemEx::CFarMenuItemEx()
{
	Flags = UserData = 0;
#ifdef FAR3
	Reserved[0] = Reserved[1] = 0;
	AccelKey.VirtualKeyCode = 0;
	AccelKey.ControlKeyState = 0;
#else
	Reserved = 0;
	AccelKey = 0;
#endif
	SetText(_T(""));
}

CFarMenuItemEx::CFarMenuItemEx(const TCHAR *szTitle, DWORD dwFlags)
{
	Flags = dwFlags;
	UserData = 0;
#ifdef FAR3
	Reserved[0] = Reserved[1] = 0;
	AccelKey.VirtualKeyCode = 0;
	AccelKey.ControlKeyState = 0;
#else
	Reserved = 0;
	AccelKey = 0;
#endif
	SetText(szTitle);
}

CFarMenuItemEx::CFarMenuItemEx(const tstring &strTitle, DWORD dwFlags)
{
	Flags = dwFlags;
	UserData = 0;
#ifdef FAR3
	Reserved[0] = Reserved[1] = 0;
	AccelKey.VirtualKeyCode = 0;
	AccelKey.ControlKeyState = 0;
#else
	Reserved = 0;
	AccelKey = 0;
#endif
	SetText(strTitle.c_str());
}

CFarMenuItemEx::CFarMenuItemEx(int nMsgID, DWORD dwFlags)
{
	Flags = dwFlags;
	UserData = 0;
#ifdef FAR3
	Reserved[0] = Reserved[1] = 0;
	AccelKey.VirtualKeyCode = 0;
	AccelKey.ControlKeyState = 0;
#else
	Reserved = 0;
	AccelKey = 0;
#endif
	SetText(GetMsg(nMsgID));
}

CFarMenuItemEx::CFarMenuItemEx(bool bSeparator)
{
	Flags = bSeparator ? MIF_SEPARATOR : 0;
	UserData = 0;
#ifdef FAR3
	Reserved[0] = Reserved[1] = 0;
	AccelKey.VirtualKeyCode = 0;
	AccelKey.ControlKeyState = 0;
#else
	Reserved = 0;
	AccelKey = 0;
#endif
	SetText(_T(""));
}

CFarMenuItemEx::CFarMenuItemEx(const CFarMenuItemEx &Item) {
	*this = (const FarMenuItemEx &)Item;
}

CFarMenuItemEx::CFarMenuItemEx(const FarMenuItemEx &Item) {
	*this = Item;
}

void CFarMenuItemEx::operator=(const FarMenuItemEx &Item) {
	Flags = Item.Flags;
	AccelKey = Item.AccelKey;
	UserData = Item.UserData;
#ifdef UNICODE
#ifdef FAR3
	Reserved[0] = Item.Reserved[0];
	Reserved[1] = Item.Reserved[1];
#else
	Reserved = Item.Reserved;
#endif
	SetText(Item.Text);
#else
	Reserved = Item.Reserved;
	SetText(Item.Text.Text);
#endif
}

void CFarMenuItemEx::operator=(const CFarMenuItemEx &Item) {
	*this = (const FarMenuItemEx &)Item;
}

#ifndef FAR3

CFarMenuItemEx::CFarMenuItemEx(const CFarMenuItem &Item) {
	*this = (const FarMenuItem &)Item;
}

CFarMenuItemEx::CFarMenuItemEx(const FarMenuItem &Item) {
	*this = Item;
}

void CFarMenuItemEx::operator=(const FarMenuItem &Item) {
	Flags =
		(Item.Selected  ? MIF_SELECTED  : 0) |
		(Item.Checked   ? MIF_CHECKED   : 0) |
		(Item.Separator ? MIF_SEPARATOR : 0);
	AccelKey = 0;
	Reserved = 0;
	UserData = 0;
	SetText(Item.Text);
}

#endif

bool CFarMenuItemEx::Checked() const
{
	return (Flags & MIF_CHECKED) != 0;
}

void CFarMenuItemEx::Check(bool bCheck)
{
	if (bCheck)
		Flags |=  MIF_CHECKED;
	else
		Flags &= ~MIF_CHECKED;
}

bool CFarMenuItemEx::Selected() const
{
	return (Flags & MIF_SELECTED) != 0;
}

void CFarMenuItemEx::Select(bool bSelect)
{
	if (bSelect)
		Flags |=  MIF_SELECTED;
	else
		Flags &= ~MIF_SELECTED;
}

CFarMenuItemEx::~CFarMenuItemEx() {
#ifdef UNICODE
	if (Text) free((void *)Text);
#endif
}

#pragma endregion

int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iItemsNumber, int iButtonsNumber, ...) {
	const TCHAR **ppszItems = new const TCHAR *[iItemsNumber];
	va_list List;
	va_start(List,iButtonsNumber);
	for (int I=0; I<iItemsNumber; I++)
		ppszItems[I]=va_arg(List, const TCHAR *);
	va_end(List);

	int Result = StartupInfo.Message(uiFlags, szHelpTopic, ppszItems, iItemsNumber, iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iButtonsNumber, std::vector<std::tstring> &arrItems) {
	const TCHAR **ppszItems = new const TCHAR *[arrItems.size()];
	for (int I=0; I < (int)arrItems.size(); I++)
		ppszItems[I]=arrItems[I].c_str();

	int Result = StartupInfo.Message(uiFlags, szHelpTopic, ppszItems, arrItems.size(), iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

void ShowMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	const TCHAR *ppszItems[] = {pszTitle, pszMessage1, pszMessage2, g_pszOKButton};
	if (pszMessage2) {
		StartupInfo.Message(0, _T(""), ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(0, _T(""), ppszItems, 3, 1);
	}
}

void ShowError(const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	const TCHAR *ppszItems[] = {g_pszErrorTitle, pszMessage1, pszMessage2, g_pszOKButton};
	if (pszMessage2) {
		StartupInfo.Message(FMSG_WARNING, g_pszErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(FMSG_WARNING, g_pszErrorTopic, ppszItems, 3, 1);
	}
}

void ShowLastError(const TCHAR *pszMessage, const TCHAR *pszFileName) {
	const TCHAR *ppszItems[] = {g_pszErrorTitle, pszMessage, pszFileName, g_pszOKButton};
	if (pszFileName) {
		StartupInfo.Message(FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 3, 1);
	}
}

void ShowWaitMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	g_hSaveScreen = StartupInfo.SaveScreen(0, 0, -1, -1);
	const TCHAR *ppszItems[] = {pszTitle, pszMessage1, pszMessage2};
	StartupInfo.Message(0, _T(""), ppszItems, pszMessage2?3:2, 0);
}

void HideWaitMessage() {
	StartupInfo.RestoreScreen(g_hSaveScreen);
}

bool Interrupted() {
	if (g_bInterrupted) return true;

	static DWORD dwTick = 0;
	DWORD dwCurTick = GetTickCount();
	if (dwCurTick < dwTick+250) return false;
	dwTick = dwCurTick;

	static HANDLE hInput=GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD rcInput;
	DWORD dwRead;
	do {
		if (!PeekConsoleInput(hInput,&rcInput,1,&dwRead)) return false;
		if (!dwRead) return FALSE;
		if (!ReadConsoleInput(hInput,&rcInput,1,&dwRead)) return false;
		if ((rcInput.EventType==KEY_EVENT) && (rcInput.Event.KeyEvent.bKeyDown)
			&& ((rcInput.Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE) ||
				(rcInput.Event.KeyEvent.wVirtualScanCode==VK_ESCAPE)))
			return g_bInterrupted=true;
	} while (dwRead);
	return false;
}

#ifndef _UNICODE
bool SetFileName(PluginPanelItem &Item, const TCHAR *szName) {
	return _tcsncpy_s(Item.FindData.cFileName, CleanFileName(szName).c_str(), _TRUNCATE) == 0;
}

bool SetFileName(PluginPanelItem &Item, const std::string& strName) {
	return _tcsncpy_s(Item.FindData.cFileName, CleanFileName(strName).c_str(), _TRUNCATE) == 0;
}
#endif

void SetMode(HANDLE hPlugin, int iViewMode, int iSortMode, int iSortOrder, int OpMode) {
	if (OpMode & (OPM_SILENT | OPM_FIND)) return;

#ifndef _UNICODE
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, &iViewMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, &iSortMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, &iSortOrder);
#else
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, (int)iViewMode, NULL);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, (int)iSortMode, NULL);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, (int)iSortOrder, NULL);
#endif
}

tstring FarMaskToRE(const TCHAR *szMask) {
	tstring strRE = _T("^");
	const TCHAR *szCur = szMask;

	while (*szCur) {
		switch (*szCur) {
		case '.':
			strRE += _T("\\.");
			break;
		case '?':
			strRE += _T(".");
			break;
		case '*':
			strRE += _T(".*");
			break;
		case '[':{
			strRE += '[';
			do {
				szCur++;
				switch (*szCur) {
				case 0:
				case ']':
				case ',':
					break;
				default:
					strRE += *szCur;
					break;
				}
			} while (*szCur && (*szCur != ']'));
			strRE += ']';
			    }
			break;
		case '{':		case '}':
		case '(':		case ')':
		case ']':		case '+':
		case '^':		case '$':
			strRE += '\\';
			strRE += *szCur;
			break;

		default:
			strRE += *szCur;
			break;
		}
		szCur++;
	}

	if (_tcscspn(szMask, _T(".?*[")) != _tcslen(szMask)) strRE += '$';	// Masks without any of these are non-terminal
	return strRE;
}

CFarMaskSet::CFarMaskSet(const TCHAR *szMasks)
: m_pInclude(NULL), m_pIncludeExtra(NULL), m_pExclude(NULL), m_pExcludeExtra(NULL)
{
	bool bExclude = false;
	tstring strCurMask = _T("");

	setlocale(LC_ALL, ".OCP");
	m_pOEMTable = pcre_maketables();

	while (*szMasks) {
		switch (*szMasks) {
		case ' ':
		case '\t':
		case ';':
		case ',':
			szMasks++;
			break;
		case '|':
			if (!bExclude) {
				bExclude = true;
				const TCHAR *szErr;
				int nErr;
				m_pInclude = pcre_compile(strCurMask.c_str(), PCRE_CASELESS, &szErr, &nErr, m_pOEMTable);
				if (m_pInclude) m_pIncludeExtra = pcre_study(m_pInclude, 0, &szErr);
				strCurMask = _T("");
			}
			szMasks++;
			break;
		case '"':{
			const TCHAR *szEnd = _tcschr(szMasks+1, '"');
			if (!strCurMask.empty()) strCurMask += '|';
			if (szEnd) {
				strCurMask += _T('(') + FarMaskToRE(tstring(szMasks+1, szEnd - szMasks - 1).c_str()) + _T(')');
				szMasks = szEnd+1;
			} else {
				strCurMask += _T('(') + FarMaskToRE(szMasks) + _T(')');
				szMasks = _tcschr(szMasks, 0);
			}
			    }
			break;
		default:{
			int nLen = _tcscspn(szMasks, _T(",;|"));
			while ((nLen > 0) && (isspace(szMasks[nLen-1]))) nLen--;

			if (!strCurMask.empty()) strCurMask += '|';
			strCurMask += _T('(') + FarMaskToRE(tstring(szMasks, nLen).c_str()) + _T(')');
			szMasks += nLen;
			   }
			break;
		}
	}

	if (bExclude) {
		const TCHAR *szErr;
		int nErr;
		m_pExclude = pcre_compile(strCurMask.c_str(), PCRE_CASELESS, &szErr, &nErr, m_pOEMTable);
		if (m_pExclude) m_pExcludeExtra = pcre_study(m_pInclude, 0, &szErr);
	} else {
		const TCHAR *szErr;
		int nErr;
		m_pInclude = pcre_compile(strCurMask.c_str(), PCRE_CASELESS, &szErr, &nErr, m_pOEMTable);
		if (m_pInclude) m_pIncludeExtra = pcre_study(m_pInclude, 0, &szErr);

		m_pExclude = NULL;
		m_pExcludeExtra = NULL;
	}
}

bool CFarMaskSet::operator()(const TCHAR *szFileName) {
	const TCHAR *szName = _tcsrchr(szFileName, '\\');
	tstring strName = (szName) ? szName + 1 :szFileName;
	if (strName.find('.') == string::npos) strName += '.';

	if (m_pExclude && (pcre_exec(m_pExclude, m_pExcludeExtra, strName.c_str(), strName.length(), 0, 0, NULL, 0) >= 0)) return false;
	return (m_pInclude && (pcre_exec(m_pInclude, m_pIncludeExtra, strName.c_str(), strName.length(), 0, 0, NULL, 0)) >= 0);
}

bool CFarMaskSet::Valid() {
	return m_pInclude != NULL;
}

CFarMaskSet::~CFarMaskSet() {
	if (m_pInclude) pcre_free(m_pInclude);
	if (m_pIncludeExtra) pcre_free(m_pIncludeExtra);
	if (m_pExclude) pcre_free(m_pExclude);
	if (m_pExcludeExtra) pcre_free(m_pExcludeExtra);
	if (m_pOEMTable) pcre_free((void *)m_pOEMTable);
}

CFarSaveScreen::CFarSaveScreen(const TCHAR *szMessage) {
	m_hSave = StartupInfo.SaveScreen(0, 0, -1, -1);
	ViewMessage(szMessage);
}

CFarSaveScreen::CFarSaveScreen(int X1, int Y1, int X2, int Y2, const TCHAR *szMessage) {
	m_hSave = StartupInfo.SaveScreen(X1, Y1, X2, Y2);
	ViewMessage(szMessage);
}

void CFarSaveScreen::ViewMessage(const TCHAR *szMessage) {
	m_strTitle.resize(256);
	while (GetConsoleTitle(&m_strTitle[0], m_strTitle.size()) == 0) m_strTitle.resize(m_strTitle.size()*2);

	if (szMessage) {
		SetConsoleTitle(szMessage);
		Message(0, NULL, 2, 0, "", szMessage);
	}
}

CFarSaveScreen::~CFarSaveScreen() {
	StartupInfo.RestoreScreen(m_hSave);
	SetConsoleTitle(&m_strTitle[0]);
}

CFarPanelMode::CFarPanelMode()
: m_iViewMode(4), m_iSortMode(SM_NAME), m_iSortOrder(0), m_bNeedApply(true)
{
}

CFarPanelMode::CFarPanelMode(int iViewMode, int iSortMode, int iSortOrder)
: m_iViewMode(iViewMode), m_iSortMode(iSortMode), m_iSortOrder(iSortOrder), m_bNeedApply(true)
{
}

void CFarPanelMode::LoadReg(HKEY hKey)
{
	QueryRegIntValue(hKey, _T("ViewMode"), &m_iViewMode, m_iViewMode, 0, 9);
	QueryRegIntValue(hKey, _T("SortMode"), &m_iSortMode, m_iSortMode, 0, 11);
	QueryRegIntValue(hKey, _T("SortOrder"), &m_iSortOrder, m_iSortOrder, 0, 1);
}

void CFarPanelMode::SaveReg(HKEY hKey)
{
	SetRegIntValue(hKey, _T("ViewMode"), m_iViewMode);
	SetRegIntValue(hKey, _T("SortMode"), m_iSortMode);
	SetRegIntValue(hKey, _T("SortOrder"), m_iSortOrder);
}

void CFarPanelMode::LoadReg(CFarSettingsKey &hKey)
{
	hKey.QueryIntValue(_T("ViewMode"),  m_iViewMode);
	hKey.QueryIntValue(_T("SortMode"),  m_iSortMode);
	hKey.QueryIntValue(_T("SortOrder"), m_iSortOrder);
}

void CFarPanelMode::SaveReg(CFarSettingsKey &hKey)
{
	hKey.SetIntValue(_T("ViewMode"),  m_iViewMode);
	hKey.SetIntValue(_T("SortMode"),  m_iSortMode);
	hKey.SetIntValue(_T("SortOrder"), m_iSortOrder);
}

void CFarPanelMode::Assign(HANDLE hPlugin) {
	PanelInfo PInfo;
#ifdef UNICODE
	if (StartupInfo.Control(hPlugin, FCTL_GETPANELINFO, 0, (LONG_PTR)&PInfo)) Assign(PInfo);
#else
	if (StartupInfo.Control(hPlugin, FCTL_GETPANELINFO, &PInfo)) Assign(PInfo);
#endif
}

void CFarPanelMode::Assign(PanelInfo &PInfo) {
	m_iViewMode = PInfo.ViewMode;
	m_iSortMode = PInfo.SortMode;
	m_iSortOrder = (PInfo.Flags & PFLAGS_REVERSESORTORDER) ? 1 : 0;
}

void CFarPanelMode::Apply(HANDLE hPlugin, int nOpMode) {
	if (nOpMode & (OPM_SILENT | OPM_FIND)) return;

#ifdef UNICODE
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, m_iViewMode, NULL);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, m_iSortMode, NULL);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, m_iSortOrder, NULL);
#else
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, &m_iViewMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, &m_iSortMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, &m_iSortOrder);
#endif
}

CEditorSetString::CEditorSetString(int nNumber, LPCTSTR szText, LPCTSTR szEOL, int nLength)
{
#ifdef FAR3
	StructSize = sizeof(EditorSetString);
#endif
	StringNumber = nNumber;
	StringText = szText;
	StringEOL = szEOL;
	StringLength = nLength;
}

CFarSplitString::CFarSplitString(int nMax) : m_arrLines(nMax) {
}

CFarSplitString::CFarSplitString(int nMax, const tstring &strText) : m_arrLines(nMax) {
	Split(strText);
}

tstring &CFarSplitString::operator[](int nIndex) {
	return m_arrLines[nIndex];
}

void CFarSplitString::Split(const tstring &strText) {
	tstring strRemain = strText;

	for (size_t nIndex = 0; nIndex < m_arrLines.size()-1; nIndex++) {
		int nPos = strRemain.find('\n');
		if (nPos == string::npos) {
			m_arrLines[nIndex] = strRemain;
			strRemain = _T("");
		} else {
			m_arrLines[nIndex] = strRemain.substr(0, nPos);
			strRemain = strRemain.substr(nPos+1);
		}
	}
	m_arrLines[m_arrLines.size()-1] = strRemain;
}

tstring CFarSplitString::Combine() {
	tstring strResult;
	for (int nIndex = m_arrLines.size()-1; nIndex >= 0; nIndex--) {
		if (!strResult.empty()) strResult = m_arrLines[nIndex] + _T("\n") + strResult; else
		if (!m_arrLines[nIndex].empty()) strResult = m_arrLines[nIndex];
	}
	return strResult;
}

#ifdef UNICODE

#ifndef FAR3

WIN32_FIND_DATA FFDtoWFD(const FAR_FIND_DATA &Data) {
	WIN32_FIND_DATA fd;

	fd.dwFileAttributes = Data.dwFileAttributes;
	fd.ftCreationTime = Data.ftCreationTime;
	fd.ftLastAccessTime = Data.ftLastAccessTime;
	fd.ftLastWriteTime = Data.ftLastWriteTime;
	fd.nFileSizeHigh = (DWORD)(Data.nFileSize >> 32);
	fd.nFileSizeLow = (DWORD)(Data.nFileSize);
	fd.dwReserved0 = 0;
	fd.dwReserved1 = 0;
	_tcsncpy(fd.cFileName, Data.lpwszFileName, sizeof(fd.cFileName)/sizeof(fd.cFileName[0]));
	_tcsncpy(fd.cAlternateFileName, Data.lpwszAlternateFileName, sizeof(fd.cAlternateFileName)/sizeof(fd.cAlternateFileName[0]));

	return fd;
}

#endif

CPluginPanelItem::CPluginPanelItem()
{
#ifdef FAR3
	FileName = NULL;
	AlternateFileName = NULL;
	UserData.FreeData = NULL;
#else
	FindData.lpwszFileName = NULL;
	FindData.lpwszAlternateFileName = NULL;
#endif
	Description = NULL;
	Owner = NULL;
}

CPluginPanelItem::CPluginPanelItem(const CPluginPanelItem &item)
{
	*this = (const PluginPanelItem &)item;
}

CPluginPanelItem::CPluginPanelItem(const PluginPanelItem &item)
{
	*this = item;
}

void CPluginPanelItem::operator = (const PluginPanelItem &item)
{
	(PluginPanelItem &)(*this) = item;

#ifdef FAR3
	if (FileName) FileName = _wcsdup(FileName);
	if (AlternateFileName) AlternateFileName = _wcsdup(AlternateFileName);
#else
	if (FindData.lpwszFileName) FindData.lpwszFileName = _wcsdup(FindData.lpwszFileName);
	if (FindData.lpwszAlternateFileName) FindData.lpwszAlternateFileName = _wcsdup(FindData.lpwszAlternateFileName);
#endif

	if (Description) Description = _wcsdup(Description);
	if (Owner) Owner = _wcsdup(Owner);
}

void CPluginPanelItem::operator = (const CPluginPanelItem &item)
{
	*this = (PluginPanelItem &)item;
}

void CPluginPanelItem::SetFindData(const WIN32_FIND_DATA &fd)
{
#ifdef FAR3
	FileAttributes = fd.dwFileAttributes;
	CreationTime = fd.ftCreationTime;
	LastAccessTime = fd.ftLastAccessTime;
	LastWriteTime = fd.ftLastWriteTime;
	FileSize = (((__int64)fd.nFileSizeHigh) << 32) + fd.nFileSizeLow;
	FileName = _wcsdup(fd.cFileName);
	AlternateFileName = _wcsdup(fd.cAlternateFileName);
#else
	FindData.dwFileAttributes = fd.dwFileAttributes;
	FindData.ftCreationTime = fd.ftCreationTime;
	FindData.ftLastAccessTime = fd.ftLastAccessTime;
	FindData.ftLastWriteTime = fd.ftLastWriteTime;
	FindData.nFileSize = (((__int64)fd.nFileSizeHigh) << 32) + fd.nFileSizeLow;
	FindData.lpwszFileName = _wcsdup(fd.cFileName);
	FindData.lpwszAlternateFileName = _wcsdup(fd.cAlternateFileName);
#endif
}

CPluginPanelItem::~CPluginPanelItem()
{
#ifdef FAR3
	if (FileName) free((void *)FileName);
	if (AlternateFileName) free((void *)AlternateFileName);
#else
	if (FindData.lpwszFileName) free((void *)FindData.lpwszFileName);
	if (FindData.lpwszAlternateFileName) free((void *)FindData.lpwszAlternateFileName);
#endif
	if (Description) free((void *)Description);
	if (Owner) free((void *)Owner);
}

void GetPanelItems(int nCount, bool bSelected, bool bAnotherPanel, panelitem_vector &arrItems, panelbuffer_vector &arrBuffers)
{
	GetPanelItems(nCount, bSelected, bAnotherPanel ? PANEL_PASSIVE : PANEL_ACTIVE, arrItems, arrBuffers);
}

void GetPanelItems(int nCount, bool bSelected, HANDLE hPanel, panelitem_vector &arrItems, panelbuffer_vector &arrBuffers)
{
	PluginPanelItem Item;
	arrItems.clear();
	arrBuffers.clear();

	DWORD dwCtl = bSelected ? FCTL_GETSELECTEDPANELITEM : FCTL_GETPANELITEM;

	for (int nItem = 0; nItem < nCount; nItem++) {
		int nSize = StartupInfo.Control(hPanel, dwCtl, nItem, NULL);

		vector<BYTE> arrItem(nSize);
		StartupInfo.Control(hPanel, dwCtl, nItem, (LONG_PTR)&arrItem[0]);

		memmove(&Item, &arrItem[0], sizeof(PluginPanelItem));
		arrItems.push_back(Item);
		arrBuffers.push_back(arrItem);
	}
}

void UpdatePanel(bool bClearSelection, const TCHAR *szCurrentName, bool bAnotherPanel)
{
	HANDLE hPanel = bAnotherPanel ? PANEL_PASSIVE : PANEL_ACTIVE;
	StartupInfo.Control(hPanel, FCTL_UPDATEPANEL, bClearSelection, NULL);

	if (szCurrentName)
	{
		CPanelInfo PInfo;
		PInfo.GetInfo(bAnotherPanel);
		PanelRedrawInfo RInfo = {ITEM_SS(PanelRedrawInfo) PInfo.Find(szCurrentName), 0};

		StartupInfo.Control(hPanel, FCTL_REDRAWPANEL, 0, (LONG_PTR)&RInfo);

	} else {
		StartupInfo.Control(hPanel, FCTL_REDRAWPANEL, 0, NULL);
	}
}

void SetPanelSelection(bool bAnotherPanel, const panelitem_vector &arrItems) {
	HANDLE hPlugin = bAnotherPanel ? PANEL_PASSIVE : PANEL_ACTIVE;
	StartupInfo.Control(hPlugin, FCTL_BEGINSELECTION, 0, NULL);

	for (size_t nItem = 0; nItem < arrItems.size(); nItem++)
		StartupInfo.Control(hPlugin, FCTL_SETSELECTION, nItem, (arrItems[nItem].Flags & PPIF_SELECTED) ? 1 : 0);

	StartupInfo.Control(hPlugin, FCTL_ENDSELECTION, 0, NULL);
}

void SetPanelSelection(CPanelInfo &Info, bool bAnotherPanel, bool bRedraw) {
	SetPanelSelection(bAnotherPanel, Info.PanelItems);
	if (bRedraw)
		StartupInfo.Control(bAnotherPanel ? PANEL_PASSIVE : PANEL_ACTIVE, FCTL_REDRAWPANEL, 0, NULL);
}

bool CPanelInfo::GetInfo(bool bAnotherPanel)
{
	return GetInfo(bAnotherPanel ? PANEL_PASSIVE : PANEL_ACTIVE);
}

bool CPanelInfo::GetInfo(HANDLE hPanel)
{
#ifdef FAR3
	StructSize = sizeof(PanelInfo);
#endif

	if (!StartupInfo.Control(hPanel, FCTL_GETPANELINFO, 0, (LONG_PTR)(PanelInfo *)this)) return false;

	wchar_t szCurDir[MAX_PATH];
	StartupInfo.Control(hPanel, FCTL_GETCURRENTDIRECTORY, MAX_PATH, (LONG_PTR)szCurDir);
	strCurDir = szCurDir;
	CurDir = strCurDir.c_str();

	GetPanelItems(ItemsNumber, false, hPanel, PanelItems, PanelBuffers);
	GetPanelItems(SelectedItemsNumber, true, hPanel, SelectedItems, SelectedBuffers);

#ifdef FAR3
	Plugin = PluginHandle != NO_PANEL_HANDLE;
#endif

	return true;
}

#else // UNICODE

bool CPanelInfo::GetInfo(bool bAnotherPanel)
{
	return StartupInfo.Control(INVALID_HANDLE_VALUE,
		bAnotherPanel ? FCTL_GETANOTHERPANELINFO : FCTL_GETPANELINFO,
		(PanelInfo *)this) != 0;
}

bool CPanelInfo::GetInfo(HANDLE hPanel)
{
	return StartupInfo.Control(hPanel, FCTL_GETPANELINFO, (PanelInfo *)this) != 0;
}

void UpdatePanel(bool bClearSelection, const TCHAR *szCurrentName, bool bAnotherPanel)
{
	StartupInfo.Control(INVALID_HANDLE_VALUE,
		bAnotherPanel ? FCTL_UPDATEANOTHERPANEL : FCTL_UPDATEPANEL,
		bClearSelection ? NULL : (void *)!NULL);

	if (szCurrentName)
	{
		CPanelInfo PInfo;
		PInfo.GetInfo(bAnotherPanel);
		PanelRedrawInfo RInfo = {PInfo.Find(szCurrentName), 0};

		StartupInfo.Control(INVALID_HANDLE_VALUE,
			bAnotherPanel ? FCTL_REDRAWANOTHERPANEL : FCTL_REDRAWPANEL,
			&RInfo);

	} else {
		StartupInfo.Control(INVALID_HANDLE_VALUE,
			bAnotherPanel ? FCTL_REDRAWANOTHERPANEL : FCTL_REDRAWPANEL,
			NULL);
	}
}

void SetPanelSelection(CPanelInfo &Info, bool bAnotherPanel, bool bRedraw) {
	StartupInfo.Control(INVALID_HANDLE_VALUE, bAnotherPanel ? FCTL_SETANOTHERSELECTION : FCTL_SETSELECTION, &Info);
	if (bRedraw)
		StartupInfo.Control(INVALID_HANDLE_VALUE, bAnotherPanel ? FCTL_REDRAWANOTHERPANEL : FCTL_REDRAWPANEL, NULL);
}

#endif // UNICODE

int CPanelInfo::Find(LPCTSTR szFileName) {
	for (int nItem = 0; nItem < ItemsNumber; nItem++) {
		if (_tcscmp(FarPanelFileName(PanelItems[nItem]), szFileName) == 0) return nItem;
	}
	return -1;
}

void SetFindDataName(PluginPanelItem &Item, const TCHAR *szFileName, const TCHAR *szAlternateFileName)
{
#ifdef UNICODE
#ifdef FAR3
	Item.FileName = _wcsdup(szFileName);
	if (szAlternateFileName)
		Item.AlternateFileName = _wcsdup(szAlternateFileName);
#else
	Item.FindData.lpwszFileName = _wcsdup(szFileName);
	if (szAlternateFileName)
		Item.FindData.lpwszAlternateFileName = _wcsdup(szAlternateFileName);
#endif
#else
	strcpy(Item.FindData.cFileName, szFileName);
	if (szAlternateFileName)
		strcpy(Item.FindData.cAlternateFileName, szAlternateFileName);
#endif
}

void StdFreeFindData(PluginPanelItem *PanelItems, int ItemsNumber)
{
	for (int nItem = 0; nItem < ItemsNumber; nItem++) {
#ifdef UNICODE
#ifdef FAR3
		if (PanelItems[nItem].FileName) free((void *)PanelItems[nItem].FileName);
		if (PanelItems[nItem].AlternateFileName) free((void *)PanelItems[nItem].AlternateFileName);
#else
		if (PanelItems[nItem].FindData.lpwszFileName) free((void *)PanelItems[nItem].FindData.lpwszFileName);
		if (PanelItems[nItem].FindData.lpwszAlternateFileName) free((void *)PanelItems[nItem].FindData.lpwszAlternateFileName);
#endif
#endif
		if (PanelItems[nItem].Description) free((void *)PanelItems[nItem].Description);
		if (PanelItems[nItem].Owner) free((void *)PanelItems[nItem].Owner);
	}

	free(PanelItems);
}

tstring GetDlgItemText(HANDLE hDlg, int nItem) {
#ifdef UNICODE
	return (const wchar_t *)StartupInfo.SendDlgMessage(hDlg, DM_GETCONSTTEXTPTR, nItem, 0);
#else
	FarDialogItem Item;
	StartupInfo.SendDlgMessage(hDlg, DM_GETDLGITEM, nItem, (LONG_PTR)&Item);
	return (Item.Flags & DIF_VAREDIT) ? Item.Ptr.PtrData : Item.Data;
#endif
}

void SetDlgItemText(HANDLE hDlg, int nID, const TCHAR *szText) {
	StartupInfo.SendDlgMessage(hDlg, DM_SETTEXTPTR, nID, (LONG_PTR)szText);
}

void ShowDlgItem(HANDLE hDlg, int nID, bool bShow) {
	StartupInfo.SendDlgMessage(hDlg, DM_SHOWITEM, nID, (bShow) ? 1 : 0);
}

void EnableDlgItem(HANDLE hDlg, int nID, bool bEnable) {
	StartupInfo.SendDlgMessage(hDlg, DM_ENABLE, nID, bEnable);
}

bool IsDlgItemChecked(HANDLE hDlg, int nID) {
	return StartupInfo.SendDlgMessage(hDlg, DM_GETCHECK, nID, 0) != 0;
}

void CheckDlgItem(HANDLE hDlg, int nID, bool bCheck) {
	StartupInfo.SendDlgMessage(hDlg, DM_SETCHECK, nID, bCheck);
}

int  GetDlgListPos(HANDLE hDlg, int nID) {
	return StartupInfo.SendDlgMessage(hDlg, DM_LISTGETCURPOS, nID, NULL);
}

void SetDlgListPos(HANDLE hDlg, int nID, int nPos) {
	StartupInfo.SendDlgMessage(hDlg, DM_LISTSETCURPOS, nID, nPos);
}

#ifndef FAR_NO_NAMESPACE
};
#endif
