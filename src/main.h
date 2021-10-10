#pragma once

#include "include/obfuscate.h"

#if defined(__arm__)
#include "include/Substrate/SubstrateHook.h"
#include "include/Substrate/CydiaSubstrate.h"
#endif

#include "include/KittyMemory/KittyMemory.hpp"
#include "include/KittyMemory/MemoryBackup.hpp"
#include "include/KittyMemory/MemoryPatch.hpp"

extern void* g_GrowtopiaHandle;
extern ProcMap g_GrowtopiaMap;
