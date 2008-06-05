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

#define PCRE_SPLIT_FLAGS				0xF0000000
#define PCRE_SPLIT_STRIPLEADING			0x80000000
#define PCRE_SPLIT_NOSTRIPTRAILING		0x40000000

class CRegExp
{
public:
	// Preparing
	CRegExp();
	CRegExp(string strPattern, int iCompileFlags = 0);
	bool Compile(string strPattern, int iCompileFlags = 0);
	operator bool() {return m_pPattern != NULL;}
	int RefCount() {return m_iRefCount;}
	~CRegExp();

	// Using
	bool Match(string strAnalyze, int iExecFlags = 0, vector<string> *arrReferences = NULL);
	bool CreateReplace(string strAnalyze, string strReplacePattern, string &strResult, int iExecFlags = 0);
	bool Replace(string &strAnalyze, string strReplacePattern,
		bool bGlobal = false, int iExecFlags = 0);

	static bool Match(string strAnalyze, string strPattern, int iCompileFlags = 0, int iExecFlags = 0, vector<string> *arrReferences = NULL);
	static bool CreateReplace(string strAnalyze, string strPattern, string strReplacePattern,
		string &strResult, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Replace(string &strAnalyze, string strPattern, string strReplacePattern,
		bool bGlobal = false, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Split(string strAnalyze, string strPattern,  vector<string> *arrSplit, int iCompileFlags = 0, int iExecFlags = 0);
	static bool Grep(string strAnalyze, string strPattern, string strReplacePattern,
		vector<string> *arrFound, int iCompileFlags = 0, int iExecFlags = 0);

	const char *m_pszErrPtr;
	int m_iErrOffset;
protected:
	void Release();

	char ConvertCase(char C);
	void AddChar(string &strResult, char C);
	void FillReferences(string &strAnalyze, int iCount, vector<string> *arrReferences);
	string CreateReplaceString(string &strReplacePattern, vector<string> &arrReferences);

	enum ECaseConvert {CCV_NONE,CCV_UPPER,CCV_LOWER,CCV_FLIP};
	pcre *m_pPattern;
	pcre_extra *m_pPatternExtra;
	int m_iRefCount;
	int *m_piRefs;

	ECaseConvert CaseConvert;
	ECaseConvert OneCaseConvert;
};

#ifdef _DEBUG
#pragma comment(lib, "pcreMDd8.lib")
#else
#ifdef __INTEL_COMPILER
#pragma comment(lib, "pcreMTI.lib")
#else
#ifdef _DLL 
#pragma comment(lib, "pcreMD8.lib")
#else
#pragma comment(lib, "pcreMT8.lib")
#endif
#endif
#endif

#endif
