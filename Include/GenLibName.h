#ifdef _DLL
	#define LIB_RUNTIME "MD"
#else
	#define LIB_RUNTIME "MT"
#endif

#ifdef _DEBUG
	#define LIB_DEBUG "d"
#else
	#define LIB_DEBUG ""
#endif

#ifdef __INTEL_COMPILER
	#define LIB_COMPILER "I"
#else
	#define LIB_COMPILER "8"
#endif

#ifdef _UNICODE
	#define LIB_UNICODE "U"
#else
	#define LIB_UNICODE ""
#endif

#ifdef _WIN64
	#define LIB_PLATFORM "x64"
#else
	#define LIB_PLATFORM ""
#endif

#ifndef LIB_APPEND
	#define LIB_APPEND ""
#endif

#define LIB_SPEC LIB_RUNTIME LIB_DEBUG LIB_COMPILER LIB_UNICODE LIB_PLATFORM LIB_APPEND
#pragma comment(lib, LIB_NAME LIB_SPEC ".lib")

#undef LIB_PLATFORM
#undef LIB_RUNTIME
#undef LIB_DEBUG
#undef LIB_COMPILER
#undef LIB_UNICODE
#undef LIB_SPEC
#undef LIB_NAME
#undef LIB_APPEND
