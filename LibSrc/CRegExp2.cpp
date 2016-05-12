#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <CRegExp2.h>
#include <StringEx.h>
#include <map>

int pcre2_get_stringlist(pcre2_match_data *match, vector<wstring> &arrNames)
{
	uint32_t item = 0;
	PCRE2_UCHAR *buffer;
	PCRE2_SIZE bufflen;

	for (; ; item++)
	{
		if (pcre2_substring_get_bynumber(match, item, &buffer, &bufflen) == 0)
			arrNames.push_back(tstring((TCHAR *)buffer, bufflen));
		else
			break;
	}

	return item;
}

template<class CHAR>
CRegExpT<CHAR>::CRegExpT() : m_pPattern(NULL), m_piRefs(NULL), m_iRefCount(0)
{
}

template<class CHAR>
CRegExpT<CHAR>::CRegExpT(cstring strPattern, int iCompileFlags) : m_pPattern(NULL), m_piRefs(NULL)
{
	Compile(strPattern, iCompileFlags);
}

template<class CHAR>
bool CRegExpT<CHAR>::Compile(cstring strPattern, int iCompileFlags)
{
	Release();

	m_pPattern = pcre2_compile((PCRE2_SPTR)strPattern.c_str(), PCRE2_ZERO_TERMINATED, iCompileFlags, &m_pszErrPtr, &m_iErrOffset,NULL);
	if (!m_pPattern) return false;

	pcre_fullinfo(m_pPattern, m_pPatternExtra, PCRE_INFO_CAPTURECOUNT, &m_iRefCount);
	m_iRefCount++;
	m_piRefs = new int[m_iRefCount*3];

	return true;
}

template<class CHAR>
void CRegExpT<CHAR>::Release()
{
	if (m_piRefs) {delete[] m_piRefs; m_piRefs = NULL;}
	if (m_pPattern) {pcre2_code_free(m_pPattern); m_pPattern = NULL;}

	m_iRefCount = 0;
}

template<class CHAR>
CRegExpT<CHAR>::~CRegExpT() {
	Release();
}

template<class CHAR>
void CRegExpT<CHAR>::FillReferences(cstring &strAnalyze, int iCount, vector<cstring> *arrReferences)
{
	arrReferences->clear();
	for (int i = 0; i < iCount; i++) {
		if (m_piRefs[i*2] >= 0)
			arrReferences->push_back(cstring(strAnalyze, m_piRefs[i*2], m_piRefs[i*2+1] - m_piRefs[i*2]));
		else
			arrReferences->push_back(cstring());
	}
}

template<class CHAR>
bool CRegExpT<CHAR>::Match(cstring strAnalyze, int iExecFlags, vector<cstring> *arrReferences)
{
	int iResult = pcre_exec(m_pPattern, m_pPatternExtra, strAnalyze.data(), (int)strAnalyze.size(), 0, iExecFlags, m_piRefs, m_iRefCount*3);
	if (iResult < 0)
	{
		return false;
	}
	if (arrReferences)
		FillReferences(strAnalyze, iResult, arrReferences);
	return true;
}

template<class CHAR>
CHAR CRegExpT<CHAR>::ConvertCase(CHAR C)
{
	C = CStringT<CHAR>::ConvertCase(C, OneCaseConvert);
	OneCaseConvert=CaseConvert;
	return C;
}

template<class CHAR>
void CRegExpT<CHAR>::AddChar(cstring &strResult, CHAR C)
{
	strResult += ConvertCase(C);
}

