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

PluginStartupInfo StartupInfo;
const TCHAR *g_pszErrorTitle;
const TCHAR *g_pszErrorTopic;
const TCHAR *g_pszLastErrorTopic;
const TCHAR *g_pszOKButton;
HANDLE g_hSaveScreen;
bool g_bInterrupted;

const TCHAR *GetMsg(int MsgId) {
	return const_cast<TCHAR *>(StartupInfo.GetMsg(StartupInfo.ModuleNumber,MsgId));
}
#define strccpy(to, from) strncpy(to, from, sizeof(to))
#define _tcsccpy(to, from) _tcsncpy(to, from, sizeof(to)/sizeof(to[0]))

int WhichRadioButton(struct FarDialogItem *Item,int ItemsNumber) {
	for (int I=0;I<ItemsNumber;I++) if (Item[I].Selected) return I;
	return -1;
}

CFarMenuItem::CFarMenuItem(const TCHAR *szTitle) {
	Selected = Checked = Separator = 0;
#ifdef _UNICODE
	m_strText = szTitle;
	Text = m_strText.c_str();
#else
	_tcscpy(Text, szTitle);
#endif
}

CFarMenuItem::CFarMenuItem(const tstring &strTitle) {
	Selected = Checked = Separator = 0;
#ifdef _UNICODE
	m_strText = strTitle;
	Text = m_strText.c_str();
#else
	_tcscpy(Text, strTitle.c_str());
#endif
}

CFarMenuItem::CFarMenuItem(int nMsgID) {
	Selected = Checked = Separator = 0;
#ifdef _UNICODE
	m_strText = GetMsg(nMsgID);
	Text = m_strText.c_str();
#else
	_tcscpy(Text, GetMsg(nMsgID));
#endif
}

CFarMenuItem::CFarMenuItem(bool bSeparator) {
	Selected = Checked = 0;
	Separator = bSeparator ? 1 : 0;
#ifdef _UNICODE
	m_strText = _T("");
	Text = m_strText.c_str();
#else
	Text[0] = 0;
#endif
}

CFarMenuItem::CFarMenuItem(const CFarMenuItem &Item)
{
	*this = Item;
}

void CFarMenuItem::operator=(const CFarMenuItem &Item)
{
	Selected = Item.Selected;
	Checked = Item.Checked;
	Separator = Item.Separator;
#ifdef _UNICODE
	m_strText = Item.m_strText;
	Text = m_strText.c_str();
#else
	_tcscpy(Text, Item.Text);
#endif
}

int ChooseMenu(int ItemCount, const TCHAR **ppszItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			int iDefault, unsigned int uiFlags, const int *piBreakKeys, int *nBreakCode) {
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

int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iItemsNumber, int iButtonsNumber, ...) {
	const TCHAR **ppszItems = new const TCHAR *[iItemsNumber];
	va_list List;
	va_start(List,iButtonsNumber);
	for (int I=0; I<iItemsNumber; I++)
		ppszItems[I]=va_arg(List, const TCHAR *);
	va_end(List);

	int Result = StartupInfo.Message(StartupInfo.ModuleNumber, uiFlags, szHelpTopic, ppszItems, iItemsNumber, iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iButtonsNumber, std::vector<std::tstring> &arrItems) {
	const TCHAR **ppszItems = new const TCHAR *[arrItems.size()];
	for (int I=0; I < (int)arrItems.size(); I++)
		ppszItems[I]=arrItems[I].c_str();

	int Result = StartupInfo.Message(StartupInfo.ModuleNumber, uiFlags, szHelpTopic, ppszItems, arrItems.size(), iButtonsNumber);
	delete[] ppszItems;
	return Result;
}

void ShowMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	const TCHAR *ppszItems[] = {pszTitle, pszMessage1, pszMessage2, GetMsg(0)};
	if (pszMessage2) {
		StartupInfo.Message(StartupInfo.ModuleNumber, 0, _T(""), ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, 0, _T(""), ppszItems, 3, 1);
	}
}

void ShowError(const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	const TCHAR *ppszItems[] = {g_pszErrorTitle, pszMessage1, pszMessage2, GetMsg(0)};
	if (pszMessage2) {
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING, g_pszErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING, g_pszErrorTopic, ppszItems, 3, 1);
	}
}

void ShowLastError(const TCHAR *pszMessage, const TCHAR *pszFileName) {
	const TCHAR *ppszItems[] = {g_pszErrorTitle, pszMessage, pszFileName, GetMsg(0)};
	if (pszFileName) {
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 4, 1);
	} else {
		ppszItems[2] = ppszItems[3];
		StartupInfo.Message(StartupInfo.ModuleNumber, FMSG_WARNING|FMSG_ERRORTYPE, g_pszLastErrorTopic, ppszItems, 3, 1);
	}
}

void ShowWaitMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2) {
	g_hSaveScreen = StartupInfo.SaveScreen(0, 0, -1, -1);
	const TCHAR *ppszItems[] = {pszTitle, pszMessage1, pszMessage2};
	StartupInfo.Message(StartupInfo.ModuleNumber, 0, _T(""), ppszItems, pszMessage2?3:2, 0);
}

void HideWaitMessage() {
	StartupInfo.RestoreScreen(g_hSaveScreen);
}

bool Interrupted() {
	if (g_bInterrupted) return true;

	static HANDLE hInput=GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD rcInput;
	DWORD dwRead;
	do {
		if (!PeekConsoleInput(hInput,&rcInput,1,&dwRead)) return false;
		if (!dwRead) return FALSE;
		if (!ReadConsoleInput(hInput,&rcInput,1,&dwRead)) return false;
		if ((rcInput.EventType==KEY_EVENT) && (rcInput.Event.KeyEvent.bKeyDown)
			&& (rcInput.Event.KeyEvent.wVirtualScanCode==VK_ESCAPE)) return g_bInterrupted=true;
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

void CFarPanelMode::LoadReg(HKEY hKey) {
	QueryRegIntValue(hKey, _T("ViewMode"), &m_iViewMode, m_iViewMode, 0, 9);
	QueryRegIntValue(hKey, _T("SortMode"), &m_iSortMode, m_iSortMode, 0, 11);
	QueryRegIntValue(hKey, _T("SortOrder"), &m_iSortOrder, m_iSortOrder, 0, 1);
}

void CFarPanelMode::SaveReg(HKEY hKey) {
	SetRegIntValue(hKey, _T("ViewMode"), m_iViewMode);
	SetRegIntValue(hKey, _T("SortMode"), m_iSortMode);
	SetRegIntValue(hKey, _T("SortOrder"), m_iSortOrder);
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
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, m_iViewMode, 0);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, m_iSortMode, 0);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, m_iSortOrder, 0);
#else
	StartupInfo.Control(hPlugin, FCTL_SETVIEWMODE, &m_iViewMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTMODE, &m_iSortMode);
	StartupInfo.Control(hPlugin, FCTL_SETSORTORDER, &m_iSortOrder);
#endif
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

#ifndef FAR_NO_NAMESPACE
};
#endif
