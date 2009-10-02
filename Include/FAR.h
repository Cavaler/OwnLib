#ifndef __FAR_H
#define __FAR_H

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

#define _FAR_USE_WIN32_FIND_DATA
#ifdef _UNICODE
#include <plugin2.hpp>
#else
#include <plugin.hpp>
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

extern PluginStartupInfo StartupInfo;
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

class CFarMenuItem : public FarMenuItem {
public:
	CFarMenuItem();
	CFarMenuItem(const TCHAR *szTitle);
	CFarMenuItem(const tstring &strTitle);
	CFarMenuItem(int nMsgID);
	CFarMenuItem(bool bSeparator);
	CFarMenuItem(const CFarMenuItem &Item);
	CFarMenuItem(const FarMenuItem &Item);
	void operator=(const FarMenuItem &Item);
	~CFarMenuItem();
};

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
