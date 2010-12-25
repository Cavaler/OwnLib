#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#undef _FAR_NO_NAMELESS_UNIONS
#include <FarDlg.h>

#ifndef FAR_NO_NAMESPACE
namespace FarLib {
#endif

// ************************ DIALOG ************************

CFarDialog::CFarDialog(int iX,int iY,const TCHAR *szHelpTopic,DWORD dwFlags):
X1(-1),Y1(-1),X2(iX),Y2(iY),Focused(0),HelpTopic(szHelpTopic),
m_hDlg(NULL),m_pWindowProc(NULL),m_pCWindowProc(NULL),m_lParam(0),m_dwFlags(dwFlags),m_bUseID(false),m_nCancelID(-1) {}

CFarDialog::CFarDialog(int iX1,int iY1,int iX2,int iY2,const TCHAR *szHelpTopic,DWORD dwFlags):
X1(iX1),Y1(iY1),X2(iX2),Y2(iY2),Focused(0),HelpTopic(szHelpTopic),
m_hDlg(NULL),m_pWindowProc(NULL),m_pCWindowProc(NULL),m_lParam(0),m_dwFlags(dwFlags),m_bUseID(false),m_nCancelID(-1) {}

int CFarDialog::Add(CFarDialogItem *Item) {
	Items.push_back(Item);
	m_mapCodes[Item->m_nOwnID] = Items.size()-1;
	return Items.size()-1;
}

int CFarDialog::Add(CFarDialogItem *Item, int nOwnID) {
	Item->m_nOwnID = nOwnID;
	return Add(Item);
}

int CFarDialog::AddFrame(const TCHAR *Title) {
	int X,Y;
	X=(X1==-1)?X2-4:X2-X1-1;
	Y=(Y1==-1)?Y2-2:Y2-Y1-1;

	return Add(new CFarBoxItem(TRUE,3,1,X,Y,0,Title));
}

int CFarDialog::AddFrame(int TitleId) {return AddFrame(GetMsg(TitleId));}
int CFarDialog::AddFrame() {return AddFrame((TCHAR *)NULL);}

int CFarDialog::AddButton(const TCHAR *szTitle) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,szTitle));
}

int CFarDialog::AddButton(int nId) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,nId));
}

int CFarDialog::AddButtons(const TCHAR *OKTitle,const TCHAR *CancelTitle) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,TRUE,OKTitle));
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,CancelTitle));
}

int CFarDialog::AddButtons(int OKId,int CancelId) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,TRUE,OKId));
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,CancelId));
}

void CFarDialog::SetFocus(int Focus) {
	if (m_hDlg == NULL) {
		//	Design-time
		Focused = (Focus >= 0) ? Index(Focus) : Items.size()+Focus;
	} else {
		//	Run-time
		StartupInfo.SendDlgMessage(m_hDlg, DM_SETFOCUS, Index(Focus), NULL);
	}
}

LONG_PTR WINAPI CFarDialog::s_WindowProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2) {
	static CFarDialog *pDlg = NULL;
	if (Msg == DN_INITDIALOG) pDlg = (CFarDialog *)Param2;

	return pDlg->WindowProc(hDlg, Msg, Param1, Param2);
}

LONG_PTR CFarDialog::WindowProc(HANDLE hDlg, int Msg, int Param1, LONG_PTR Param2) {
	long lResult = 0;

	if (Msg == DN_INITDIALOG) {
		m_hDlg = hDlg;
		Param2 = m_lParam;
	}

	if (m_pCWindowProc) {
		return m_pCWindowProc(this, Msg, Param1, Param2);
	}

	if (m_pWindowProc) {
		return m_pWindowProc(hDlg, Msg, Param1, Param2);
	}

	return StartupInfo.DefDlgProc(hDlg, Msg, Param1, Param2);
}

void CFarDialog::SetWindowProc(FARWINDOWPROC lpWindowProc,long lParam) {
	m_pWindowProc = lpWindowProc;
	m_lParam = lParam;
}

void CFarDialog::SetWindowProc(CFARWINDOWPROC lpCWindowProc,long lParam) {
	m_pCWindowProc = lpCWindowProc;
	m_lParam = lParam;
}

