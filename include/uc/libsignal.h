#pragma once
#ifdef LIBSIGNAL_EXPORTS
#define LIB_UC_EXPORTS
#endif
#include <uc/libuc.h>
#include <uc/signal/SignalBuffer.h>
#include <uc/signal/Record.h>

#ifndef LIB_UC_EXPORTS
#pragma comment(lib, "libFilter" _UC_PLATFORM ".lib")
#pragma message("Automatically linking with libFilter" _UC_PLATFORM ".lib")
#endif