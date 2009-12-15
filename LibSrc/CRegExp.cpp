#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <CRegExp.h>
#include <StringEx.h>
#include <map>

#ifdef UNICODE

static wstring g_wstrError;

const wchar_t *WError(const char *szErrorPtr) {
	if (szErrorPtr) {
		g_wstrError = ANSIToUnicode(szErrorPtr);
		return g_wstrError.c_str();
	} else {
		return NULL;
	}
}

pcre * pcre_compile(const wchar_t *pattern, int options, const wchar_t **errorptr, int *erroroffset, const unsigned char *tables) {
	string szPattern = UTF8FromUnicode(pattern);

	const char *szErrorPtr;
	pcre *re = pcre_compile(szPattern.c_str(), options | PCRE_UTF8, &szErrorPtr, erroroffset, tables);
	*errorptr = WError(szErrorPtr);

	return re;
}

pcre_extra * pcre_study(const pcre *external_re, int options, const wchar_t **errorptr) {
	const char *szErrorPtr;
	pcre_extra *re = pcre_study(external_re, options, &szErrorPtr);
	*errorptr = WError(szErrorPtr);

	return re;
}

int pcre_exec(const pcre *argument_re, const pcre_extra *extra_data,
			  const wchar_t *subject, int length, int start_offset, int options, int *offsets,
			  int offsetcount) {

	string szSubject = UTF8FromUnicode(wstring(subject, length));

	std::map<int, int> utf2char;
	std::map<int, int> char2utf;
	utf2char[-1] = -1;	// For nonexistent matches
	int nByte = 0, nChar = 0;
	for (; nByte < szSubject.length(); ) {
		utf2char[nByte] = nChar;
		char2utf[nChar] = nByte;
		char c = szSubject[nByte];
		if		((c & 0x80) == 0x00) nByte += 1;		//	0xxxxxxx
		else if ((c & 0xE0) == 0xC0) nByte += 2;		//	110xxxxx
		else if ((c & 0xF0) == 0xE0) nByte += 3;		//	1110xxxx
		else if ((c & 0xF8) == 0xF0) nByte += 4;		//	11110xxx
		else nByte += 1;
		nChar++;
	}
	utf2char[nByte] = nChar;
	char2utf[nChar] = nByte;

	int nResult = pcre_exec(argument_re, extra_data, szSubject.c_str(), szSubject.length(), char2utf[start_offset], options, offsets, offsetcount);

	if ((nResult >= 0) && offsets && offsetcount) {
		for (int nMatch = 0; nMatch < nResult; nMatch++) {
			offsets[nMatch*2  ] = utf2char[offsets[nMatch*2  ]];
			offsets[nMatch*2+1] = utf2char[offsets[nMatch*2+1]];
		}
	}

	return nResult;
}

#endif

CRegExp::CRegExp() : m_pPattern(NULL), m_pPatternExtra(NULL), m_piRefs(NULL), m_iRefCount(0) {
}

CRegExp::CRegExp(tstring strPattern, int iCompileFlags) : m_pPattern(NULL), m_pPatternExtra(NULL), m_piRefs(NULL) {
	Compile(strPattern, iCompileFlags);
}

bool CRegExp::Compile(tstring strPattern, int iCompileFlags) {
	Release();

	m_pPattern = pcre_compile(strPattern.c_str(), iCompileFlags, &m_pszErrPtr, &m_iErrOffset,NULL);
	if (!m_pPattern) return false;
	m_pPatternExtra = pcre_study(m_pPattern, 0, &m_pszErrPtr);

	m_iRefCount= pcre_info(m_pPattern, NULL, NULL) + 1;
	m_piRefs = new int[m_iRefCount*3];

	return true;
}

void CRegExp::Release() {
	if (m_piRefs) {delete[] m_piRefs; m_piRefs = NULL;}
	if (m_pPattern) {pcre_free(m_pPattern); m_pPattern = NULL;}
	if (m_pPatternExtra) {pcre_free(m_pPatternExtra); m_pPatternExtra = NULL;}
	m_iRefCount = 0;
}

CRegExp::~CRegExp() {
	Release();
}

void CRegExp::FillReferences(tstring &strAnalyze, int iCount, vector<tstring> *arrReferences) {
	arrReferences->clear();
	for (int i = 0; i < iCount; i++) {
		if (m_piRefs[i*2] >= 0)
			arrReferences->push_back(tstring(strAnalyze, m_piRefs[i*2], m_piRefs[i*2+1] - m_piRefs[i*2]));
		else
			arrReferences->push_back(tstring());
	}
}

