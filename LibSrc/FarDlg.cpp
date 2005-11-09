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

CFarDialog::CFarDialog(int iX,int iY,const char *szHelpTopic):
X1(-1),Y1(-1),X2(iX),Y2(iY),Focused(0),HelpTopic(szHelpTopic),
Items(NULL),ItemsNumber(0),m_pHandler(NULL),m_lParam(0),m_bHandled(false) {}

CFarDialog::CFarDialog(int iX1,int iY1,int iX2,int iY2,const char *szHelpTopic):
X1(iX1),Y1(iY1),X2(iX2),Y2(iY2),Focused(0),HelpTopic(szHelpTopic),
Items(NULL),ItemsNumber(0),m_pHandler(NULL),m_lParam(0),m_bHandled(false) {}

int CFarDialog::Add(CFarDialogItem *Item) {
	Items=(CFarDialogItem **)realloc(Items,(ItemsNumber+1)*sizeof(CFarDialogItem *));
	Items[ItemsNumber]=Item;
	m_arrItemHandlers.push_back(NULL);
	return ItemsNumber++;
}

int CFarDialog::Add(CFarDialogItem *Item,CFarEventHandler *pHandler) {
	Items=(CFarDialogItem **)realloc(Items,(ItemsNumber+1)*sizeof(CFarDialogItem *));
	Items[ItemsNumber]=Item;
	m_arrItemHandlers.push_back(pHandler);
	m_bHandled |= (pHandler!=NULL);
	return ItemsNumber++;
}

int CFarDialog::AddFrame(const char *Title) {
	int X,Y;
	X=(X1==-1)?X2-4:X2-X1-1;
	Y=(Y1==-1)?Y2-2:Y2-Y1-1;

	return Add(new CFarBoxItem(TRUE,3,1,X,Y,0,Title));
}

int CFarDialog::AddFrame(int TitleId) {return AddFrame(GetMsg(TitleId));}
int CFarDialog::AddFrame() {return AddFrame((char *)NULL);}

int CFarDialog::AddButton(const char *szTitle) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,szTitle));
}

int CFarDialog::AddButton(int nId) {
	int Y=(Y1==-1)?Y2-4:Y2-Y1-3;
	return Add(new CFarButtonItem(0,Y,DIF_CENTERGROUP,FALSE,nId));
}

int CFarDialog::AddButtons(const char *OKTitle,const char *CancelTitle) {
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
	if (Focus>=0) Focused=Focus; else Focused=ItemsNumber+Focus;
}

long WINAPI CFarDialog::s_WindowProc(HANDLE hDlg, int Msg, int Param1, long Param2) {
	static CFarDialog *pDlg = NULL;
	if (Msg == DN_INITDIALOG) {
		pDlg = (CFarDialog *)Param2;
		Param2 = pDlg->m_lParam;
	}
	return pDlg->WindowProc(hDlg, Msg, Param1, Param2);
}

int  CFarDialog::WindowProc(HANDLE hDlg, int Msg, int Param1, long Param2) {
	long lResult = 0;

	if (m_pHandler && m_pHandler->ProcessChain(hDlg, Msg, Param1, Param2, lResult))
		return lResult;

	if ((Param1 >= 0) && (Param1 < (int)m_arrItemHandlers.size()) &&
		m_arrItemHandlers[Param1] && m_arrItemHandlers[Param1]->ProcessChain(hDlg, Msg, Param1, Param2, lResult))
		return lResult;

	return StartupInfo.DefDlgProc(hDlg, Msg, Param1, Param2);
}

void CFarDialog::SetWindowProc(FARWINDOWPROC lpWindowProc,long lParam) {
	SetHandler(new CFarWindowProcHandler(lpWindowProc, FWP_ASRESULT, m_pHandler), lParam);
}

void CFarDialog::SetHandler(CFarEventHandler *pHandler,long lParam) {
	if (m_pHandler)
		m_pHandler->m_pNextHandler = pHandler;
	else
		m_pHandler = pHandler;
	m_bHandled |= (pHandler!=NULL);
	m_lParam = lParam;
}

