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

// ******************** GENERIC ITEM *******************

void SetItemText(FarDialogItem *pItem, const TCHAR *szText) {
#ifdef UNICODE
	pItem->PtrData = _tcsdup(szText);
	pItem->MaxLen = 0;
#else
	strncpy(pItem->Data, szText, sizeof(pItem->Data));
#endif
}

CFarDialogItem::CFarDialogItem(int iX1,int iY1,int iX2,int iY2,DWORD dwFlags,CFarText szText):
	X1(iX1),Y1(iY1),X2(iX2),Y2(iY2),Flags(dwFlags),Text(szText)
{
}

void CFarDialogItem::CreateItem(FarDialogItem *Item) {
	Item->X1=X1;
	Item->Y1=Y1;
	Item->X2=X2;
	Item->Y2=Y2;
	Item->Focus=FALSE;
	Item->Selected=FALSE;
	Item->Flags=Flags;
	Item->DefaultButton=FALSE;
	SetItemText(Item, Text.c_str());
}

CFarDialogItem::~CFarDialogItem() {
}

// ********************** CUSTOM************************

CFarCustomItem::CFarCustomItem(int iX1,int iY1,int iX2,int iY2,int iSelected,DWORD dwFlags,CFarText szText):
CFarDialogItem(iX1,iY1,iX2,iY2,dwFlags,szText),Selected(iSelected) {}

void CFarCustomItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_USERCONTROL;
	CFarDialogItem::CreateItem(Item);
	Item->Selected=Selected;
}

// ************************ BOX ************************

CFarBoxItem::CFarBoxItem(BOOL bDouble,int iX1,int iY1,int iX2,int iY2,DWORD dwFlags,CFarText szText):
CFarDialogItem(iX1,iY1,iX2,iY2,dwFlags,szText),Double(bDouble) {}

void CFarBoxItem::CreateItem(FarDialogItem *Item) {
	Item->Type=(Double)?DI_DOUBLEBOX:DI_SINGLEBOX;
	CFarDialogItem::CreateItem(Item);
}

// *********************** TEXT ************************

CFarTextItem::CFarTextItem(int X,int Y,DWORD dwFlags,CFarText szText):
CFarDialogItem(X,Y,0,0,dwFlags,szText) {}

void CFarTextItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_TEXT;
	CFarDialogItem::CreateItem(Item);
}

// ********************** BUTTON ***********************

CFarButtonItem::CFarButtonItem(int X,int Y,DWORD dwFlags,BOOL bDefault,CFarText szText):
CFarDialogItem(X,Y,0,0,dwFlags,szText),Default(bDefault) {}

void CFarButtonItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_BUTTON;
	CFarDialogItem::CreateItem(Item);
	Item->DefaultButton=Default;
}

// ********************* CHECKBOX **********************

CFarCheckBoxItem::CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,bool *pbVariable):
CFarDialogItem(X,Y,0,0,dwFlags,szText),m_nMethod(0),m_pbVariable(pbVariable) {}

CFarCheckBoxItem::CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,bool &pbVariable):
CFarDialogItem(X,Y,0,0,dwFlags,szText),m_nMethod(0),m_pbVariable(&pbVariable) {}

CFarCheckBoxItem::CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL *pBVariable):
CFarDialogItem(X,Y,0,0,dwFlags,szText),m_nMethod(1),m_pBVariable(pBVariable) {}

CFarCheckBoxItem::CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL &pBVariable):
CFarDialogItem(X,Y,0,0,dwFlags,szText),m_nMethod(1),m_pBVariable(&pBVariable) {}

CFarCheckBoxItem::CFarCheckBoxItem(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage,int iMaskValue):
CFarDialogItem(X,Y,0,0,dwFlags,szText),m_nMethod(2),m_piStorage(new CFarIntegerStorage(piStorage)),m_iValue(iMaskValue) {}

void CFarCheckBoxItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_CHECKBOX;
	CFarDialogItem::CreateItem(Item);
	switch (m_nMethod) {
	case 0:
		Item->Selected = *m_pbVariable;
		break;
	case 1:
		Item->Selected = *m_pBVariable;
		break;
	case 2:
		Item->Selected = (m_piStorage->GetI() & m_iValue)!=0;
		break;
	}
}

void CFarCheckBoxItem::StoreData(FarDialogItem *Item) {
	switch (m_nMethod) {
	case 0:
		*m_pbVariable = Item->Selected!=0;
		break;
	case 1:
		*m_pBVariable = Item->Selected;
		break;
	case 2:
		if (Item->Selected)
			m_piStorage->Put(m_piStorage->GetI() | m_iValue);
		else
			m_piStorage->Put(m_piStorage->GetI() & ~m_iValue);
		break;
	}
}

