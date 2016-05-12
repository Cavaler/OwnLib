#ifndef __CREGEXP2_H
#define __CREGEXP2_H

#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2\pcre2.h>

#ifdef UNICODE
#define PCRE2_CODE_UNIT_WIDTH 16
#include <pcre2\pcre2.h>
#endif

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;
#include <tstring.h>

#define PCRE_SPLIT_FLAGS				0xF0000000
#define PCRE_SPLIT_STRIPLEADING			0x80000000
#define PCRE_SPLIT_NOSTRIPTRAILING		0x40000000

#ifdef UNICODE
#endif

int pcre2_get_stringlist(pcre2_match_data *match, vector<tstring> &arrNames);

template<class CHAR>
class CRegExpT
{
public:
	typedef basic_string<CHAR> cstring;

	// Preparing
	CRegExpT();
	CRegExpT(cstring strPattern, int iCompileFlags = 0);
	~CRegExpT();

	bool Compile(cstring strPattern, int iCompileFlags = 0);
	operator bool() {return m_pPattern != NULL;}
	int RefCount() {return m_iRefCount;}
	int RefStart(int nRef) { return m_piRefs[nRef]; }

	// Using
	bool Match(cstring strAnalyze, int iExecFlags = 0, vector<cstring> *arrReferences = NULL);
	bool CreateReplace(cstring strAnalyze, cstring strReplacePattern, cstring &strResult, int iExecFlags = 0);
	bool Replace(cstring &strAnalyze, cstring strReplacePattern,
		bool bGlobal = false, int iExecFlags = 0);

	static bool Match(cstring strAnalyze, cstring strPattern, int iCompileFlags = 0, int iExecFlags = 0, vector<cstring> *arrReferences = NULL);
	static bool CreateReplace(cstring strAnalyze, cstring strPattern, cstring strReplacePattern,
		cstring &strResult, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Replace(cstring &strAnalyze, cstring strPattern, cstring strReplacePattern,
		bool bGlobal = false, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Split(cstring strAnalyze, cstring strPattern,  vector<cstring> *arrSplit, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Grep(cstring strAnalyze, cstring strPattern, cstring strReplacePattern,
		vector<cstring> *arrFound, int iCompileFlags = 0, int iExecFlags = 0);

	const CHAR *m_pszErrPtr;
	int m_iErrOffset;
protected:
	void Release();

	CHAR ConvertCase(CHAR C);
	void AddChar(cstring &strResult, CHAR C);
	void FillReferences(cstring &strAnalyze, int iCount, vector<cstring> *arrReferences);
	cstring CreateReplaceString(cstring &strReplacePattern, vector<cstring> &arrReferences);

	pcre2_code *m_pPattern;
	int m_iRefCount;
	int *m_piRefs;

	ECaseConvert CaseConvert;
	ECaseConvert OneCaseConvert;
};

typedef CRegExpT<TCHAR> CRegExp;

#define LIB_NAME "pcre"
#include "GenLibName.h"

#endif
