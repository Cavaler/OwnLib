#ifndef __FARDLG_H
#define __FARDLG_H

#pragma warning(disable:4786)
#include <vector>
#include <string>
#include <map>
using namespace std;

#include <Far.h>

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

class CFarDialog;
class CFarDialogItem;

// *********************** GENERAL ***********************

typedef LONG_PTR(WINAPI *CFARWINDOWPROC)(
	CFarDialog *pDlg,
    HANDLE      hDlg,
    int         Msg,
    int         Param1,
    LONG_PTR    Param2
);

class CFarDialog {
public:
	CFarDialog(int iX,int iY,const TCHAR *szHelpTopic,DWORD dwFlags=0);
	CFarDialog(int iX1,int iY1,int iX2,int iY2,const TCHAR *szHelpTopic,DWORD dwFlags=0);
	int  AddFrame(const TCHAR *Title);
	int  AddFrame(int TitleId);
	int  AddFrame();
	int  Add(CFarDialogItem *Item);
	int  Add(CFarDialogItem *Item, int nOwnID);
	int  AddButton(const TCHAR *szTitle);
	int  AddButton(int nId);
	int  AddButtons(const TCHAR *OKTitle,const TCHAR *CancelTitle);
	int  AddButtons(int OKId,int CancelId);

	void SetFocus(int Focus);
	int  Display(int ValidExitCodes,...);

	void SetUseID(bool bUseID);
	int  GetID(int nIndex);
	int  GetIndex(int nID);

	void SetWindowProc( FARWINDOWPROC lpWindowProc,  long lParam);
	void SetWindowProc(CFARWINDOWPROC lpCWindowProc, long lParam);
	~CFarDialog();

protected:

	int X1,Y1,X2,Y2,Focused;
	const TCHAR *HelpTopic;

	vector<CFarDialogItem *>	Items;
	map<int, size_t>			m_mapCodes;

	long	m_lParam;
	DWORD	m_dwFlags;

	bool	m_bUseID;

	static LONG_PTR WINAPI s_WindowProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2);
	LONG_PTR WindowProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2);
	 FARWINDOWPROC m_pWindowProc;
	CFARWINDOWPROC m_pCWindowProc;

	bool AnyWindowProc();
};

// *********************** HELPERS ***********************

class CFarText {
public:
	CFarText() : m_strHolder(), m_nOwnID(0) {}
	CFarText(const CFarText &Text) : m_strHolder(Text.m_strHolder.c_str()), m_nOwnID(Text.ID()) {}
	CFarText(int nMsgID) : m_strHolder(GetMsg(nMsgID)), m_nOwnID(nMsgID) {}
	CFarText(int nMsgID, const TCHAR *pszModule) : m_strHolder(GetMsgEx(nMsgID, pszModule)), m_nOwnID(nMsgID) {}
	CFarText(const TCHAR *pszText) : m_strHolder(pszText ? pszText : _T("")), m_nOwnID(0) {}
	CFarText(const tstring &strText) : m_strHolder(strText.c_str()), m_nOwnID(0) {}

	int ID() const { return m_nOwnID; }
	operator const TCHAR *() const {return m_strHolder.c_str();}
protected:
	int     m_nOwnID;
	tstring m_strHolder;
};

class CFarListData {
public:
	CFarListData();
	CFarListData(FarList *pList, bool bCopy = false);
	CFarListData(const TCHAR **ppszItems,int iItemCount);
	CFarListData(const vector<CFarText> arrItems);
	int Append(const TCHAR *szItem);
	operator FarList *() const {return m_pList;}
	~CFarListData();
protected:
	FarList *m_pList;
	bool m_bFree;
};

class CFarIntegerConverter {
public:
	static CFarIntegerConverter Instance;
	
	virtual void ToString(int iValue, TCHAR *pszBuffer, int nSize);
	virtual bool FromString(const TCHAR *pszBuffer, int &iValue);
};

