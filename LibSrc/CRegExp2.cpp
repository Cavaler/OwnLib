#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <CRegExp2.h>
#include <StringEx.h>
#include <map>
/*
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
*/
template<class CHAR>
CRegExp2T<CHAR>::CRegExp2T() : m_pPattern(NULL), m_pMatchData(NULL)
{
}

template<class CHAR>
CRegExp2T<CHAR>::CRegExp2T(cstring strPattern, int iCompileFlags) : m_pPattern(NULL), m_pMatchData(NULL)
{
	Compile(strPattern, iCompileFlags);
}

template<class CHAR>
bool CRegExp2T<CHAR>::Compile(cstring strPattern, int iCompileFlags)
{
	Release();

	m_pPattern = pcre2_compile((PCRE2_SPTR)strPattern.c_str(), PCRE2_ZERO_TERMINATED, iCompileFlags, &m_iErrCode, &m_iErrOffset, NULL);
	if (!m_pPattern) return false;

	m_pMatchData = pcre2_match_data_create_from_pattern(m_pPattern, NULL);
	if (!m_pMatchData) return false;

	return true;
}

template<class CHAR>
void CRegExp2T<CHAR>::Release()
{
	if (m_pPattern) {pcre2_code_free(m_pPattern); m_pPattern = NULL;}
	if (m_pMatchData) {pcre2_match_data_free(m_pMatchData); m_pMatchData = NULL;}
}

template<class CHAR>
CRegExp2T<CHAR>::~CRegExp2T() {
	Release();
}

template<class CHAR>
void CRegExp2T<CHAR>::FillReferences(cstring &strAnalyze, int iCount, vector<cstring> *arrReferences)
{
	arrReferences->clear();

	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(m_pMatchData);

	for (int i = 0; i < iCount; i++) {
		if (ovector[i*2] >= 0)
			arrReferences->push_back(cstring(strAnalyze, ovector[i*2], ovector[i*2+1] - ovector[i*2]));
		else
			arrReferences->push_back(cstring());
	}
}

template<class CHAR>
bool CRegExp2T<CHAR>::Match(cstring strAnalyze, int iExecFlags, vector<cstring> *arrReferences)
{
	int iResult = pcre2_match(m_pPattern, (PCRE2_SPTR)strAnalyze.data(), (int)strAnalyze.size(), 0, iExecFlags, m_pMatchData, NULL);
	if (iResult < 0)
		return false;

	if (arrReferences)
		FillReferences(strAnalyze, iResult, arrReferences);

	return true;
}

template<class CHAR>
CHAR CRegExp2T<CHAR>::ConvertCase(CHAR C)
{
	C = CStringT<CHAR>::ConvertCase(C, OneCaseConvert);
	OneCaseConvert=CaseConvert;
	return C;
}

template<class CHAR>
void CRegExp2T<CHAR>::AddChar(cstring &strResult, CHAR C)
{
	strResult += ConvertCase(C);
}

template<class CHAR>
typename CRegExp2T<CHAR>::cstring
CRegExp2T<CHAR>::CreateReplaceString(cstring &strReplacePattern, vector<cstring> &arrReferences)
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
bool CRegExp2T<CHAR>::CreateReplace(cstring strAnalyze, cstring strReplacePattern, cstring &strResult, int iExecFlags)
{
	vector<cstring> arrReferences;
	bool bResult = Match(strAnalyze, iExecFlags, &arrReferences);
	if (!bResult) return false;

	strResult = CreateReplaceString(strReplacePattern, arrReferences);
	return true;
}

template<class CHAR>
bool CRegExp2T<CHAR>::Replace(cstring &strAnalyze, cstring strReplacePattern, bool bGlobal, int iExecFlags)
{
	size_t iStart = 0;
	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(m_pMatchData);
	do {
		cstring strReplace;
		bool bResult = CreateReplace(strAnalyze.substr(iStart), strReplacePattern, strReplace, iExecFlags);
		if (!bResult) return m_pPattern!=NULL;
		strAnalyze.replace(iStart + ovector[0], ovector[1] - ovector[0], strReplace);
		iStart = iStart + strReplace.size();
	} while (bGlobal);
	return true;
}

template<class CHAR>
bool CRegExp2T<CHAR>::Match(cstring strAnalyze, cstring strPattern, int iCompileFlags, int iExecFlags, vector<cstring> *arrReferences)
{
	CRegExp2T<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Match(strAnalyze, iExecFlags, arrReferences);
}

template<class CHAR>
bool CRegExp2T<CHAR>::CreateReplace(cstring strAnalyze, cstring strPattern, cstring strReplacePattern,
					  cstring &strResult, int iCompileFlags, int iExecFlags)
{
	CRegExp2T<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.CreateReplace(strAnalyze, strReplacePattern, strResult, iExecFlags);
}

template<class CHAR>
bool CRegExp2T<CHAR>::Replace(cstring &strAnalyze, cstring strPattern, cstring strReplacePattern,
					  bool bGlobal, int iCompileFlags, int iExecFlags)
{
	CRegExp2T<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	return RegExp.Replace(strAnalyze, strReplacePattern, bGlobal, iExecFlags);
}

template<class CHAR>
bool CRegExp2T<CHAR>::Split(cstring strAnalyze, cstring strPattern,  vector<cstring> *arrSplit, int iCompileFlags, int iExecFlags)
{
	CRegExp2T<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(RegExp.m_pMatchData);

	arrSplit->clear();
	int iStart = 0;

	do {
        int iResult = pcre2_match(RegExp.m_pPattern, (PCRE2_SPTR)strAnalyze.data(), (int)strAnalyze.size(), iStart,
			iExecFlags & ~PCRE_SPLIT_FLAGS, RegExp.m_pMatchData, NULL);
		if (iResult < 0) break;

		if (((iExecFlags&PCRE_SPLIT_STRIPLEADING) == 0) ||
			(ovector[0] != 0) ||
			(strPattern == CStringT<CHAR>::_T2(" ")))
		{
			if (arrSplit) arrSplit->push_back(strAnalyze.substr(iStart, ovector[0] - iStart));
		}
		iStart = ovector[1];
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
bool CRegExp2T<CHAR>::Grep(cstring strAnalyze, cstring strPattern, cstring strReplacePattern, vector<cstring> *arrFound, int iCompileFlags, int iExecFlags)
{
	CRegExp2T<CHAR> RegExp(strPattern, iCompileFlags);
	if (!RegExp.m_pPattern) return false;
	PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(RegExp.m_pMatchData);

	arrFound->clear();
	int iStart = 0;

	do {
		int iResult = pcre2_match(RegExp.m_pPattern, (PCRE2_SPTR)strAnalyze.data(), (int)strAnalyze.size(), iStart, iExecFlags, RegExp.m_pMatchData, NULL);
		if (iResult < 0) break;

		vector<cstring> arrReferences;
		RegExp.FillReferences(strAnalyze, iResult, &arrReferences);
		cstring strResult = RegExp.CreateReplaceString(strReplacePattern, arrReferences);
		if (arrFound) arrFound->push_back(strResult);
		iStart = ovector[1];
	} while (true);
	return true;
}

template class CRegExp2T<TCHAR>;