CFarCheckBoxItem::~CFarCheckBoxItem() {
	switch (m_nMethod) {
	case 2:
		delete m_piStorage;break;
	}
}

// ********************* CHECKBOX3 **********************

CFarCheckBox3Item::CFarCheckBox3Item(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage):
CFarDialogItem(X,Y,0,0,dwFlags|DIF_3STATE,szText),m_nMethod(1),m_piStorage(new CFarIntegerStorage(piStorage)) {}

CFarCheckBox3Item::CFarCheckBox3Item(int X,int Y,DWORD dwFlags,CFarText szText,DWORD *pdwCleared,DWORD *pdwSet,DWORD dwMaskValue):
CFarDialogItem(X,Y,0,0,dwFlags|DIF_3STATE,szText),m_nMethod(1),m_pdwCleared(pdwCleared),m_pdwSet(pdwSet),m_dwMaskValue(dwMaskValue) {}

void CFarCheckBox3Item::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_CHECKBOX;
	CFarDialogItem::CreateItem(Item);
	switch (m_nMethod) {
	case 0:
		Item->Selected = m_piStorage->GetI();
		break;
	case 1:
		if (*m_pdwSet & m_dwMaskValue)
			Item->Selected = 1;
		else if (*m_pdwCleared & m_dwMaskValue)
			Item->Selected = 0;
		else Item->Selected = 2;
		break;
	}
}

void CFarCheckBox3Item::StoreData(FarDialogItem *Item) {
	switch (m_nMethod) {
	case 0:
		m_piStorage->Put(Item->Selected);
		break;
	case 1:
		switch (Item->Selected) {
		case 0:
			*m_pdwCleared |= m_dwMaskValue;
			*m_pdwSet &= ~m_dwMaskValue;
			break;
		case 1:
			*m_pdwCleared &= ~m_dwMaskValue;
			*m_pdwSet |= m_dwMaskValue;
			break;
		case 2:
			*m_pdwCleared &= ~m_dwMaskValue;
			*m_pdwSet &= ~m_dwMaskValue;
			break;
		}
	}
}

CFarCheckBox3Item::~CFarCheckBox3Item() {
	switch (m_nMethod) {
	case 0:
		delete m_piStorage;break;
	}
}

// ******************** RADIOBUTTON ********************

CFarRadioButtonItem::CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,bool *pbVariable):
CFarCheckBoxItem(X,Y,dwFlags,szText,pbVariable) {}

CFarRadioButtonItem::CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,bool &pbVariable):
CFarCheckBoxItem(X,Y,dwFlags,szText,&pbVariable) {}

CFarRadioButtonItem::CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL *pBVariable):
CFarCheckBoxItem(X,Y,dwFlags,szText,pBVariable) {}

CFarRadioButtonItem::CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,BOOL &pBVariable):
CFarCheckBoxItem(X,Y,dwFlags,szText,&pBVariable) {}

CFarRadioButtonItem::CFarRadioButtonItem(int X,int Y,DWORD dwFlags,CFarText szText,CFarIntegerStorage piStorage,int iValue):
CFarCheckBoxItem(X,Y,dwFlags,szText,piStorage,iValue) {}

void CFarRadioButtonItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_RADIOBUTTON;
	CFarDialogItem::CreateItem(Item);
	switch (m_nMethod) {
	case 0:
		Item->Selected = *m_pbVariable;
		break;
	case 1:
		Item->Selected = *m_pBVariable;
		break;
	case 2:
		Item->Selected = m_piStorage->GetI()==m_iValue;
		break;
	}
}

void CFarRadioButtonItem::StoreData(FarDialogItem *Item) {
	switch (m_nMethod) {
	case 0:
		*m_pbVariable = Item->Selected!=0;
		break;
	case 1:
		*m_pBVariable = Item->Selected;
		break;
	case 2:
		if (Item->Selected) m_piStorage->Put(m_iValue);
		break;
	}
}

// ************************ LIST BOX ***********************

CFarListBoxItem::CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,int *piVariable):
CFarDialogItem(X1,Y1,X2,Y2,dwFlags),m_pData(pData),m_piVariable(piVariable) {}

CFarListBoxItem::CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarStorage *pStorage):
CFarDialogItem(X1,Y1,X2,Y2,dwFlags),m_pData(pData),m_piVariable(NULL),m_pStorage(pStorage) {}

CFarListBoxItem::CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarTextStorage TextStorage):
CFarDialogItem(X1,Y1,X2,Y2,dwFlags),m_pData(pData),m_piVariable(NULL),m_pStorage(new CFarTextStorage(TextStorage)) {}

