#ifndef __FAR_H
#define __FAR_H

#ifdef FAR3
#define LIB_APPEND "_3"
#endif
#define LIB_NAME "FarLib"
#include "GenLibName.h"

#ifndef _WIN64
#pragma pack(2)
#endif
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#endif

#include <vector>
#include <string>
#include "tstring.h"
#include "handles.h"

#define _FAR_USE_WIN32_FIND_DATA

#ifdef UNICODE
#ifdef FAR3
#pragma pack(push,8)
#include <plugin3.hpp>
#include <farcolor3.hpp>
#pragma pack(pop)
#define FCTL_GETCURRENTDIRECTORY FCTL_GETPANELDIRECTORY
#define FCTL_CLOSEPLUGIN FCTL_CLOSEPANEL
#define FMENU_USEEXT 0
#define FCTL_SETPANELDIR FCTL_SETPANELDIRECTORY
#define NO_PANEL_HANDLE NULL
#define CP_AUTODETECT CP_DEFAULT
enum FAR_PKF_FLAGS
{
	PKF_CONTROL     = 0x00000001,
	PKF_ALT         = 0x00000002,
	PKF_SHIFT       = 0x00000004,
};
#else
#include <plugin2.hpp>
#include <farkeys2.hpp>
#include <farcolor2.hpp>
#define FCTL_GETCURRENTDIRECTORY FCTL_GETPANELDIR
#define NO_PANEL_HANDLE INVALID_HANDLE_VALUE
#endif
#define FAR_EXPORT(name) name##W
#else
#include <plugin.hpp>
#include <farkeys.hpp>
#include <farcolor.hpp>
#define FAR_EXPORT(name) name
#define NO_PANEL_HANDLE INVALID_HANDLE_VALUE
#endif

#include <CRegExp.h>

#define PSI140SIZE	312
#define PSI150SIZE	324
#define PSI152SIZE	332
#define PSI160SIZE	336
#define PSI170SIZE	372

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

class CPluginStartupInfo : public PluginStartupInfo
{
public:
	const TCHAR *GetMsg(int MsgId);
	int Message(DWORD Flags, const TCHAR *HelpTopic, const TCHAR ** Items, 	int ItemsNumber, int ButtonsNumber);

#ifdef FAR3
	GUID	m_GUID;
	wchar_t RootKey[MAX_PATH];
	void operator =(const PluginStartupInfo &Info);

//	int Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
//		const TCHAR *HelpTopic, const FarKey *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber);
	int Menu(int X, int Y, int MaxHeight, FARMENUFLAGS Flags, const TCHAR *Title, const TCHAR *Bottom,
		const TCHAR *HelpTopic, const int *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber);
	INT_PTR Control(HANDLE hPanel, DWORD Command, int Param1, LONG_PTR Param2);
	INT_PTR SendDlgMessage(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2);
	LONG_PTR DefDlgProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2);
	int EditorControl(int Command, void *Param);
	int ViewerControl(int Command, void *Param);
#else
	void operator =(const PluginStartupInfo &Info);
	int Menu(int X, int Y, int MaxHeight, DWORD Flags, const TCHAR *Title, const TCHAR *Bottom,
		const TCHAR *HelpTopic, const int *BreakKeys, int *BreakCode, const FarMenuItem *Item, size_t ItemsNumber);
#endif
};

extern CPluginStartupInfo StartupInfo;
extern const TCHAR *g_pszErrorTitle;
extern const TCHAR *g_pszErrorTopic;
extern const TCHAR *g_pszLastErrorTopic;
extern const TCHAR *g_pszOKButton;
extern HANDLE g_hSaveScreen;
extern bool g_bInterrupted;

tstring FarMaskToRE(const TCHAR *szMask);

class CFarMaskSet {
public:
	CFarMaskSet(const TCHAR *szMasks);
	bool operator()(const TCHAR *szFileName);
	bool Valid();
	~CFarMaskSet();
protected:
	pcre *m_pInclude;
	pcre_extra *m_pIncludeExtra;
	pcre *m_pExclude;
	pcre_extra *m_pExcludeExtra;

	const unsigned char *m_pOEMTable;
};

class CFarSaveScreen {
public:
	CFarSaveScreen(const TCHAR *szMessage = NULL);
	CFarSaveScreen(int X1, int Y1, int X2, int Y2, const TCHAR *szMessage = NULL);
	~CFarSaveScreen();
protected:
	HANDLE m_hSave;

	void ViewMessage(const TCHAR *szMessage);
	std::vector<TCHAR> m_strTitle;
};

