// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <Windows.h>

#ifdef LIBCORE_EXPORTS
#define LIB_UC_EXPORTS
#endif
#include <vector>
#include <uc/libuc.h>
#include <uc/libcore.h>
#include <uc/signals/SignalBuffer.h>
#include <uc/signals/SignalSeries.h>