CFarListBoxItem::CFarListBoxItem(int X1,int Y1,int X2,int Y2,DWORD dwFlags,CFarListData *pData,CFarIntegerStorage IntStorage):
CFarDialogItem(X1,Y1,X2,Y2,dwFlags),m_pData(pData),m_piVariable(NULL),m_pStorage(new CFarIntegerStorage(IntStorage)) {}

void CFarListBoxItem::CreateItem(FarDialogItem *Item) {
	Item->Type=DI_LISTBOX;
	CFarDialogItem::CreateItem(Item);
	Item->ListItems = *m_pData;
	if (m_piVariable) {
		if ((*m_piVariable >= 0) && (*m_piVariable < Item->ListItems->ItemsNumber)) {
			Item->ListItems->Items[*m_piVariable].Flags |= LIF_SELECTED;
		}
	} else {
		tstring strData(*m_pStorage);
		for (int iItem = 0; iItem < Item->ListItems->ItemsNumber; iItem++) {
			if (!_tcscmp(Item->ListItems->Items[iItem].Text, strData.c_str())) {
				Item->ListItems->Items[iItem].Flags |= LIF_SELECTED;
				break;
			}
		}
	}
}

void CFarListBoxItem::StoreData(FarDialogItem *Item) {
	if (m_piVariable) {
		*m_piVariable=Item->ListPos;
	} else {
		if ((Item->ListPos >= 0) && (Item->ListPos < Item->ListItems->ItemsNumber))
			m_pStorage->Put(Item->ListItems->Items[Item->ListPos].Text);
		else
			m_pStorage->Put(_T(""));
	}
}

CFarListBoxItem::~CFarListBoxItem() {
	delete m_pData;
	if (!m_piVariable) delete m_pStorage;
}

// ************************ EDIT ***********************

CFarEditItem::CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarStorage *pStorage,CFarValidator *pValidator,int iType):
CFarDialogItem(iX1,Y,iX2,0,dwFlags),m_pszHistory(szHistory),m_pStorage(pStorage),m_pValidator(pValidator),m_iType(iType) {}

CFarEditItem::CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarTextStorage TextStorage,CFarValidator *pValidator,int iType):
CFarDialogItem(iX1,Y,iX2,0,dwFlags),m_pszHistory(szHistory),m_pStorage(new CFarTextStorage(TextStorage)),m_pValidator(pValidator),m_iType(iType) {}

CFarEditItem::CFarEditItem(int iX1,int Y,int iX2,DWORD dwFlags,const TCHAR *szHistory,CFarIntegerStorage IntStorage,CFarValidator *pValidator,int iType):
CFarDialogItem(iX1,Y,iX2,0,dwFlags),m_pszHistory(szHistory),m_pStorage(new CFarIntegerStorage(IntStorage)),m_pValidator(pValidator),m_iType(iType) {}

void CFarEditItem::CreateItem(FarDialogItem *Item) {
	Item->Type = m_iType;
	CFarDialogItem::CreateItem(Item);
	if (Item->History = m_pszHistory) Item->Flags|=DIF_HISTORY;
#ifdef UNICODE
	m_pStorage->Get(Item->PtrData);
#else
	if (Item->Flags & DIF_VAREDIT) {
		Item->Ptr.PtrFlags = 0;
		Item->Ptr.PtrData = (char *)malloc(Item->Ptr.PtrLength = 65520);
		m_pStorage->Get(Item->Ptr.PtrData, Item->Ptr.PtrLength);
	} else {
		m_pStorage->Get(Item->Data, sizeof(Item->Data));
	}
#endif
}

bool CFarEditItem::Validate(FarDialogItem *Item) {
#ifdef UNICODE
	if (m_pValidator) {
		if (!m_pValidator->Validate(Item->PtrData, _tcslen(Item->PtrData)+1)) return false;
	}
	return m_pStorage->Verify(Item->PtrData);
#else
	if (m_pValidator) {
		if (Item->Flags & DIF_VAREDIT) {
			if (!m_pValidator->Validate(Item->Ptr.PtrData, Item->Ptr.PtrLength)) return false;
		} else {
			if (!m_pValidator->Validate(Item->Data, sizeof(Item->Data))) return false;
		}
	}
	if (Item->Flags & DIF_VAREDIT) {
		return m_pStorage->Verify(Item->Ptr.PtrData);
	} else {
		return m_pStorage->Verify(Item->Data);
	}
#endif
}