bool CFarDialog::AnyWindowProc()
{
	return m_pWindowProc || m_pCWindowProc;
}

int CFarDialog::Display(int ValidExitCodes,...) {
	vector<FarDialogItem> DialogItems(Items.size());
	int Result;

	for (size_t I=0; I<Items.size(); I++) Items[I]->CreateItem(&DialogItems[I]);
	if (Focused < Items.size()) DialogItems[Focused].Focus=TRUE;

#ifdef UNICODE
	HANDLE hDlg = StartupInfo.DialogInit(StartupInfo.ModuleNumber,X1,Y1,X2,Y2,HelpTopic,&DialogItems[0],Items.size(),0,m_dwFlags,
		AnyWindowProc() ? s_WindowProc : NULL,(long)this);
	for (int nItem = 0; nItem < Items.size(); nItem++) {
		Items[nItem]->m_hDlg = hDlg;
		Items[nItem]->m_nItem = nItem;
	}
#endif

	do {
		int Code=-1;

#ifdef UNICODE
		Result = StartupInfo.DialogRun(hDlg);
#else
		if (AnyWindowProc() || m_dwFlags) {
			Result=StartupInfo.DialogEx(StartupInfo.ModuleNumber,X1,Y1,X2,Y2,HelpTopic,&DialogItems[0],Items.size(),0,m_dwFlags,s_WindowProc,(long)this);
		} else {
			Result=StartupInfo.Dialog(StartupInfo.ModuleNumber,X1,Y1,X2,Y2,HelpTopic,&DialogItems[0],Items.size());
		}
#endif
		if (Result<0) break;

		if (ValidExitCodes==0) {
//			break;
		} else if (ValidExitCodes == -1) {
			if (m_bUseID) {
				int nID = GetID(Result);
				if (nID == m_nCancelID) {Result = -1; break;}
				Result = nID;
			} else {
				if (Result != Items.size()-2) {Result = -1; break;}
				Result = 0;
			}
		} else {
			va_list List;
			va_start(List,ValidExitCodes);
			for (size_t I=0; (int)I < ValidExitCodes; I++) {
				Code=va_arg(List,int);
				if (m_bUseID) {
					if (GetIndex(Code) == Result) {
						Result = Code;
						break;
					}
				} else {
					if (Code<0) Code=Items.size()+Code;
					if (Code==Result) {
						Result = I;
						break;
					}
				}
				Code=-1;
			}
			va_end(List);
			if (Code<0) {Result=-1;break;}	// Bad exit code
		}

		for (size_t I=0;I<Items.size();I++) {
			if (!Items[I]->Validate(&DialogItems[I])) {
				for (size_t J=0; J<Items.size(); J++) DialogItems[J].Focus=FALSE;
				DialogItems[I].Focus=TRUE;
				Result=-1;break;
			}
		}
		if (Result==-1) continue;			// Validation failed

		for (size_t I=0;I<Items.size();I++) Items[I]->StoreData(&DialogItems[I]);
		break;
	} while (TRUE);

#ifdef UNICODE
	StartupInfo.DialogFree(hDlg);

	for (int nItem = 0; nItem < Items.size(); nItem++) {
		if (DialogItems[nItem].PtrData) free((TCHAR *)DialogItems[nItem].PtrData);
	}
#endif

	return Result;
}

HANDLE CFarDialog::hDlg() const
{
	return m_hDlg;
}

void CFarDialog::SetUseID(bool bUseID)
{
	m_bUseID = bUseID;
}

int  CFarDialog::GetID(int nIndex)
{
	if ((nIndex < 0) || (nIndex >= Items.size())) return 0;
	return Items[nIndex]->m_nOwnID;
}

int  CFarDialog::GetIndex(int nID)
{
	return m_mapCodes[nID];
}

bool CFarDialog::HasItem(int nID)
{
	return m_mapCodes.find(nID) != m_mapCodes.end();
}

void CFarDialog::SetCancelID(int nCancelID)
{
	m_bUseID = true;
	m_nCancelID = nCancelID;
}