bool CRegExp::Match(tstring strAnalyze, int iExecFlags, vector<tstring> *arrReferences) {
	int iResult = pcre_exec(m_pPattern, m_pPatternExtra, strAnalyze.data(), strAnalyze.size(), 0, iExecFlags, m_piRefs, m_iRefCount*3);
	if (iResult < 0)
	{
		return false;
	}
	if (arrReferences)
		FillReferences(strAnalyze, iResult, arrReferences);
	return true;
}

enum ECaseConvert {CCV_NONE,CCV_UPPER,CCV_LOWER,CCV_FLIP};

TCHAR CRegExp::ConvertCase(TCHAR C) {
	if (OneCaseConvert==CCV_NONE) return C;

#ifndef UNICODE
	TCHAR Ansi,Oem;
	OemToCharBuff(&C,&Ansi,1);
	CharToOemBuff(&Ansi,&Oem,1);
	if (Oem!=C) return C;
#else
	TCHAR Ansi = C;
#endif

	switch (OneCaseConvert) {
	case CCV_UPPER:
		Ansi=(TCHAR)CharUpper((TCHAR *)(UTCHAR)Ansi);break;
	case CCV_LOWER:
		Ansi=(TCHAR)CharLower((TCHAR *)(UTCHAR)Ansi);break;
	case CCV_FLIP:{
		TCHAR Lower=(TCHAR)CharLower((TCHAR *)(UTCHAR)Ansi);
		Ansi=(Lower==Ansi)?(TCHAR)CharUpper((TCHAR *)(UTCHAR)Ansi):Lower;
		break;
				  }
	}
	OneCaseConvert=CaseConvert;
#ifdef UNICODE
	return Ansi;
#else
	CharToOemBuffA(&Ansi,&C,1);
	return C;
#endif
}

void CRegExp::AddChar(tstring &strResult, TCHAR C) {
	strResult += ConvertCase(C);
}

tstring CRegExp::CreateReplaceString(tstring &strReplacePattern, vector<tstring> &arrReferences)
{
	tstring strResult = _T("");

	const TCHAR *pszReplace = strReplacePattern.c_str();
	CaseConvert = CCV_NONE;
	OneCaseConvert = CCV_NONE;

	while (*pszReplace) {
		if (OneCaseConvert==CCV_NONE) OneCaseConvert=CaseConvert;
		switch (*pszReplace) {
		case '\\':
			pszReplace++;
			switch (*pszReplace) {
			case 0:strResult += '\\';pszReplace--;break;
			case 'a':strResult += '\a';break;
			case 'b':strResult += '\b';break;
			case 'e':strResult += '\x1B';break;
			case 'f':strResult += '\f';break;
			case 'n':strResult += '\n';break;
			case 'r':strResult += '\r';break;
			case 't':strResult += '\t';break;
			case 'v':strResult += '\v';break;

			case '0':case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9':{
				int iChar=0;
				for (int I=0;I<3;I++) {
					if (isdigit((UTCHAR)*pszReplace)) {
						iChar=iChar*8+(*pszReplace-'0');pszReplace++;
					} else break;
				}
				pszReplace--;AddChar(strResult, iChar);
					 }
			case 'x':{
				int iChar=0;pszReplace++;
				for (int I=0;I<2;I++) {
					if (isxdigit((UTCHAR)*pszReplace)) {
						iChar=iChar*16+*pszReplace;
						if (*pszReplace<='9') iChar-='0'; else
						if (*pszReplace<='F') iChar+=10-'A'; else iChar+=10-'a';
						pszReplace++;
					} else break;
				}
				pszReplace--;AddChar(strResult, iChar);break;
					 }

			case 'l':OneCaseConvert=CCV_LOWER;break;
			case 'u':OneCaseConvert=CCV_UPPER;break;
			case 'c':OneCaseConvert=CCV_FLIP;break;
			case 'L':CaseConvert=OneCaseConvert=CCV_LOWER;break;
			case 'U':CaseConvert=OneCaseConvert=CCV_UPPER;break;
			case 'C':CaseConvert=OneCaseConvert=CCV_FLIP;break;
			case 'E':CaseConvert=OneCaseConvert=CCV_NONE;break;

			default:AddChar(strResult, *pszReplace);
			}
			break;
		case '$':{
			int iParam=0;
			while (isdigit((UTCHAR)pszReplace[1])) {
				iParam = iParam*10 + (pszReplace[1]-'0');
				pszReplace++;
			}
			if (iParam < (int)arrReferences.size())
				strResult += arrReferences[iParam];
			break;
				 }
		default:AddChar(strResult, *pszReplace);
		}
		if (*pszReplace) pszReplace++; else break;
	}
	return strResult;
}