class CFarPanelMode {
public:
	CFarPanelMode();
	CFarPanelMode(int iViewMode, int iSortMode, int iSortOrder);
	void LoadReg(HKEY hKey);
	void SaveReg(HKEY hKey);

	void Assign(HANDLE hPlugin);
	void Assign(PanelInfo &PInfo);
	void Apply(HANDLE hPlugin, int nOpMode = 0);

public:
	int m_iViewMode;
	int m_iSortMode;
	int m_iSortOrder;

	bool m_bNeedApply;
};

class CFarSplitString {
public:
	CFarSplitString(int nMax);
	CFarSplitString(int nMax, const tstring &strText);
	tstring &operator[](int nIndex);
	void Split(const tstring &strText);
	tstring Combine();
protected:
	vector<tstring> m_arrLines;
};

//	Different order in FAR3 requires this
struct CEditorSetString : public EditorSetString
{
	CEditorSetString(int nNumber, LPCTSTR szText, LPCTSTR szEOL, int nLength = -1);
};

#ifdef FAR3
#define FarMenuItemEx FarMenuItem
#else
class CFarMenuItem : public FarMenuItem {
public:
	CFarMenuItem();
	CFarMenuItem(const TCHAR *szTitle);
	CFarMenuItem(const tstring &strTitle);
	CFarMenuItem(int nMsgID);
	CFarMenuItem(bool bSeparator);
	CFarMenuItem(const CFarMenuItem &Item);
	CFarMenuItem(const FarMenuItem &Item);
	void operator=(const  FarMenuItem &Item);
	void operator=(const CFarMenuItem &Item);
	~CFarMenuItem();
protected:
	void SetText(const TCHAR *szTitle);
};
#endif

class CFarMenuItemEx : public FarMenuItemEx {
public:
	CFarMenuItemEx();
	CFarMenuItemEx(const TCHAR *szTitle, DWORD dwFlags = 0);
	CFarMenuItemEx(const tstring &strTitle, DWORD dwFlags = 0);
	CFarMenuItemEx(int nMsgID, DWORD dwFlags = 0);
	CFarMenuItemEx(bool bSeparator);

	CFarMenuItemEx(const CFarMenuItemEx &Item);
	CFarMenuItemEx(const  FarMenuItemEx &Item);
	void operator=(const  FarMenuItemEx &Item);
	void operator=(const CFarMenuItemEx &Item);

#ifndef FAR3
	CFarMenuItemEx(const CFarMenuItem &Item);
	CFarMenuItemEx(const  FarMenuItem &Item);
	void operator=(const  FarMenuItem &Item);
#endif

	bool Checked() const;
	void Check(bool bCheck = true);
	bool Selected() const;
	void Select(bool bSelect = true);

	~CFarMenuItemEx();
protected:
	void SetText(const TCHAR *szTitle);
};

#ifdef FAR3
vector<FarKey> ConvertKeys(const int *piBreakKeys);
#else
void UpgradeMenuItemVector(const vector<CFarMenuItem> &arrSrc, vector<CFarMenuItemEx> &arrDst);
#endif

#include "FarSettings.h"

#ifdef UNICODE

#ifdef FAR3
#define FarPanelFileName(pi) (pi).FileName
#define FarPanelAttr(pi) (pi).FileAttributes
typedef WIN32_FIND_DATA WF_FIND_DATA;
WIN32_FIND_DATA PanelToWFD(const PluginPanelItem &Item);
#else
#define FarFileName(fd) (fd).lpwszFileName
#define FarPanelFileName(pi) FarFileName((pi).FindData)
#define FarPanelAttr(pi) (pi).FindData.dwFileAttributes
WIN32_FIND_DATA FFDtoWFD(const FAR_FIND_DATA &Data);
#define PanelToWFD(Item) FFDtoWFD(Item.FindData)
typedef FAR_FIND_DATA WF_FIND_DATA;
#endif

struct CPluginPanelItem : PluginPanelItem {
	CPluginPanelItem();
	CPluginPanelItem(const CPluginPanelItem &item);
	CPluginPanelItem(const  PluginPanelItem &item);
	void operator = (const  PluginPanelItem &item);
	void operator = (const CPluginPanelItem &item);
	void SetFindData(const WIN32_FIND_DATA &fd);
	~CPluginPanelItem();
};
typedef vector<CPluginPanelItem> panelitem_vector;

typedef vector<BYTE> panelbuffer;
typedef vector<panelbuffer> panelbuffer_vector;

