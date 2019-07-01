#ifndef __LIB_UC_H__
#define __LIB_UC_H__

#define LIB_UC_VERSION "0.0.1"
#if defined(_WIN64)
	#define _UC_PLATFORM "x64"
#else
	#define _UC_PLATFORM ""
#endif
#include <memory>
#include <string>
#include <vector>
#include <tchar.h>
#ifdef _UNICODE
#define TSTRING std::wstring 
#else
#define TSTRING std::string 
#endif

#endif