#ifndef __CREGEXP_H
#define __CREGEXP_H

#ifndef PCRE_STATIC
#define PCRE_STATIC
#endif
#include <pcre\pcre.h>

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;
#include <tstring.h>

#define PCRE_SPLIT_FLAGS				0xF0000000
#define PCRE_SPLIT_STRIPLEADING			0x80000000
#define PCRE_SPLIT_NOSTRIPTRAILING		0x40000000

#ifdef UNICODE

pcre * pcre_compile(const wchar_t *pattern, int options, const wchar_t **errorptr, int *erroroffset, const unsigned char *tables);
pcre_extra * pcre_study(const pcre *external_re, int options, const wchar_t **errorptr);
int pcre_exec(const pcre *argument_re, const pcre_extra *extra_data,
		  const wchar_t *subject, int length, int start_offset, int options, int *offsets,
		  int offsetcount);

#endif

class CRegExp
{
public:
	// Preparing
	CRegExp();
	CRegExp(tstring strPattern, int iCompileFlags = 0);
	bool Compile(tstring strPattern, int iCompileFlags = 0);
	operator bool() {return m_pPattern != NULL;}
	int RefCount() {return m_iRefCount;}
	~CRegExp();

	// Using
	bool Match(tstring strAnalyze, int iExecFlags = 0, vector<tstring> *arrReferences = NULL);
	bool CreateReplace(tstring strAnalyze, tstring strReplacePattern, tstring &strResult, int iExecFlags = 0);
	bool Replace(tstring &strAnalyze, tstring strReplacePattern,
		bool bGlobal = false, int iExecFlags = 0);

	static bool Match(tstring strAnalyze, tstring strPattern, int iCompileFlags = 0, int iExecFlags = 0, vector<tstring> *arrReferences = NULL);
	static bool CreateReplace(tstring strAnalyze, tstring strPattern, tstring strReplacePattern,
		tstring &strResult, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Replace(tstring &strAnalyze, tstring strPattern, tstring strReplacePattern,
		bool bGlobal = false, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Split(tstring strAnalyze, tstring strPattern,  vector<tstring> *arrSplit, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Grep(tstring strAnalyze, tstring strPattern, tstring strReplacePattern,
		vector<tstring> *arrFound, int iCompileFlags = 0, int iExecFlags = 0);

	const TCHAR *m_pszErrPtr;
	int m_iErrOffset;
protected:
	void Release();

	TCHAR ConvertCase(TCHAR C);
	void AddChar(tstring &strResult, TCHAR C);
	void FillReferences(tstring &strAnalyze, int iCount, vector<tstring> *arrReferences);
	tstring CreateReplaceString(tstring &strReplacePattern, vector<tstring> &arrReferences);

	enum ECaseConvert {CCV_NONE,CCV_UPPER,CCV_LOWER,CCV_FLIP};
	pcre *m_pPattern;
	pcre_extra *m_pPatternExtra;
	int m_iRefCount;
	int *m_piRefs;

	ECaseConvert CaseConvert;
	ECaseConvert OneCaseConvert;
};

#define LIB_NAME "pcre"
#include "GenLibName.h"

#endif