void GetPanelItems(int nCount, bool bSelected, bool bAnotherPanel, panelitem_vector &arrItems, panelbuffer_vector &arrBuffers);
void GetPanelItems(int nCount, bool bSelected, HANDLE hPanel, panelitem_vector &arrItems, panelbuffer_vector &arrBuffers);
void SetPanelSelection(bool bAnotherPanel, const panelitem_vector &arrItems);

struct CPanelInfo : PanelInfo
{
	LPCWSTR CurDir;
	panelitem_vector PanelItems;
	panelitem_vector SelectedItems;

	panelbuffer_vector PanelBuffers;
	panelbuffer_vector SelectedBuffers;

	bool GetInfo(bool bAnotherPanel = false);
	bool GetInfo(HANDLE hPanel);
	wstring strCurDir;

	int Find(LPCTSTR szFileName);
#ifdef FAR3
	bool Plugin;
#endif
};

#else

#define FarFileName(fd) (fd).cFileName
#define FarPanelFileName(pi) FarFileName((pi).FindData)
#define FarPanelAttr(pi) (pi).FindData.dwFileAttributes
#define FFDtoWFD(Data) (Data)
#define PanelToWFD(Item) Item.FindData
typedef WIN32_FIND_DATA WF_FIND_DATA;

struct CPanelInfo : PanelInfo {
	bool GetInfo(bool bAnotherPanel = false);
	bool GetInfo(HANDLE hPanel);
	int Find(LPCTSTR szFileName);
};

typedef PluginPanelItem CPluginPanelItem;
typedef vector<CPluginPanelItem> panelitem_vector;

#endif	// UNICODE

void SetFindDataName(PluginPanelItem &Item, const TCHAR *szFileName, const TCHAR *szAlternateFileName = NULL);
void StdFreeFindData(PluginPanelItem *PanelItems, int ItemsNumber);

void UpdatePanel(bool bClearSelection, const TCHAR *szCurrentName = NULL, bool bAnotherPanel = false);
void SetPanelSelection(CPanelInfo &Info, bool bAnotherPanel, bool bRedraw);

#if defined(_VECTOR_) || defined (_STLP_VECTOR)
int  ChooseMenu(std::vector<std::tstring> &arrItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			 int iDefault, unsigned int uiFlags = FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,
			 const int *piBreakKeys = NULL, int *nBreakCode = NULL);
int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iButtonsNumber, std::vector<std::tstring> &arrItems);
#endif

const TCHAR *GetMsg(int MsgId);
int  WhichRadioButton(struct FarDialogItem *Item,int ItemsNumber);
int  ChooseMenu(int ItemCount, const TCHAR **ppszItems, const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic,
			 int iDefault, unsigned int uiFlags = FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,
			 const int *piBreakKeys = NULL, int *nBreakCode = NULL);
int  ChooseMenu(const TCHAR *Title, const TCHAR *Bottom, const TCHAR *HelpTopic, int ItemCount, ...);
int  Message(UINT uiFlags, const TCHAR *szHelpTopic, int iItemsNumber, int iButtonsNumber, ...);
void ShowMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2 = NULL);
void ShowError(const TCHAR *pszMessage1, const TCHAR *pszMessage2 = NULL);
void ShowLastError(const TCHAR *pszMessage, const TCHAR *pszFileName = NULL);
void ShowWaitMessage(const TCHAR *pszTitle, const TCHAR *pszMessage1, const TCHAR *pszMessage2 = NULL);
void HideWaitMessage();
bool Interrupted();

bool SetFileName(PluginPanelItem &Item, const TCHAR *szName);
bool SetFileName(PluginPanelItem &Item, const std::tstring& strName);

void SetMode(HANDLE hPlugin, int iViewMode, int iSortMode, int iSortOrder, int OpMode=0);

tstring GetDlgItemText(HANDLE hDlg, int nItem);
void SetDlgItemText(HANDLE hDlg, int nID, const TCHAR *szText);
void ShowDlgItem(HANDLE hDlg, int nID, bool bShow);
void EnableDlgItem(HANDLE hDlg, int nID, bool bEnable);
bool IsDlgItemChecked(HANDLE hDlg, int nID);
void CheckDlgItem(HANDLE hDlg, int nID, bool bCheck);
int  GetDlgListPos(HANDLE hDlg, int nID);
void SetDlgListPos(HANDLE hDlg, int nID, int nPos);

// FarGetMsgEx.cpp
void InitLanguageFiles();
void FreeLanguageFiles();
const TCHAR *GetMsgEx(int MsgId, const TCHAR *Module=NULL);

#ifndef FAR_NO_NAMESPACE
};
#ifdef FAR_USE_NAMESPACE
using namespace FarLib;
#endif
#endif

#endif
