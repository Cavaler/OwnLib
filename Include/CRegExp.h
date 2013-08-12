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

template<typename s>
inline void PCRE_FREE(s *&p) { if (p) { pcre_free(p); p = NULL; } }

template<> inline void PCRE_FREE(pcre_extra   *&p) { if (p) { pcre_free_study(p);   p = NULL; } }
template<> inline void PCRE_FREE(pcre16_extra *&p) { if (p) { pcre16_free_study(p); p = NULL; } }
template<> inline void PCRE_FREE(pcre32_extra *&p) { if (p) { pcre32_free_study(p); p = NULL; } }

#ifdef UNICODE

pcre * pcre_compile(const wchar_t *pattern, int options, const wchar_t **errorptr, int *erroroffset, const unsigned char *tables);
pcre_extra * pcre_study(const pcre *external_re, int options, const wchar_t **errorptr);
int pcre_exec(const pcre *argument_re, const pcre_extra *extra_data,
		  const wchar_t *subject, int length, int start_offset, int options, int *offsets,
		  int offsetcount);
int pcre_get_named_substring(const pcre *code, const wchar_t *subject, int *ovector,
			int stringcount, const wchar_t *stringname, const wchar_t **stringptr);
int pcre_get_stringnumber(const pcre *code, const wchar_t *stringname);
int pcre_get_stringlist(const pcre *code, vector<wstring> &arrNames);

#endif

int pcre_get_stringlist(const pcre *code, vector<string> &arrNames);

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

	pcre *m_pPattern;
	pcre_extra *m_pPatternExtra;
	int m_iRefCount;
	int *m_piRefs;

	ECaseConvert CaseConvert;
	ECaseConvert OneCaseConvert;
};

typedef CRegExpT<TCHAR> CRegExp;

#define LIB_NAME "pcre"
#include "GenLibName.h"

#endif