void CFarEditItem::StoreData(FarDialogItem *Item) {
#ifdef UNICODE
	m_pStorage->Put(Item->PtrData);
#else
	if (Item->Flags & DIF_VAREDIT) {
		m_pStorage->Put(Item->Ptr.PtrData);
	} else {
		m_pStorage->Put(Item->Data);
	}
#endif
}

CFarEditItem::~CFarEditItem() {
	if (m_pValidator) delete m_pValidator;
	delete m_pStorage;
}

// ************************ COMBO BOX ***********************

CFarComboBoxItem::CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarStorage *pStorage,CFarValidator *pValidator):
CFarEditItem(iX1,Y,iX2,dwFlags|DIF_DROPDOWNLIST,NULL,pStorage,pValidator,DI_COMBOBOX),m_pData(pData) {}

CFarComboBoxItem::CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarTextStorage TextStorage,CFarValidator *pValidator):
CFarEditItem(iX1,Y,iX2,dwFlags|DIF_DROPDOWNLIST,NULL,TextStorage,pValidator,DI_COMBOBOX),m_pData(pData) {}

CFarComboBoxItem::CFarComboBoxItem(int iX1,int Y,int iX2,DWORD dwFlags,CFarListData *pData,CFarIntegerStorage IntStorage,CFarValidator *pValidator,int nOffset):
CFarEditItem(iX1,Y,iX2,dwFlags|DIF_DROPDOWNLIST,NULL,IntStorage,pValidator,DI_COMBOBOX),m_pData(pData),m_nOffset(nOffset) {}

tstring CFarComboBoxItem::NoAmpText(tstring strText)
{
	if ((Flags & DIF_LISTNOAMPERSAND) == 0) return strText;

	int nOff = strText.find('&');
	if (nOff != string::npos) strText.erase(nOff, 1);

	return strText;
}

void CFarComboBoxItem::CreateItem(FarDialogItem *Item) {
	CFarEditItem::CreateItem(Item);
	Item->ListItems = *m_pData;

	for (int iItem = 0; iItem < Item->ListItems->ItemsNumber; iItem++)
		Item->ListItems->Items[iItem].Flags &= ~LIF_SELECTED;

	CFarIntegerStorage *pInt = dynamic_cast<CFarIntegerStorage *>(m_pStorage);
	if (pInt) {
		int nValue = pInt->GetI()-m_nOffset;
		if ((nValue >= 0) && (nValue < Item->ListItems->ItemsNumber)) {
			Item->ListItems->Items[nValue].Flags |= LIF_SELECTED;
			SetItemText(Item, NoAmpText(Item->ListItems->Items[nValue].Text).c_str());
		}
	} else {
		tstring strData(*m_pStorage);
		for (int iItem = 0; iItem < Item->ListItems->ItemsNumber; iItem++) {
			if (!_tcscmp(Item->ListItems->Items[iItem].Text, strData.c_str())) {
				Item->ListItems->Items[iItem].Flags |= LIF_SELECTED;
				break;
			}
		}
	}
}

bool CFarComboBoxItem::Validate(FarDialogItem *Item) {
	if (m_pValidator) {
#ifdef UNICODE
		if (!m_pValidator->Validate(Item->PtrData, _tcslen(Item->PtrData))) return false;
#else
		if (Item->Flags & DIF_VAREDIT) {
			if (!m_pValidator->Validate(Item->Ptr.PtrData, Item->Ptr.PtrLength)) return false;
		} else {
			if (!m_pValidator->Validate(Item->Data, sizeof(Item->Data))) return false;
		}
#endif
	}
	return true;
}

void CFarComboBoxItem::StoreData(FarDialogItem *Item) {
	CFarIntegerStorage *pInt = dynamic_cast<CFarIntegerStorage *>(m_pStorage);
	CFarTextStorage *pTxt = dynamic_cast<CFarTextStorage *>(m_pStorage);
	if (pInt) {
		pInt->Put(Item->ListPos);
/*		pInt->Put(-1);

		FarList *pList = *m_pData;
		for (int nPos = 0; nPos < pList->ItemsNumber; nPos++) {
			if (!strcmp(pList->Items[nPos].Text, Item->Data)) {
				pInt->Put(nPos+m_nOffset);
				break;
			}
		}*/
	} else if (pTxt) {
#ifdef UNICODE
		pTxt->Put(Item->PtrData);
#else
		pTxt->Put(Item->Data);
#endif
	} else {
		TCHAR szText[16];
		_stprintf_s(szText, 16, _T("%d"), Item->ListPos);
		m_pStorage->Put(szText);
	}
}

CFarComboBoxItem::~CFarComboBoxItem() {
	delete m_pData;
}

#ifndef FAR_NO_NAMESPACE
};
#endif
