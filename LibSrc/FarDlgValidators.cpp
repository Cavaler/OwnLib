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

void ErrorMessage(const char *Message,const char *HelpTopic) {
	const char *Lines[]={g_pszErrorTitle,Message,g_pszOKButton};
	StartupInfo.Message(StartupInfo.ModuleNumber,FMSG_WARNING,HelpTopic,Lines,3,1);
}

void ErrorMessage(const char *Message1,const char *Message2,const char *HelpTopic) {
	const char *Lines[]={g_pszErrorTitle,Message1,Message2,g_pszOKButton};
	StartupInfo.Message(StartupInfo.ModuleNumber,FMSG_WARNING,HelpTopic,Lines,4,1);
}

// ***************** INTEGER RANGE VALIDATOR *****************

const char *CFarIntegerRangeValidator::s_szErrorMsg=NULL;
const char *CFarIntegerRangeValidator::s_szHelpTopic=NULL;

bool CFarIntegerRangeValidator::Validate(char *pszData, int nSize) {
	int iValue;
	if (!m_pConverter->FromString(pszData, iValue) || (iValue < m_iMin) || (iValue > m_iMax)) {
		char Message[512];
		sprintf(Message,m_szErrorMsg,m_iMin,m_iMax);
		ErrorMessage(Message,m_szHelpTopic);
		return false;
	}
	else return true;
}

const char *CFarDetailedRangeValidator::s_szTooBigMsg=NULL;
const char *CFarDetailedRangeValidator::s_szTooSmallMsg=NULL;
const char *CFarDetailedRangeValidator::s_szNotANumberMsg=NULL;
const char *CFarDetailedRangeValidator::s_szTooBigTopic=NULL;
const char *CFarDetailedRangeValidator::s_szTooSmallTopic=NULL;
const char *CFarDetailedRangeValidator::s_szNotANumberTopic=NULL;

bool CFarDetailedRangeValidator::Validate(char *pszData, int nSize) {
	int iValue;
	char Message[512];

	if (!m_pConverter->FromString(pszData, iValue)) {
		sprintf(Message,m_szNotANumberMsg);
		ErrorMessage(Message,m_szNotANumberTopic);
		return false;
	}
	else if (iValue < m_iMin) {
		sprintf(Message,m_szTooSmallMsg,m_iMin);
		ErrorMessage(Message,m_szTooSmallTopic);
		return false;
	}
	else if (iValue > m_iMax) {
		sprintf(Message,m_szTooBigMsg,m_iMax);
		ErrorMessage(Message,m_szTooBigTopic);
		return false;
	}
	else return true;
}
#define WIN32_LEAN_AND_MEAN

#ifndef FAR_NO_NAMESPACE
};
#endif