bool CRegExp::CreateReplace(tstring strAnalyze, tstring strReplacePattern, tstring &strResult, int iExecFlags)
{
	vector<tstring> arrReferences;
	bool bResult = Match(strAnalyze, iExecFlags, &arrReferences);
	if (!bResult) return false;

	strResult = CreateReplaceString(strReplacePattern, arrReferences);
	return true;
}

bool CRegExp::Replace(tstring &strAnalyze, tstring strReplacePattern, bool bGlobal, int iExecFlags)
{
	int iStart = 0;
	do {
		tstring strReplace;
		bool bResult = CreateReplace(strAnalyze.substr(iStart), strReplacePattern, strReplace, iExecFlags);
		if (!bResult) return m_pPattern!=NULL;
		strAnalyze.replace(iStart + m_piRefs[0], m_piRefs[1] - m_piRefs[0], strReplace);
		iStart = iStart + strReplace.size();
	} while (bGlobal);
	return true;
}

bool CRegExp::Match(tstring strAnalyze, tstring strPattern, int iCompileFlags, int iExecFlags, vector<tstring> *arrReferences)
{
	CRegExp RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Match(strAnalyze, iExecFlags, arrReferences);
}

bool CRegExp::CreateReplace(tstring strAnalyze, tstring strPattern, tstring strReplacePattern,
					  tstring &strResult, int iCompileFlags, int iExecFlags)
{
	CRegExp RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.CreateReplace(strAnalyze, strReplacePattern, strResult, iExecFlags);
}

bool CRegExp::Replace(tstring &strAnalyze, tstring strPattern, tstring strReplacePattern,
					  bool bGlobal, int iCompileFlags, int iExecFlags)
{
	CRegExp RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Replace(strAnalyze, strReplacePattern, bGlobal, iExecFlags);
}

bool CRegExp::Split(tstring strAnalyze, tstring strPattern,  vector<tstring> *arrSplit, int iCompileFlags, int iExecFlags)
{
	CRegExp RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	arrSplit->clear();
	int iStart = 0;
	do {
		int iResult = pcre_exec(RegExp.m_pPattern, RegExp.m_pPatternExtra, strAnalyze.data(), strAnalyze.size(), iStart,
			iExecFlags & ~PCRE_SPLIT_FLAGS, RegExp.m_piRefs, RegExp.m_iRefCount*3);
		if (iResult < 0) break;
		if (((iExecFlags&PCRE_SPLIT_STRIPLEADING) == 0) || (RegExp.m_piRefs[0] != 0) || (strPattern == _T(" ")))
			if (arrSplit) arrSplit->push_back(strAnalyze.substr(iStart, RegExp.m_piRefs[0] - iStart));
		iStart = RegExp.m_piRefs[1];
	} while (true);
	if (((iExecFlags&PCRE_SPLIT_NOSTRIPTRAILING) != 0) || (iStart != strAnalyze.size()) || (strPattern == _T(" ")))
		if (arrSplit) arrSplit->push_back(strAnalyze.substr(iStart));
	return true;
}

bool CRegExp::Grep(tstring strAnalyze, tstring strPattern, tstring strReplacePattern, vector<tstring> *arrFound, int iCompileFlags, int iExecFlags)
{
	CRegExp RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	arrFound->clear();
	int iStart = 0;
	do {
		int iResult = pcre_exec(RegExp.m_pPattern, RegExp.m_pPatternExtra, strAnalyze.data(), strAnalyze.size(), iStart, iExecFlags, RegExp.m_piRefs, RegExp.m_iRefCount*3);
		if (iResult < 0) break;
		vector<tstring> arrReferences;
		RegExp.FillReferences(strAnalyze, iResult, &arrReferences);
		tstring strResult = RegExp.CreateReplaceString(strReplacePattern, arrReferences);
		if (arrFound) arrFound->push_back(strResult);
		iStart = RegExp.m_piRefs[1];
	} while (true);
	return true;
}
