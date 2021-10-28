#pragma once
#include <jni.h>
#include <android/log.h>

#include "Utils.h"
#include "include/obfuscate.h"
#if defined(__arm__)
#include "include/Substrate/SubstrateHook.h"
#include "include/Substrate/CydiaSubstrate.h"
#else
#include "include/And64InlineHook/And64InlineHook.hpp"
#endif

#define TAG "ModMenu"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,   TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,   TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,    TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARNING, TAG, __VA_ARGS__))

// To enable debug.
#define DEBUGGT

#ifdef DEBUGGT
#define TAGD "ModMenuDebug"
#define LOGDD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,   TAGD, __VA_ARGS__))
#define LOGED(...) ((void)__android_log_print(ANDROID_LOG_ERROR,   TAGD, __VA_ARGS__))
#define LOGID(...) ((void)__android_log_print(ANDROID_LOG_INFO,    TAGD, __VA_ARGS__))
#define LOGWD(...) ((void)__android_log_print(ANDROID_LOG_WARNING, TAGD, __VA_ARGS__))
#endif

#define GT(x) reinterpret_cast<void*>(KittyMemory::getAbsoluteAddress(g_GrowtopiaMap, Utilities::Utils::String2Offset(OBFUSCATE(x))))
#define GTS(x) dlsym(g_GrowtopiaHandle, OBFUSCATE(x))

#ifdef __arm__
#define HOOK(a, b, c) MSHookFunction(a, b, c)
#elif __aarch64__
#define HOOK(a, b, c) A64HookFunction(a, b, c)
#endif
