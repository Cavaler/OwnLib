#ifndef __FAR_H
#define __FAR_H

#pragma pack(1)
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#endif
#include <plugin.hpp>
#include <pcre\pcre.h>

#define PSI140SIZE	312
#define PSI150SIZE	324
#define PSI152SIZE	332
#define PSI160SIZE	336
#define PSI170SIZE	372

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

struct InitDialogItem {
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  unsigned int Selected;
  unsigned int Flags;
  unsigned char DefaultButton;
  char *Data;
};

extern PluginStartupInfo StartupInfo;
extern const char *g_pszErrorTitle;
extern const char *g_pszErrorTopic;
extern const char *g_pszLastErrorTopic;
extern const char *g_pszOKButton;
extern HANDLE g_hSaveScreen;

#if defined(_STRING_) || defined (_STLP_STRING)
string FarMaskToRE(const char *szMask);
#endif

class CFarMaskSet {
public:
	CFarMaskSet(const char *szMasks);
	bool operator()(const char *szFileName);
	bool Valid();
	~CFarMaskSet();
protected:
	pcre *m_pInclude;
	pcre_extra *m_pIncludeExtra;
	pcre *m_pExclude;
	pcre_extra *m_pExcludeExtra;
};

#if defined(_VECTOR_) || defined (_STLP_VECTOR)
int  ChooseMenu(std::vector<std::string> &arrItems, const char *Title, const char *Bottom, const char *HelpTopic,
			 int iDefault, unsigned int uiFlags = FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,
			 const int *piBreakKeys = NULL, int *nBreakCode = NULL);
int  Message(UINT uiFlags, const char *szHelpTopic, int iButtonsNumber, std::vector<std::string> &arrItems);
#endif

const char *GetMsg(int MsgId);
void InitDialogItems(struct InitDialogItem *Init,struct FarDialogItem *Item,int ItemsNumber);
int  WhichRadioButton(struct FarDialogItem *Item,int ItemsNumber);
int  ChooseMenu(int ItemCount, const char **ppszItems, const char *Title, const char *Bottom, const char *HelpTopic,
			 int iDefault, unsigned int uiFlags = FMENU_WRAPMODE|FMENU_AUTOHIGHLIGHT,
			 const int *piBreakKeys = NULL, int *nBreakCode = NULL);
int  ChooseMenu(const char *Title, const char *Bottom, const char *HelpTopic, int ItemCount, ...);
int  Message(UINT uiFlags, const char *szHelpTopic, int iItemsNumber, int iButtonsNumber, ...);
void ShowMessage(const char *pszTitle, const char *pszMessage1, const char *pszMessage2 = NULL);
void ShowError(const char *pszMessage1, const char *pszMessage2 = NULL);
void ShowLastError(const char *pszMessage, const char *pszFileName = NULL);
void ShowWaitMessage(const char *pszTitle, const char *pszMessage1, const char *pszMessage2 = NULL);
void HideWaitMessage();

// FarGetMsgEx.cpp
void InitLanguageFiles();
void FreeLanguageFiles();
const char *GetMsgEx(int MsgId, const char *Module=NULL);

#ifndef FAR_NO_NAMESPACE
};
#ifdef FAR_USE_NAMESPACE
using namespace FarLib;
#endif
#endif

#endif