class CFarDoubleConverter {
public:
	CFarDoubleConverter() : m_nDigits(-1) {}
	CFarDoubleConverter(int nDigits) : m_nDigits(nDigits) {}
	static CFarDoubleConverter Instance;
	static CFarDoubleConverter Instance0;
	static CFarDoubleConverter Instance1;
	static CFarDoubleConverter Instance2;

	virtual void ToString(double iValue, TCHAR *pszBuffer, int nSize);
	virtual bool FromString(const TCHAR *pszBuffer, double &iValue);
protected:
	int m_nDigits;
};

class CFarHexConverter : public CFarIntegerConverter {
public:
	static CFarHexConverter Instance;
	virtual void ToString(int iValue, TCHAR *pszBuffer, int nSize);
	virtual bool FromString(const TCHAR *pszBuffer, int &iValue);
};

class CFarHexDnConverter : public CFarHexConverter {
public:
	static CFarHexDnConverter Instance;
	virtual void ToString(int iValue, TCHAR *pszBuffer, int nSize);
};

class CFarSizeConverter : public CFarIntegerConverter {
public:
	static CFarSizeConverter Instance;
	virtual void ToString(int iValue, TCHAR *pszBuffer, int nSize);
	virtual bool FromString(const TCHAR *pszBuffer, int &iValue);
};

class CFarStorage {
public:
	virtual void Get(TCHAR *pszBuffer, int nSize) const = 0;
	virtual void Get(const TCHAR *&ppszBuffer) const;
	virtual void Put(const TCHAR *pszBuffer) = 0;
	virtual bool Verify(const TCHAR *pszBuffer) { return true; }
	virtual operator tstring() const = 0;
};

class CFarTextStorage : public CFarStorage {
public:
	CFarTextStorage():
		m_nMethod(-1) {}
	CFarTextStorage(TCHAR &pchBuffer, bool bReadOnly = false):
		m_nMethod(0), m_pszBuffer(&pchBuffer), m_nSize(1), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(TCHAR *pszBuffer, int nSize = -1, bool bReadOnly = false):
		m_nMethod(0), m_pszBuffer(pszBuffer),
		m_nSize(nSize >= 0 ? nSize : _tcslen(pszBuffer)+1), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(const TCHAR *pszBuffer, int nSize = -1):
		m_nMethod(0), m_pszBuffer((TCHAR *)pszBuffer),
		m_nSize(nSize >= 0 ? nSize : _tcslen(pszBuffer)+1), m_bReadOnly(true) {}
	CFarTextStorage(TCHAR **ppszBuffer, bool bReadOnly = false):
		m_nMethod(1), m_ppszBuffer(ppszBuffer), m_bReadOnly(bReadOnly) {}
	CFarTextStorage(tstring &pstrBuffer, bool bReadOnly = false):
		m_nMethod(2), m_pstrBuffer(&pstrBuffer), m_bReadOnly(bReadOnly) {}

	virtual void Get(TCHAR *pszBuffer, int nSize) const;
	virtual void Put(const TCHAR *pszBuffer);
	virtual operator tstring() const;
protected:
	bool m_bReadOnly;
	int m_nMethod;
	union {
		struct {
			TCHAR *m_pszBuffer;
			int m_nSize;
		};
		TCHAR **m_ppszBuffer;
		tstring *m_pstrBuffer;
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

	CFarIntegerStorage(double *pValue, CFarDoubleConverter *pConverter = &CFarDoubleConverter::Instance):
		m_nMethod(6), m_pDouble(pValue), m_pDConverter(pConverter) {}
	CFarIntegerStorage(double &pValue, CFarDoubleConverter *pConverter = &CFarDoubleConverter::Instance):
		m_nMethod(6), m_pDouble(&pValue), m_pDConverter(pConverter) {}

	virtual void Get(TCHAR *pszBuffer, int nSize) const;
	virtual void Put(const TCHAR *pszBuffer);
	virtual bool Verify(const TCHAR *pszBuffer);
	virtual operator tstring() const;