int CFarDialog::Display(int ValidExitCodes,...) {
	FarDialogItem *DialogItems=new FarDialogItem[ItemsNumber];
	int Result;

	for (int I=0;I<ItemsNumber;I++) Items[I]->CreateItem(&DialogItems[I]);
	if (Focused<ItemsNumber) DialogItems[Focused].Focus=TRUE;

	do {
		int I,Code=-1;

		if (m_bHandled) {
			Result=StartupInfo.DialogEx(StartupInfo.ModuleNumber,X1,Y1,X2,Y2,HelpTopic,DialogItems,ItemsNumber,0,0,s_WindowProc,(long)this);
		} else {
			Result=StartupInfo.Dialog(StartupInfo.ModuleNumber,X1,Y1,X2,Y2,HelpTopic,DialogItems,ItemsNumber);
		}
		if (Result<0) break;

		if (ValidExitCodes==-1) {
			if (Result!=ItemsNumber-2) break;
			Result=0;
		} else {
			va_list List;
			va_start(List,ValidExitCodes);
			for (I=0;I<ValidExitCodes;I++) {
				Code=va_arg(List,int);
				if (Code<0) Code=ItemsNumber+Code;
				if (Code==Result) {Result=I;break;} else Code=-1;
			}
			va_end(List);
			if (Code<0) {Result=-1;break;}	// Bad exit code
		}

		for (I=0;I<ItemsNumber;I++) {
			if (!Items[I]->Validate(&DialogItems[I])) {
				for (int J=0;J<ItemsNumber;J++) DialogItems[J].Focus=FALSE;
				DialogItems[I].Focus=TRUE;
				Result=-1;break;
			}
		}
		if (Result==-1) continue;			// Validation failed

		for (I=0;I<ItemsNumber;I++) Items[I]->StoreData(&DialogItems[I]);
		break;
	} while (TRUE);

	delete[] DialogItems;
	return Result;
}

CFarDialog::~CFarDialog() {
	if (Items) {
		for (int I=0;I<ItemsNumber;I++) delete Items[I];
		free(Items);
	}
}

// *********************** HELPERS ***********************

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

CFarListData::CFarListData(const char **ppszItems,int iItemCount) : m_bFree(true) {
	m_pList = new FarList;
	m_pList->Items = (FarListItem *)malloc((m_pList->ItemsNumber = iItemCount) * sizeof(FarListItem));
	for (int nIndex = 0; nIndex < iItemCount; nIndex++) {
		m_pList->Items[nIndex].Flags = 0;
		memset(m_pList->Items[nIndex].Reserved, 0, sizeof(m_pList->Items[nIndex].Reserved));
		strncpy(m_pList->Items[nIndex].Text, ppszItems[nIndex], sizeof(m_pList->Items[nIndex].Text));
	}
}

CFarListData::CFarListData(const vector<CFarText> arrItems) : m_bFree(true) {
	m_pList = new FarList;
	m_pList->Items = (FarListItem *)malloc((m_pList->ItemsNumber = arrItems.size()) * sizeof(FarListItem));
	for (int nIndex = 0; nIndex < (int)arrItems.size(); nIndex++) {
		m_pList->Items[nIndex].Flags = 0;
		memset(m_pList->Items[nIndex].Reserved, 0, sizeof(m_pList->Items[nIndex].Reserved));
		strncpy(m_pList->Items[nIndex].Text, arrItems[nIndex], sizeof(m_pList->Items[nIndex].Text));
	}
}

void CFarListData::Append(const char *szItem) {
	if (!m_bFree) return;
	m_pList->Items = (FarListItem *)realloc(m_pList->Items, (m_pList->ItemsNumber + 1) * sizeof(FarListItem));
	m_pList->Items[m_pList->ItemsNumber].Flags = 0;
	memset(m_pList->Items[m_pList->ItemsNumber].Reserved, 0, sizeof(m_pList->Items[m_pList->ItemsNumber].Reserved));
	strncpy(m_pList->Items[m_pList->ItemsNumber].Text, szItem, sizeof(m_pList->Items[m_pList->ItemsNumber].Text));
	m_pList->ItemsNumber++;
}

CFarListData::~CFarListData() {
	if (m_bFree) {
		free(m_pList->Items);
		delete m_pList;
	}
}

CFarIntegerConverter CFarIntegerConverter::Instance;

void CFarIntegerConverter::ToString(int iValue, char *pszBuffer, int nSize) {
	_snprintf(pszBuffer, nSize, "%d", iValue);
}

bool CFarIntegerConverter::FromString(const char *pszBuffer, int &iValue) {
	return sscanf(pszBuffer, "%d", &iValue) > 0;
}

CFarHexConverter CFarHexConverter::Instance;

void CFarHexConverter::ToString(int iValue, char *pszBuffer, int nSize) {
	_snprintf(pszBuffer, nSize, "%X", iValue);
}

bool CFarHexConverter::FromString(const char *pszBuffer, int &iValue) {
	return sscanf(pszBuffer, "%x", &iValue) > 0;
}

CFarHexDnConverter CFarHexDnConverter::Instance;

void CFarHexDnConverter::ToString(int iValue, char *pszBuffer, int nSize) {
	_snprintf(pszBuffer, nSize, "%x", iValue);
}

CFarSizeConverter CFarSizeConverter::Instance;

