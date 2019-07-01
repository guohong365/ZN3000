#pragma once
#include <uc/libuc.h>
#include <string>
#include <vector>
#include <tchar.h>
#ifdef LIBCORE_EXPORTS
#define LIB_CORE_API __declspec(dllexport)
#define LIB_CORE_CLASS __declspec(dllexport)
#define LIB_CORE_EXTERN
#else
#define LIB_CORE_API __declspec(dllimport)
#define LIB_CORE_CLASS __declspec(dllimport)
#define LIB_CORE_EXTERN extern
#endif


LIB_CORE_EXTERN template class LIB_CORE_CLASS std::allocator<wchar_t>;
LIB_CORE_EXTERN template class LIB_CORE_CLASS std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;
LIB_CORE_EXTERN template class LIB_CORE_CLASS std::allocator<char>;
LIB_CORE_EXTERN template class LIB_CORE_CLASS std::basic_string<char, std::char_traits<char>, std::allocator<char>>;


#ifndef LIBCORE_EXPORTS
#pragma comment(lib, "libcore" _UC_PLATFORM ".lib")
#endif
