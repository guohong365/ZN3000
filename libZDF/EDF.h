#pragma once

#ifdef LIBZDF_EXPORTS
#define ZDF_API __declspec(dllexport)
#else
#define ZDF_API __declspec(dllimport)
#pragma comment(lib, "libZDF.lib")
#endif

