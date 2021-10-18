#pragma once
#include <dlfcn.h>

#include "include/obfuscate.h"

#if defined(__arm__)
#include "include/Substrate/SubstrateHook.h"
#include "include/Substrate/CydiaSubstrate.h"
#else
#include "include/And64InlineHook/And64InlineHook.hpp"
#endif

#include "include/KittyMemory/KittyMemory.hpp"
#include "include/KittyMemory/MemoryBackup.hpp"
#include "include/KittyMemory/MemoryPatch.hpp"

#include "utilities/Macros.h"

extern void* g_GrowtopiaHandle;
extern ProcMap g_GrowtopiaMap;

void DoMainPulse();