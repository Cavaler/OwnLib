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

//	We must store them for get_substring
vector<int> g_origOffsets;

template<class Vector> void resize_vector(Vector &vec, size_t nLength)
{
	if ((vec.size() < nLength) || (vec.size() > nLength*16))
		vec.resize(nLength);
}

int pcre_exec(const pcre *argument_re, const pcre_extra *extra_data,
			  const wchar_t *subject, int length, int start_offset, int options, int *offsets,
			  int offsetcount)
{
	static std::vector<size_t> utf2char_;
	static std::vector<size_t> char2utf_;

	string szSubject = UTF8FromUnicode(wstring(subject, length));

	resize_vector(utf2char_, szSubject.length()*4+2);
	resize_vector(char2utf_, szSubject.length()+2);
	size_t *utf2char = &utf2char_[1];
	size_t *char2utf = &char2utf_[1];

	utf2char[-1] = -1;	// For nonexistent matches
	size_t nByte = 0, nChar = 0;
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

	g_origOffsets.resize(offsetcount);

#ifdef UNICODE
	options |= PCRE_NO_START_OPTIMIZE;
#endif
	int nResult = pcre_exec(argument_re, extra_data, szSubject.c_str(), (int)szSubject.length(), (int)char2utf[start_offset],
		options, offsetcount ? &g_origOffsets[0] : NULL, offsetcount);

	if ((nResult >= 0) && offsets && offsetcount) {
		for (int nMatch = 0; nMatch < nResult; nMatch++) {
			offsets[nMatch*2  ] = (int)utf2char[g_origOffsets[nMatch*2  ]];
			offsets[nMatch*2+1] = (int)utf2char[g_origOffsets[nMatch*2+1]];
		}
	}

	return nResult;
}

int pcre_get_named_substring(const pcre *code, const wchar_t *subject, int *ovector,
							 int stringcount, const wchar_t *stringname, const wchar_t **stringptr)
{
	//	Last stored offsets
	ovector = g_origOffsets.empty() ? NULL : &g_origOffsets[0];

	// We have no length - let's use end of matched piece
	string szSubject = UTF8FromUnicode(wstring(subject, ovector[1]));
	string szName = UTF8FromUnicode(stringname);

	const char *szString;
	int nLen = pcre_get_named_substring(code, szSubject.c_str(), ovector, stringcount,
		szName.c_str(), &szString);

	if (nLen <= 0) {
		*stringptr = NULL;
		return nLen;
	}

	wstring strString = UTF8ToUnicode(string(szString, nLen));
	pcre_free((void *)szString);

	wchar_t *wszString = (wchar_t *)pcre_malloc(strString.size()*2+2);
	memmove(wszString, strString.c_str(), strString.size()*2+2);
	*stringptr = wszString;
	return (int)strString.size();
}

int pcre_get_stringnumber(const pcre *code, const wchar_t *stringname) {
	string szName = UTF8FromUnicode(stringname);

	return pcre_get_stringnumber(code, szName.c_str());
}

int pcre_get_stringlist(const pcre *code, vector<wstring> &arrNames)
{
	int rc, count, entrysize;
	BYTE *nametable;

	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMECOUNT, &count)) != 0)
		return rc;
	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMEENTRYSIZE, &entrysize)) != 0)
		return rc;
	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMETABLE, &nametable)) != 0)
		return rc;

	for (int item = 0; item < count; item++)
	{
		BYTE *entry = nametable + entrysize*item;
		char *name  = (char *)(entry + 2);
		arrNames.push_back(UTF8ToUnicode(name));
	}

	return count;
}

#endif

int pcre_get_stringlist(const pcre *code, vector<string> &arrNames)
{
	int rc, count, entrysize;
	BYTE *nametable;

	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMECOUNT, &count)) != 0)
		return rc;
	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMEENTRYSIZE, &entrysize)) != 0)
		return rc;
	if ((rc = pcre_fullinfo(code, NULL, PCRE_INFO_NAMETABLE, &nametable)) != 0)
		return rc;

	for (int item = 0; item < count; item++)
	{
		BYTE *entry = nametable + entrysize*item;
		char *name  = (char *)(entry + 2);
		arrNames.push_back(name);
	}

	return count;
}

template<class CHAR>
CRegExpT<CHAR>::CRegExpT() : m_pPattern(NULL), m_pPatternExtra(NULL), m_piRefs(NULL), m_iRefCount(0)
{
}

template<class CHAR>
CRegExpT<CHAR>::CRegExpT(cstring strPattern, int iCompileFlags) : m_pPattern(NULL), m_pPatternExtra(NULL), m_piRefs(NULL)
{
	Compile(strPattern, iCompileFlags);
}

template<class CHAR>
bool CRegExpT<CHAR>::Compile(cstring strPattern, int iCompileFlags)
{
	Release();

	m_pPattern = pcre_compile(strPattern.c_str(), iCompileFlags, &m_pszErrPtr, &m_iErrOffset,NULL);
	if (!m_pPattern) return false;
	m_pPatternExtra = pcre_study(m_pPattern, 0, &m_pszErrPtr);

	pcre_fullinfo(m_pPattern, m_pPatternExtra, PCRE_INFO_CAPTURECOUNT, &m_iRefCount);
	m_iRefCount++;
	m_piRefs = new int[m_iRefCount*3];

	return true;
}

template<class CHAR>
void CRegExpT<CHAR>::Release()
{
	if (m_piRefs) {delete[] m_piRefs; m_piRefs = NULL;}
	if (m_pPattern) {pcre_free(m_pPattern); m_pPattern = NULL;}
	if (m_pPatternExtra) {pcre_free(m_pPatternExtra); m_pPatternExtra = NULL;}
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
