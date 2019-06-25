#ifndef __LIB_UC_H__
#define __LIB_UC_H__

#ifdef LIB_UC_EXPORTS
#define LIB_UC_API __declspec(dllexport)
#define LIB_UC_EXTERN
#else
#define LIB_UC_API __declspec(dllimport)
#define LIB_UC_EXTERN extern
#endif

#define LIB_UC_VERSION "0.0.1"
#if defined(_WIN64)
	#define _UC_PLATFORM "x64"
#else
	#define _UC_PLATFORM ""
#endif

#ifdef _UNICODE
#define TSTRING std::wstring 
#else
#define TSTRING std::string 
#endif

#endif