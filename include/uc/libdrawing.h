#ifndef __LIB_UI_H__
#define __LIB_UI_H__

#include <uc/libUC.h>
#include <tchar.h>
#include <uc/drawing/GdiplusEnviroment.h>
#include <uc/drawing/ScreenInfo.h>
#include <uc/image/ImageUtility.h>
#include <uc/math/MathUtility.h>
#include <uc/drawing/UIHelper.h>

#ifndef LIB_UC_EXPORTS

#pragma comment(lib, "libui" _UC_PLATFORM ".lib")
#pragma message("Automatically linking with libui" _UC_PLATFORM ".lib")

#endif

#endif