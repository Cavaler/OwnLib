#ifndef __FARDLG_H
#define __FARDLG_H

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;

#include <Far.h>

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

class CFarDialogItem;
class CFarEventHandler;

// *********************** GENERAL ***********************

class CFarDialog {
public:
	CFarDialog(int iX,int iY,const char *szHelpTopic,DWORD dwFlags=0);
	CFarDialog(int iX1,int iY1,int iX2,int iY2,const char *szHelpTopic,DWORD dwFlags=0);
	int  AddFrame(const char *Title);
	int  AddFrame(int TitleId);
	int  AddFrame();
	int  Add(CFarDialogItem *Item);
	int  Add(CFarDialogItem *Item,CFarEventHandler *pHandler);
	int  AddButton(const char *szTitle);
	int  AddButton(int nId);
	int  AddButtons(const char *OKTitle,const char *CancelTitle);
	int  AddButtons(int OKId,int CancelId);
	void SetFocus(int Focus);
	void SetWindowProc(FARWINDOWPROC lpWindowProc,long lParam);
	void SetHandler(CFarEventHandler *pHandler,long lParam);
	static long WINAPI s_WindowProc(HANDLE hDlg, int Msg, int Param1, long Param2);
	int  WindowProc(HANDLE hDlg, int Msg, int Param1, long Param2);
	int  Display(int ValidExitCodes,...);
	~CFarDialog();
protected:
	int X1,Y1,X2,Y2,Focused;
	const char *HelpTopic;
	CFarDialogItem **Items;
	int ItemsNumber;
	CFarEventHandler *m_pHandler;
	vector<CFarEventHandler *> m_arrItemHandlers;

	long m_lParam;
	bool m_bHandled;
	DWORD m_dwFlags;
};

// *********************** HELPERS ***********************

class CFarText {
public:
	CFarText() : m_strHolder() {}
	CFarText(const CFarText &Text) : m_strHolder(Text.m_strHolder.c_str()) {}
	CFarText(int nMsgID) : m_strHolder(GetMsg(nMsgID)) {}
	CFarText(int nMsgID, const char *pszModule) : m_strHolder(GetMsgEx(nMsgID, pszModule)) {}
	CFarText(const char *pszText) : m_strHolder(pszText ? pszText : "") {}
	CFarText(const string &strText) : m_strHolder(strText.c_str()) {}
	operator const char *() const {return m_strHolder.c_str();}
protected:
	string m_strHolder;
};

class CFarListData {
public:
	CFarListData();
	CFarListData(FarList *pList, bool bCopy = false);
	CFarListData(const char **ppszItems,int iItemCount);
	CFarListData(const vector<CFarText> arrItems);
	int Append(const char *szItem);
	operator FarList *() const {return m_pList;}
	~CFarListData();
protected:
	FarList *m_pList;
	bool m_bFree;
};

class CFarIntegerConverter {
public:
	static CFarIntegerConverter Instance;
	virtual void ToString(int iValue, char *pszBuffer, int nSize);
	virtual bool FromString(const char *pszBuffer, int &iValue);
};

class CFarHexConverter : public CFarIntegerConverter {
public:
	static CFarHexConverter Instance;
	virtual void ToString(int iValue, char *pszBuffer, int nSize);
	virtual bool FromString(const char *pszBuffer, int &iValue);
};

class CFarHexDnConverter : public CFarHexConverter {
public:
	static CFarHexDnConverter Instance;
	virtual void ToString(int iValue, char *pszBuffer, int nSize);
};

class CFarSizeConverter : public CFarIntegerConverter {
public:
	static CFarSizeConverter Instance;
	virtual void ToString(int iValue, char *pszBuffer, int nSize);
	virtual bool FromString(const char *pszBuffer, int &iValue);
};

class CFarStorage {
public:
	virtual void Get(char *pszBuffer, int nSize) const = 0;
	virtual void Put(const char *pszBuffer) = 0;
	virtual bool Verify(const char *pszBuffer) {return true;}
	virtual operator string() const = 0;
};

class CFarTextStorage : public CFarStorage {
public:
	CFarTextStorage():
		m_nMethod(-1) {}
	CFarTextStorage(char &pchBuffer, bool bReadOnly = false):
		m_nMethod(0), m_pszBuffer(&pchBuffer), m_nSize(1), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(char *pszBuffer, int nSize = -1, bool bReadOnly = false):
		m_nMethod(0), m_pszBuffer(pszBuffer),
		m_nSize(nSize >= 0 ? nSize : strlen(pszBuffer)+1), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(const char *pszBuffer, int nSize = -1):
		m_nMethod(0), m_pszBuffer((char *)pszBuffer),
		m_nSize(nSize >= 0 ? nSize : strlen(pszBuffer)+1), m_bReadOnly(true) {}
	CFarTextStorage(char **ppszBuffer, bool bReadOnly = false):
		m_nMethod(1), m_ppszBuffer(ppszBuffer), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(string &pstrBuffer, bool bReadOnly = false):
		m_nMethod(2), m_pstrBuffer(&pstrBuffer), m_bReadOnly(bReadOnly) {}