int CFarDialog::Index(int nIndexOrID)
{
	return (m_bUseID) ? GetIndex(nIndexOrID) : nIndexOrID;
}

void CFarDialog::Close(int nID)
{
	StartupInfo.SendDlgMessage(m_hDlg, DM_CLOSE, Index(nID), 0);
}

LRESULT CFarDialog::DefDlgProc(int nMsg, int nParam1, LONG_PTR lParam2)
{
	return StartupInfo.DefDlgProc(m_hDlg, nMsg, nParam1, lParam2);
}

tstring CFarDialog::GetDlgItemText(int nID)
{
	return FarLib::GetDlgItemText(m_hDlg, Index(nID));
}

void CFarDialog::SetDlgItemText(int nID, const TCHAR *szText)
{
	FarLib::SetDlgItemText(m_hDlg, Index(nID), szText);
}

void CFarDialog::ShowDlgItem(int nID, bool bShow)
{
	FarLib::ShowDlgItem(m_hDlg, Index(nID), bShow);
}

void CFarDialog::EnableDlgItem(int nID, bool bEnable)
{
	FarLib::EnableDlgItem(m_hDlg, Index(nID), bEnable);
}

bool CFarDialog::IsDlgItemChecked(int nID)
{
	return FarLib::IsDlgItemChecked(m_hDlg, Index(nID));
}

void CFarDialog::CheckDlgItem(int nID, bool bCheck)
{
	FarLib::CheckDlgItem(m_hDlg, Index(nID), bCheck);
}

int  CFarDialog::GetCursorPos(int nID)
{
	COORD coord;
	StartupInfo.SendDlgMessage(m_hDlg, DM_GETCURSORPOS, Index(nID), (LONG_PTR)&coord);
	return coord.X;
}

void CFarDialog::SetCursorPos(int nID, int nPos)
{
	COORD coord = {nPos, 0};
	StartupInfo.SendDlgMessage(m_hDlg, DM_SETCURSORPOS, Index(nID), (LONG_PTR)&coord);
}

CFarDialog::~CFarDialog() {
	for (size_t I=0; I<Items.size(); I++) delete Items[I];
}

//////////////////////////////////////////////////////////////////////////

// *********************** HELPERS ***********************

void SetListItemText(FarListItem &Item, const TCHAR *szText) {
	Item.Flags = 0;
	memset(Item.Reserved, 0, sizeof(Item.Reserved));
#ifdef UNICODE
	Item.Text= _tcsdup(szText);
#else
	strncpy(Item.Text, szText, sizeof(Item.Text));
#endif
}

CFarListData::CFarListData() : m_bFree(true), m_pList (new FarList) {
	m_pList->Items = NULL;
	m_pList->ItemsNumber = 0;
}

CFarListData::CFarListData(FarList *pList, bool bCopy) : m_bFree(bCopy) {
	if (bCopy) {
		m_pList = new FarList;
		m_pList->Items = new FarListItem[m_pList->ItemsNumber = pList->ItemsNumber];
		for (int nIndex = 0; nIndex < pList->ItemsNumber; nIndex++)
			m_pList->Items[nIndex] = pList->Items[nIndex];
	} else {
		m_pList = pList;
	}
}

CFarListData::CFarListData(const TCHAR **ppszItems,int iItemCount) : m_bFree(true) {
	m_pList = new FarList;
	m_pList->Items = (FarListItem *)malloc((m_pList->ItemsNumber = iItemCount) * sizeof(FarListItem));
	for (int nIndex = 0; nIndex < iItemCount; nIndex++) {
		SetListItemText(m_pList->Items[nIndex], ppszItems[nIndex]);
	}
}

CFarListData::CFarListData(const vector<CFarText> arrItems) : m_bFree(true) {
	m_pList = new FarList;
	m_pList->Items = (FarListItem *)malloc((m_pList->ItemsNumber = arrItems.size()) * sizeof(FarListItem));
	for (int nIndex = 0; nIndex < (int)arrItems.size(); nIndex++) {
		SetListItemText(m_pList->Items[nIndex], arrItems[nIndex]);
	}
}