template<class CHAR>
typename CRegExpT<CHAR>::cstring
CRegExpT<CHAR>::CreateReplaceString(cstring &strReplacePattern, vector<cstring> &arrReferences)
{
	cstring strResult;

	const CHAR *pszReplace = strReplacePattern.c_str();
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

template<class CHAR>
bool CRegExpT<CHAR>::CreateReplace(cstring strAnalyze, cstring strReplacePattern, cstring &strResult, int iExecFlags)
{
	vector<cstring> arrReferences;
	bool bResult = Match(strAnalyze, iExecFlags, &arrReferences);
	if (!bResult) return false;

	strResult = CreateReplaceString(strReplacePattern, arrReferences);
	return true;
}

template<class CHAR>
bool CRegExpT<CHAR>::Replace(cstring &strAnalyze, cstring strReplacePattern, bool bGlobal, int iExecFlags)
{
	size_t iStart = 0;
	do {
		cstring strReplace;
		bool bResult = CreateReplace(strAnalyze.substr(iStart), strReplacePattern, strReplace, iExecFlags);
		if (!bResult) return m_pPattern!=NULL;
		strAnalyze.replace(iStart + m_piRefs[0], m_piRefs[1] - m_piRefs[0], strReplace);
		iStart = iStart + strReplace.size();
	} while (bGlobal);
	return true;
}

template<class CHAR>
bool CRegExpT<CHAR>::Match(cstring strAnalyze, cstring strPattern, int iCompileFlags, int iExecFlags, vector<cstring> *arrReferences)
{
	CRegExpT<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Match(strAnalyze, iExecFlags, arrReferences);
}

template<class CHAR>
bool CRegExpT<CHAR>::CreateReplace(cstring strAnalyze, cstring strPattern, cstring strReplacePattern,
					  cstring &strResult, int iCompileFlags, int iExecFlags)
{
	CRegExpT<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.CreateReplace(strAnalyze, strReplacePattern, strResult, iExecFlags);
}

template<class CHAR>
bool CRegExpT<CHAR>::Replace(cstring &strAnalyze, cstring strPattern, cstring strReplacePattern,
					  bool bGlobal, int iCompileFlags, int iExecFlags)
{
	CRegExpT<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Replace(strAnalyze, strReplacePattern, bGlobal, iExecFlags);
}

template<class CHAR>
bool CRegExpT<CHAR>::Split(cstring strAnalyze, cstring strPattern,  vector<cstring> *arrSplit, int iCompileFlags, int iExecFlags)
{
	CRegExpT<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	arrSplit->clear();
	int iStart = 0;

	do {
		int iResult = pcre_exec(RegExp.m_pPattern, RegExp.m_pPatternExtra, strAnalyze.data(), (int)strAnalyze.size(), iStart,
			iExecFlags & ~PCRE_SPLIT_FLAGS, RegExp.m_piRefs, RegExp.m_iRefCount*3);
		if (iResult < 0) break;
		if (((iExecFlags&PCRE_SPLIT_STRIPLEADING) == 0) ||
			(RegExp.m_piRefs[0] != 0) ||
			(strPattern == CStringT<CHAR>::_T2(" ")))
		{
			if (arrSplit) arrSplit->push_back(strAnalyze.substr(iStart, RegExp.m_piRefs[0] - iStart));
		}
		iStart = RegExp.m_piRefs[1];
	} while (true);

	if (((iExecFlags&PCRE_SPLIT_NOSTRIPTRAILING) != 0) ||
		(iStart != strAnalyze.size()) ||
		(strPattern == CStringT<CHAR>::_T2(" ")))
	{
		if (arrSplit) arrSplit->push_back(strAnalyze.substr(iStart));
	}

	return true;
}

template<class CHAR>
bool CRegExpT<CHAR>::Grep(cstring strAnalyze, cstring strPattern, cstring strReplacePattern, vector<cstring> *arrFound, int iCompileFlags, int iExecFlags)
{
	CRegExpT<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	arrFound->clear();
	int iStart = 0;
	do {
		int iResult = pcre_exec(RegExp.m_pPattern, RegExp.m_pPatternExtra, strAnalyze.data(), (int)strAnalyze.size(), iStart, iExecFlags, RegExp.m_piRefs, RegExp.m_iRefCount*3);
		if (iResult < 0) break;
		vector<cstring> arrReferences;
		RegExp.FillReferences(strAnalyze, iResult, &arrReferences);
		cstring strResult = RegExp.CreateReplaceString(strReplacePattern, arrReferences);
		if (arrFound) arrFound->push_back(strResult);
		iStart = RegExp.m_piRefs[1];
	} while (true);
	return true;
}

template class CRegExpT<char>;
#ifdef UNICODE
template class CRegExpT<wchar_t>;
#endif