	virtual void Get(char *pszBuffer, int nSize) const;
	virtual void Put(const char *pszBuffer);
	virtual operator string() const;
protected:
	bool m_bReadOnly;
	int m_nMethod;
	union {
		struct {
			char *m_pszBuffer;
			int m_nSize;
		};
		char **m_ppszBuffer;
		string *m_pstrBuffer;
	};
};

class CFarIntegerStorage : public CFarStorage {
public:
	CFarIntegerStorage(unsigned char *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(0), m_pUChar(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(unsigned char &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(0), m_pUChar(&pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed char *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(1), m_pSChar(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed char &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(1), m_pSChar(&pValue), m_pConverter(pConverter) {}

	CFarIntegerStorage(unsigned short *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(2), m_pUShort(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(unsigned short &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(2), m_pUShort(&pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed short *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(3), m_pSShort(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed short &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(3), m_pSShort(&pValue), m_pConverter(pConverter) {}

	CFarIntegerStorage(unsigned int *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(4), m_pULong((unsigned long *)pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(unsigned int &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(4), m_pULong((unsigned long *)&pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed int *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(5), m_pSLong((signed long *)pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed int &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(5), m_pSLong((signed long *)&pValue), m_pConverter(pConverter) {}

	CFarIntegerStorage(unsigned long *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(4), m_pULong(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(unsigned long &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(4), m_pULong(&pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed long *pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(5), m_pSLong(pValue), m_pConverter(pConverter) {}
	CFarIntegerStorage(signed long &pValue, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_nMethod(5), m_pSLong(&pValue), m_pConverter(pConverter) {}

	virtual void Get(char *pszBuffer, int nSize) const;
	virtual void Put(const char *pszBuffer);
	virtual bool Verify(const char *pszBuffer);
	virtual operator string() const;

	virtual int Get() const;
	virtual void Put(int nValue);
protected:
	int m_nMethod;
	union {
		unsigned char *m_pUChar;
		signed char *m_pSChar;
		unsigned short *m_pUShort;
		signed short *m_pSShort;
		unsigned long *m_pULong;
		signed long *m_pSLong;
	};
	CFarIntegerConverter *m_pConverter;
};

// *********************** HANDLERS ***********************

class CFarEventHandler {
public:
	CFarEventHandler(CFarEventHandler *pNextHandler=NULL);
	virtual bool Process(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result)=0;
	~CFarEventHandler();
protected:
	bool ProcessChain(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result);
	friend class CFarDialog;
private:
	CFarEventHandler *m_pNextHandler;
};

enum {FWP_FALSE, FWP_ASRESULT, FWP_TRUE};

class CFarWindowProcHandler : public CFarEventHandler {
public:
	CFarWindowProcHandler(FARWINDOWPROC pProc, int nResult, CFarEventHandler *pNextHandler = NULL);
	virtual bool Process(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result);
protected:
	int m_nResult;
	FARWINDOWPROC m_pProc;
};

class CFarItemEventHandler : public CFarEventHandler {
public:
	CFarItemEventHandler(CFarItemEventHandler *pNextHandler=NULL);
	virtual bool Process(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result);
};

// *********************** ITEMS ***********************

class CFarDialogItem {
public:
	CFarDialogItem(int iX1,int iY1,int iX2,int iY2,DWORD dwFlags,CFarText szText=NULL);
	virtual void CreateItem(FarDialogItem *Item);
	virtual bool Validate(FarDialogItem *Item) {return true;};
	virtual void StoreData(FarDialogItem *Item) {};
	virtual ~CFarDialogItem();
protected:
	int X1,Y1,X2,Y2;
	DWORD Flags;
	string Text;
};

class CFarCustomItem:public CFarDialogItem {
public:
	CFarCustomItem(int iX1,int iY1,int iX2,int iY2,int iSelected,DWORD dwFlags,CFarText szText=NULL);
	virtual void CreateItem(FarDialogItem *Item);
protected:
	int Selected;
};

class CFarBoxItem:public CFarDialogItem {
public:
	CFarBoxItem(BOOL bDouble,int iX1,int iY1,int iX2,int iY2,DWORD dwFlags,CFarText szText=NULL);
	virtual void CreateItem(FarDialogItem *Item);
protected:
	BOOL Double;
};

class CFarTextItem:public CFarDialogItem {
public:
	CFarTextItem(int X,int Y,DWORD dwFlags,CFarText szText=NULL);
	virtual void CreateItem(FarDialogItem *Item);
protected:
	BOOL Double;
};

class CFarButtonItem:public CFarDialogItem {
public:
	CFarButtonItem(int X,int Y,DWORD dwFlags,BOOL bDefault,CFarText szText);
	virtual void CreateItem(FarDialogItem *Item);
protected:
	BOOL Default;
};

class CFarCheckBoxItem:public CFarDialogItem {
public:
	CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,bool *pbVariable);
	CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,bool &pbVariable);
	CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL *pBVariable);
	CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL &pBVariable);
	CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage,int iMaskValue);
	virtual void CreateItem(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarCheckBoxItem();
protected:
	int m_nMethod;
	union {
		bool *m_pbVariable;
		BOOL *m_pBVariable;
		struct {
			CFarIntegerStorage *m_piStorage;
			int m_iValue;
		};
	};
};

class CFarCheckBox3Item:public CFarDialogItem {
public:
	CFarCheckBox3Item(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage);
	CFarCheckBox3Item(int X,int Y,DWORD dwFlags,CFarText szText,DWORD *pdwCleared,DWORD *pdwSet,DWORD dwMaskValue);
	virtual void CreateItem(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarCheckBox3Item();
protected:
	int m_nMethod;
	union {
		CFarIntegerStorage *m_piStorage;
		struct {
			DWORD *m_pdwCleared;
			DWORD *m_pdwSet;
			DWORD m_dwMaskValue;
		};
	};
};

class CFarRadioButtonItem : public CFarCheckBoxItem {
public:
	CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,bool *pbVariable);
	CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,bool &pbVariable);
	CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL *pBVariable);
	CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL &pBVariable);
	CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage,int iValue);
	virtual void CreateItem(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
};

class CFarListBoxItem:public CFarDialogItem {
public:
	CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,int *piVariable);
	CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarStorage *pStorage);
	CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarTextStorage TextStorage);
	CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarIntegerStorage TextStorage);
	virtual void CreateItem(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarListBoxItem();
protected:
	int *m_piVariable;
	CFarListData *m_pData;
	CFarStorage *m_pStorage;
};

class CFarValidator;

class CFarEditItem:public CFarDialogItem {
public:
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const char *szHistory,CFarStorage *pStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const char *szHistory,CFarTextStorage TextStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const char *szHistory,CFarIntegerStorage TextStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	virtual void CreateItem(FarDialogItem *Item);
	virtual bool Validate(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarEditItem();
protected:
	int m_iType;
	const char *m_pszHistory;
	CFarStorage *m_pStorage;
	CFarValidator *m_pValidator;
};

class CFarComboBoxItem:public CFarEditItem {
public:
	CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarStorage *pStorage,CFarValidator *pValidator=NULL);
	CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarTextStorage TextStorage,CFarValidator *pValidator=NULL);
	CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarIntegerStorage TextStorage,CFarValidator *pValidator=NULL,int nOffset=0);
	virtual void CreateItem(FarDialogItem *Item);
	virtual bool Validate(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarComboBoxItem();
protected:
	CFarListData *m_pData;
	int m_nOffset;		// 0th element in list has this number
};

// ********************* VALIDATORS ********************
typedef bool (*FarStringValidator)(char *pszData, int nSize, void *pData);

class CFarValidator {
public:
	virtual bool Validate(char *pszData, int nSize)=0;
	virtual ~CFarValidator() {}
};

class CFarStringValidator : public CFarValidator {
public:
	CFarStringValidator(FarStringValidator pValidator, void *pData):
		m_pValidator(pValidator), m_pData(pData) {}
	virtual bool Validate(char *pszData, int nSize) {return (m_pValidator) ? m_pValidator(pszData, nSize, m_pData) : true;}
protected:
	FarStringValidator m_pValidator;
	void *m_pData;
};

class CFarIntegerRangeValidator : public CFarValidator {
public:
	static const char *s_szErrorMsg;
	static const char *s_szHelpTopic;
	const char *m_szErrorMsg;
	const char *m_szHelpTopic;

	CFarIntegerRangeValidator(int iMin, int iMax, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_iMin(iMin), m_iMax(iMax), m_pConverter(pConverter), 
		m_szErrorMsg(s_szErrorMsg), m_szHelpTopic(s_szHelpTopic) {}
	virtual bool Validate(char *pszData, int nSize);
protected:
	int m_iMin,m_iMax;
	CFarIntegerConverter *m_pConverter;
};

class CFarDetailedRangeValidator : public CFarValidator {
public:
	static const char *s_szTooBigMsg;
	static const char *s_szTooSmallMsg;
	static const char *s_szNotANumberMsg;
	static const char *s_szTooBigTopic;
	static const char *s_szTooSmallTopic;
	static const char *s_szNotANumberTopic;

	CFarDetailedRangeValidator(int iMin,int iMax, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_iMin(iMin), m_iMax(iMax), m_pConverter(pConverter), 
		m_szTooBigMsg(s_szTooBigMsg), m_szTooSmallMsg(s_szTooSmallMsg),
		m_szNotANumberMsg(s_szNotANumberMsg), m_szTooBigTopic(s_szTooBigTopic),
		m_szTooSmallTopic(s_szTooSmallTopic), m_szNotANumberTopic(s_szNotANumberTopic) {}
	virtual bool Validate(char *pszData, int nSize);
protected:
	int m_iMin,m_iMax;
	CFarIntegerConverter *m_pConverter;
public:
	const char *m_szTooBigMsg;
	const char *m_szTooSmallMsg;
	const char *m_szNotANumberMsg;
	const char *m_szTooBigTopic;
	const char *m_szTooSmallTopic;
	const char *m_szNotANumberTopic;
};

#ifndef FAR_NO_NAMESPACE
};
#endif

#endif
