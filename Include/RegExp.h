#ifndef __REGEXP_H

#ifdef GNURegExps
#define RegExType 1
#endif

#ifndef RegExType
#define RegExType 2
#endif

#if RegExType==0
#ifdef RegExTypeEcho
#pragma message ("RegExps: Using GNU.")
#endif
#define STDC_HEADERS
#define REGEX_H <GNURegEx\Regex.h>
#endif

#if RegExType==1
#ifdef RegExTypeEcho
#pragma message ("RegExps: Using HSRE.")
#endif
#define REGEX_H <RegEx\Regex.h>
#endif

#if RegExType==2
#ifdef RegExTypeEcho
#pragma message ("RegExps: Using PCRE.")
#endif
#define STATIC
#include <PCRegEx\PCRE.h>
#include <PCRegEx\PCREPosix.h>
#endif

#ifdef REGEX_H
#ifdef __cplusplus
extern "C" {
#include REGEX_H
}
#else
#include REGEX_H
#endif
#endif

#endif
