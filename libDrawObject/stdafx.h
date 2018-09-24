// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once
#ifdef _DEBUG
    #ifndef _SECURE_SCL
        #define _SECURE_SCL 1
    #else
        #if (_SECURE_SCL!=1)
            #define _SECURE_SCL 1
        #endif
    #endif

    #ifndef _HAS_ITERATOR_DEBUGGING
        #define _HAS_ITERATOR_DEBUGGING 1
    #else
        #if (_HAS_ITERATOR_DEBUGGING!=1)
            #define _HAS_ITERATOR_DEBUGGING 1
        #endif
    #endif
#else
    #ifndef _SECURE_SCL
        #define _SECURE_SCL 0
    #else
        #if (_SECURE_SCL!=0)
            #define _SECURE_SCL 0
        #endif
    #endif
    #ifndef _HAS_ITERATOR_DEBUGGING
        #define _HAS_ITERATOR_DEBUGGING 0
    #else
        #if (_HAS_ITERATOR_DEBUGGING!=0)
            #define _HAS_ITERATOR_DEBUGGING 0
        #endif
    #endif
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0500		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0410 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0500	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxrich.h>
#include <atlimage.h>
#include <GdiPlus.h>
#include <vector>
#include <XTToolkitPro.h>    // Xtreme Toolkit Pro components
#include <libUI.h>

HMODULE AfxGetTemplateModule();

