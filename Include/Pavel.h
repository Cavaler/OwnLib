#ifndef __PAVEL_H
#define __PAVEL_H

#include <BASE64.h>
#include <CMapping.h>
#include <Directory.h>
#include <EasyReg.h>
#include <Handles.h>
#include <HMAC.h>
#include <MD5.h>
#include <StringEx.h>
#include <UTF8.h>
#include <XLat.h>

#ifdef _DEBUG
#pragma comment(lib, "PavelMDd8.lib")
#else
#ifdef _DLL 
#pragma comment(lib, "PavelMD8.lib")
#else
#pragma comment(lib, "PavelMT8.lib")
#endif
#endif

#endif
