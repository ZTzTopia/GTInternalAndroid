#pragma once
#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>

#include "Main.h"
#include "include/obfuscate.h"
#include "include/KittyMemory/KittyMemory.h"

#define TAG "ModMenu"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,   TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,   TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,    TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARNING, TAG, __VA_ARGS__))

#define GT(x) reinterpret_cast<void*>(KittyMemory::getAbsoluteAddress(g_growtopia_map, Utilities::Utils::String2Offset(AY_OBFUSCATE(x))))
#define GTS(x) dlsym(g_growtopia_handle, AY_OBFUSCATE(x))