int CFarListData::Append(const TCHAR *szItem) {
	if (!m_bFree) return -1;
	m_pList->Items = (FarListItem *)realloc(m_pList->Items, (m_pList->ItemsNumber + 1) * sizeof(FarListItem));
	SetListItemText(m_pList->Items[m_pList->ItemsNumber], szItem);
	return m_pList->ItemsNumber++;
}

CFarListData::~CFarListData() {
	if (m_bFree) {
#ifdef UNICODE
		for (int nItem = 0; nItem < m_pList->ItemsNumber; nItem++)
			free((TCHAR *)m_pList->Items[nItem].Text);
#endif
		free(m_pList->Items);
		delete m_pList;
	}
}

CFarIntegerConverter CFarIntegerConverter::Instance;

void CFarIntegerConverter::ToString(int iValue, TCHAR *pszBuffer, int nSize) {
	_sntprintf(pszBuffer, nSize, _T("%d"), iValue);
}

bool CFarIntegerConverter::FromString(const TCHAR *pszBuffer, int &iValue) {
	return _stscanf(pszBuffer, _T("%d"), &iValue) > 0;
}

CFarDoubleConverter CFarDoubleConverter::Instance;
CFarDoubleConverter CFarDoubleConverter::Instance0(0);
CFarDoubleConverter CFarDoubleConverter::Instance1(1);
CFarDoubleConverter CFarDoubleConverter::Instance2(2);

void CFarDoubleConverter::ToString(double dValue, TCHAR *pszBuffer, int nSize) {
	if (m_nDigits >= 0) {
		_sntprintf(pszBuffer, nSize, _T("%.*f"), m_nDigits, dValue);
	} else {
		_sntprintf(pszBuffer, nSize, _T("%f"), dValue);
	}
}

bool CFarDoubleConverter::FromString(const TCHAR *pszBuffer, double &dValue) {
	return _stscanf(pszBuffer, _T("%df"), &dValue) > 0;
}

CFarHexConverter CFarHexConverter::Instance;

void CFarHexConverter::ToString(int iValue, TCHAR *pszBuffer, int nSize) {
	_sntprintf(pszBuffer, nSize, _T("%X"), iValue);
}

bool CFarHexConverter::FromString(const TCHAR *pszBuffer, int &iValue) {
	return _stscanf(pszBuffer, _T("%x"), &iValue) > 0;
}

CFarHexDnConverter CFarHexDnConverter::Instance;

void CFarHexDnConverter::ToString(int iValue, TCHAR *pszBuffer, int nSize) {
	_sntprintf(pszBuffer, nSize, _T("%x"), iValue);
}

CFarSizeConverter CFarSizeConverter::Instance;

void CFarSizeConverter::ToString(int iValue, TCHAR *pszBuffer, int nSize) {
	if ((iValue >= 1024) && (iValue%256 == 0)) {
		if ((iValue >= 1024*1024) && (iValue % (256*1024) == 0)) {
			if ((iValue >= 1024*1024*1024) && (iValue % (256*1024*1024) == 0))
				_sntprintf(pszBuffer, nSize, _T("%.2f Gb"), (float)iValue/(1024*1024*1024));
			else
				_sntprintf(pszBuffer, nSize, _T("%.2f Mb"), (float)iValue/(1024*1024));
		} else
			_sntprintf(pszBuffer, nSize, _T("%.2f Kb"), (float)iValue/1024);
	} else
		_sntprintf(pszBuffer, nSize, _T("%d"), iValue);
}

bool CFarSizeConverter::FromString(const TCHAR *pszBuffer, int &iValue) {
	float fValue;
	TCHAR szUnit[32];

	if (_stscanf(pszBuffer, _T("%f%2s"), &fValue, szUnit) == 0) return false;
	if (toupper(szUnit[0] == 'K')) fValue *= 1024; else
	if (toupper(szUnit[0] == 'M')) fValue *= 1024*1024; else
	if (toupper(szUnit[0] == 'G')) fValue *= 1024*1024*1024;

	iValue = (int)fValue;
	return true;
}

