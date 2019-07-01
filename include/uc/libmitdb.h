#pragma once
#include <uc/libuc.h>

#ifdef LIBMITDB_EXPORTS
#define LIB_MITDB_API __declspec(dllexport)
#define LIB_MITDB_CLASS __declspec(dllexport)
#define LIB_MITDB_EXTERN
#else
#define LIB_MITDB_API __declspec(dllimport)
#define LIB_MITDB_CLASS __declspec(dllimport)
#define LIB_MITDB_EXTERN extern
#endif


namespace uc{
	namespace signals
	{
		void LIB_MITDB_API wfdbSetup(const char *database_path);
		void LIB_MITDB_API initMitdb(const char *database_path=nullptr);
	}
}

#ifndef LIBMITDB_EXPORTS
#pragma comment(lib, "libmitdb" _UC_PLATFORM ".lib")
#endif