	virtual int GetI() const;
	virtual double GetD() const;
	virtual void Put(int nValue);
	virtual void Put(double dValue);
protected:
	int m_nMethod;
	union {
		unsigned char *m_pUChar;
		signed char *m_pSChar;
		unsigned short *m_pUShort;
		signed short *m_pSShort;
		unsigned long *m_pULong;
		signed long *m_pSLong;
		double *m_pDouble;
	};
	CFarIntegerConverter *m_pConverter;
	CFarDoubleConverter *m_pDConverter;
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
	tstring Text;
#ifdef UNICODE
public:
	HANDLE	m_hDlg;
	int     m_nItem;
	int     IsSelected() const;
	wstring GetText() const;
	int     SelectedItem() const;
#endif
public:
	int		m_nOwnID;
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
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarStorage *pStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarTextStorage TextStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarIntegerStorage TextStorage,CFarValidator *pValidator=NULL,int iType=DI_EDIT);
	virtual void CreateItem(FarDialogItem *Item);
	virtual bool Validate(FarDialogItem *Item);
	virtual void StoreData(FarDialogItem *Item);
	virtual ~CFarEditItem();
protected:
	int m_iType;
	const TCHAR *m_pszHistory;
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
	tstring NoAmpText(tstring strText);
	int m_nOffset;		// 0th element in list has this number
};

// ********************* VALIDATORS ********************
typedef bool (*FarStringValidator)(const TCHAR *pszData, int nSize, void *pData);

class CFarValidator {
public:
	virtual bool Validate(const TCHAR *pszData, int nSize)=0;
	virtual ~CFarValidator() {}
};

class CFarStringValidator : public CFarValidator {
public:
	CFarStringValidator(FarStringValidator pValidator, void *pData):
		m_pValidator(pValidator), m_pData(pData) {}
	virtual bool Validate(const TCHAR *pszData, int nSize) {return (m_pValidator) ? m_pValidator(pszData, nSize, m_pData) : true;}
protected:
	FarStringValidator m_pValidator;
	void *m_pData;
};

class CFarIntegerRangeValidator : public CFarValidator {
public:
	static const TCHAR *s_szErrorMsg;
	static const TCHAR *s_szHelpTopic;
	const TCHAR *m_szErrorMsg;
	const TCHAR *m_szHelpTopic;

	CFarIntegerRangeValidator(int iMin, int iMax, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_iMin(iMin), m_iMax(iMax), m_pConverter(pConverter), 
		m_szErrorMsg(s_szErrorMsg), m_szHelpTopic(s_szHelpTopic) {}
	virtual bool Validate(const TCHAR *pszData, int nSize);
protected:
	int m_iMin,m_iMax;
	CFarIntegerConverter *m_pConverter;
};

class CFarDetailedRangeValidator : public CFarValidator {
public:
	static const TCHAR *s_szTooBigMsg;
	static const TCHAR *s_szTooSmallMsg;
	static const TCHAR *s_szNotANumberMsg;
	static const TCHAR *s_szTooBigTopic;
	static const TCHAR *s_szTooSmallTopic;
	static const TCHAR *s_szNotANumberTopic;

	CFarDetailedRangeValidator(int iMin,int iMax, CFarIntegerConverter *pConverter = &CFarIntegerConverter::Instance):
		m_iMin(iMin), m_iMax(iMax), m_pConverter(pConverter), 
		m_szTooBigMsg(s_szTooBigMsg), m_szTooSmallMsg(s_szTooSmallMsg),
		m_szNotANumberMsg(s_szNotANumberMsg), m_szTooBigTopic(s_szTooBigTopic),
		m_szTooSmallTopic(s_szTooSmallTopic), m_szNotANumberTopic(s_szNotANumberTopic) {}
	virtual bool Validate(const TCHAR *pszData, int nSize);
protected:
	int m_iMin,m_iMax;
	CFarIntegerConverter *m_pConverter;
public:
	const TCHAR *m_szTooBigMsg;
	const TCHAR *m_szTooSmallMsg;
	const TCHAR *m_szNotANumberMsg;
	const TCHAR *m_szTooBigTopic;
	const TCHAR *m_szTooSmallTopic;
	const TCHAR *m_szNotANumberTopic;
};

#ifndef FAR_NO_NAMESPACE
};
#endif

#endif
