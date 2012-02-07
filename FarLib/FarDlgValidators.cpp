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

void ErrorMessage(const TCHAR *Message,const TCHAR *HelpTopic) {
	const TCHAR *Lines[]={g_pszErrorTitle,Message,g_pszOKButton};
	StartupInfo.Message(StartupInfo.ModuleNumber,FMSG_WARNING,HelpTopic,Lines,3,1);
}

void ErrorMessage(const TCHAR *Message1,const TCHAR *Message2,const TCHAR *HelpTopic) {
	const TCHAR *Lines[]={g_pszErrorTitle,Message1,Message2,g_pszOKButton};
	StartupInfo.Message(StartupInfo.ModuleNumber,FMSG_WARNING,HelpTopic,Lines,4,1);
}

// ***************** INTEGER RANGE VALIDATOR *****************

const TCHAR *CFarIntegerRangeValidator::s_szErrorMsg=NULL;
const TCHAR *CFarIntegerRangeValidator::s_szHelpTopic=NULL;

bool CFarIntegerRangeValidator::Validate(const TCHAR *pszData, int nSize) {
	int iValue;
	if (!m_pConverter->FromString(pszData, iValue) || (iValue < m_iMin) || (iValue > m_iMax)) {
		TCHAR Message[512];
		_stprintf_s(Message, sizeof(Message), m_szErrorMsg, m_iMin, m_iMax);
		ErrorMessage(Message,m_szHelpTopic);
		return false;
	}
	else return true;
}

const TCHAR *CFarDetailedRangeValidator::s_szTooBigMsg=NULL;
const TCHAR *CFarDetailedRangeValidator::s_szTooSmallMsg=NULL;
const TCHAR *CFarDetailedRangeValidator::s_szNotANumberMsg=NULL;
const TCHAR *CFarDetailedRangeValidator::s_szTooBigTopic=NULL;
const TCHAR *CFarDetailedRangeValidator::s_szTooSmallTopic=NULL;
const TCHAR *CFarDetailedRangeValidator::s_szNotANumberTopic=NULL;

bool CFarDetailedRangeValidator::Validate(const TCHAR *pszData, int nSize) {
	int iValue;
	TCHAR Message[512];

	if (!m_pConverter->FromString(pszData, iValue)) {
		_stprintf_s(Message, sizeof(Message), m_szNotANumberMsg);
		ErrorMessage(Message,m_szNotANumberTopic);
		return false;
	}
	else if (iValue < m_iMin) {
		_stprintf_s(Message, sizeof(Message), m_szTooSmallMsg, m_iMin);
		ErrorMessage(Message,m_szTooSmallTopic);
		return false;
	}
	else if (iValue > m_iMax) {
		_stprintf_s(Message, sizeof(Message), m_szTooBigMsg, m_iMax);
		ErrorMessage(Message,m_szTooBigTopic);
		return false;
	}
	else return true;
}
#define WIN32_LEAN_AND_MEAN

#ifndef FAR_NO_NAMESPACE
};
#endif