void CFarSizeConverter::ToString(int iValue, char *pszBuffer, int nSize) {
	if ((iValue >= 1024) && (iValue%256 == 0)) {
		if ((iValue >= 1024*1024) && (iValue % (256*1024) == 0)) {
			if ((iValue >= 1024*1024*1024) && (iValue % (256*1024*1024) == 0))
				_snprintf(pszBuffer, nSize, "%f Gb", (float)iValue/(1024*1024*1024));
			else
				_snprintf(pszBuffer, nSize, "%f Mb", (float)iValue/(1024*1024));
		} else
			_snprintf(pszBuffer, nSize, "%f Kb", (float)iValue/1024);
	} else
		_snprintf(pszBuffer, nSize, "%d", iValue);
}

bool CFarSizeConverter::FromString(const char *pszBuffer, int &iValue) {
	float fValue;
	char szUnit[4];

	if (sscanf(pszBuffer, "%f%2s", &fValue, szUnit) == 0) return false;
	if (toupper(szUnit[0] == 'K')) fValue *= 1024; else
	if (toupper(szUnit[0] == 'M')) fValue *= 1024*1024; else
	if (toupper(szUnit[0] == 'G')) fValue *= 1024*1024*1024;

	iValue = (int)fValue;
	return true;
}

void CFarTextStorage::Get(char *pszBuffer, int nSize) const {
	switch (m_nMethod) {
	case 0:
		if (m_nSize < nSize) {
			strncpy(pszBuffer, m_pszBuffer, m_nSize);
			pszBuffer[m_nSize] = 0;
		} else
			strncpy(pszBuffer, m_pszBuffer, nSize);
		break;
	case 1:
		if (*m_ppszBuffer)
			strncpy(pszBuffer, *m_ppszBuffer, nSize);
		else
			pszBuffer[0] = 0;
		break;
	case 2:
		strncpy(pszBuffer, m_pstrBuffer->c_str(), nSize);break;
	}
}

void CFarTextStorage::Put(const char *pszBuffer) {
	if (m_bReadOnly) return;
	switch (m_nMethod) {
	case 0:
		strncpy(m_pszBuffer, pszBuffer, m_nSize);break;
	case 1:
		*m_ppszBuffer = (char *)realloc(*m_ppszBuffer, strlen(pszBuffer)+1);
		strcpy(*m_ppszBuffer, pszBuffer);
		break;
	case 2:
		(*m_pstrBuffer) = pszBuffer;break;
	}
}

CFarTextStorage::operator string() const {
	switch (m_nMethod) {
	case 0:
		return string(m_pszBuffer, m_nSize);
	case 1:
		return string(*m_ppszBuffer);
	case 2:
		return *m_pstrBuffer;
	default:
		return "";
	}
}

int  CFarIntegerStorage::Get() const {
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

void CFarIntegerStorage::Get(char *pszBuffer, int nSize) const {
	m_pConverter->ToString(Get(), pszBuffer, nSize);
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

void CFarIntegerStorage::Put(const char *pszBuffer) {
	int nValue;
	m_pConverter->FromString(pszBuffer, nValue);
	Put(nValue);
}

bool CFarIntegerStorage::Verify(const char *pszBuffer) {
	int nValue;
	return m_pConverter->FromString(pszBuffer, nValue);
}

CFarIntegerStorage::operator string() const {
	char szBuffer[64];
	Get(szBuffer, sizeof(szBuffer));
	return szBuffer;
}

CFarEventHandler::CFarEventHandler(CFarEventHandler *pNextHandler) :
m_pNextHandler(pNextHandler) {}

bool CFarEventHandler::ProcessChain(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result) {
	if (Process(hDlg, Msg, Param1, Param2, Result))
		return true;
	return m_pNextHandler && m_pNextHandler->ProcessChain(hDlg, Msg, Param1, Param2, Result);
}

CFarEventHandler::~CFarEventHandler() {
	if (m_pNextHandler)
		delete m_pNextHandler;
}

CFarWindowProcHandler::CFarWindowProcHandler(FARWINDOWPROC pProc, int nResult, CFarEventHandler *pNextHandler) :
CFarEventHandler(pNextHandler), m_pProc(pProc), m_nResult(nResult) {}

bool CFarWindowProcHandler::Process(HANDLE hDlg, int Msg, int Param1, long Param2, long &Result) {
	long lResult = m_pProc(hDlg, Msg, Param1, Param2);
	switch (m_nResult) {
	case FWP_ASRESULT:
		if (lResult) {
			Result = lResult;
			return true;
		}
	case FWP_FALSE:
		return false;
	default:
		Result = lResult;
		return true;
	}
}

#ifndef FAR_NO_NAMESPACE
};
#endif
