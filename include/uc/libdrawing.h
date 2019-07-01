#pragma once

#include <uc/libuc.h>

#define LIB_DRAWING_EXPORT __declspec(dllexport)
#define LIB_DRAWING_IMPORT __declspec(dllimport)

#ifdef _AFXEXT
#define	DRAWING_API LIB_DRAWING_EXPORT
#define DRAWING_CLASS LIB_DRAWING_EXPORT
#else
#define	DRAWING_API LIB_DRAWING_IMPORT
#define DRAWING_CLASS LIB_DRAWING_IMPORT
#endif

#include <tchar.h>
#include <GdiPlus.h>

#ifndef _AFXEXT
#pragma comment(lib, "libdrawing" _UC_PLATFORM ".lib")
#pragma message("Automatically linking with libdrawing" _UC_PLATFORM ".lib")
#endif
