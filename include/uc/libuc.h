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
#include <memory>
#include <string>
#include <vector>
#include <tchar.h>
#ifdef _UNICODE
#define TSTRING std::wstring 
#else
#define TSTRING std::string 
#endif
LIB_UC_EXTERN template class LIB_UC_API std::allocator<wchar_t>;
LIB_UC_EXTERN template class LIB_UC_API std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;
LIB_UC_EXTERN template class LIB_UC_API std::allocator<char>;
LIB_UC_EXTERN template class LIB_UC_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

#endif