void CFarStorage::Get(const TCHAR *&ppszBuffer) const {
	vector<TCHAR> arrBuf(65535);
	Get(&arrBuf[0], arrBuf.size());
	ppszBuffer = _tcsdup(&arrBuf[0]);
}

void CFarTextStorage::Get(TCHAR *pszBuffer, int nSize) const {
	switch (m_nMethod) {
	case 0:
		if (m_nSize < nSize) {
			_tcsncpy(pszBuffer, m_pszBuffer, m_nSize);
			pszBuffer[m_nSize] = 0;
		} else
			_tcsncpy(pszBuffer, m_pszBuffer, nSize);
		break;
	case 1:
		if (*m_ppszBuffer)
			_tcsncpy(pszBuffer, *m_ppszBuffer, nSize);
		else
			pszBuffer[0] = 0;
		break;
	case 2:
		_tcsncpy(pszBuffer, m_pstrBuffer->c_str(), nSize);break;
	}
}

void CFarTextStorage::Put(const TCHAR *pszBuffer) {
	if (m_bReadOnly) return;
	switch (m_nMethod) {
	case 0:
		_tcsncpy(m_pszBuffer, pszBuffer, m_nSize);break;
	case 1:
		*m_ppszBuffer = (TCHAR *)realloc(*m_ppszBuffer, (_tcslen(pszBuffer)+1)*sizeof(TCHAR));
		_tcscpy(*m_ppszBuffer, pszBuffer);
		break;
	case 2:
		(*m_pstrBuffer) = pszBuffer;break;
	}
}

CFarTextStorage::operator tstring() const {
	switch (m_nMethod) {
	case 0:
		return tstring(m_pszBuffer, m_nSize);
	case 1:
		return tstring(*m_ppszBuffer);
	case 2:
		return *m_pstrBuffer;
	default:
		return _T("");
	}
}

int  CFarIntegerStorage::GetI() const {
	switch (m_nMethod) {
	case 0:
		return *m_pUChar;
	case 1:
		return *m_pSChar;
	case 2:
		return *m_pUShort;
	case 3:
		return *m_pSShort;
	case 4:
		return *m_pULong;
	case 5:
		return *m_pSLong;
	default:
		return 0;
	}
}

double CFarIntegerStorage::GetD() const {
	switch (m_nMethod) {
	case 6:
		return *m_pDouble;
	default:
		return 0;
	}
}

void CFarIntegerStorage::Get(TCHAR *pszBuffer, int nSize) const {
	if (m_nMethod <= 5) {
		m_pConverter->ToString(GetI(), pszBuffer, nSize);
	} else {
		m_pDConverter->ToString(GetD(), pszBuffer, nSize);
	}
}

void CFarIntegerStorage::Put(int nValue) {
	switch (m_nMethod) {
	case 0:
		*m_pUChar = nValue;break;
	case 1:
		*m_pSChar = nValue;break;
	case 2:
		*m_pUShort = nValue;break;
	case 3:
		*m_pSShort = nValue;break;
	case 4:
		*m_pULong = nValue;break;
	case 5:
		*m_pSLong = nValue;break;
	}
}

void CFarIntegerStorage::Put(double dValue) {
	switch (m_nMethod) {
	case 6:
		*m_pDouble = dValue;break;
	}
}

void CFarIntegerStorage::Put(const TCHAR *pszBuffer) {
	if (m_nMethod <= 5) {
		int nValue;
		m_pConverter->FromString(pszBuffer, nValue);
		Put(nValue);
	} else {
		double dValue;
		m_pDConverter->FromString(pszBuffer, dValue);
		Put(dValue);
	}
}

bool CFarIntegerStorage::Verify(const TCHAR *pszBuffer) {
	if (m_nMethod <= 5) {
		int nValue;
		return m_pConverter->FromString(pszBuffer, nValue);
	} else {
		double dValue;
		return m_pDConverter->FromString(pszBuffer, dValue);
	}
}

CFarIntegerStorage::operator tstring() const {
	TCHAR szBuffer[64];
	Get(szBuffer, sizeof(szBuffer));
	return szBuffer;
}

#ifndef FAR_NO_NAMESPACE
};
#endif
