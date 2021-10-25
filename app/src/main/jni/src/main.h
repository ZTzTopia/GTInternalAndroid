#pragma once
// We just use the structure because we will use function from the growtopia library.
#include <enet/enet.h>

#include "include/obfuscate.h"

#ifdef __arm__
#include "include/Substrate/SubstrateHook.h"
#include "include/Substrate/CydiaSubstrate.h"
#elif __aarch64__
#include "include/And64InlineHook/And64InlineHook.hpp"
#endif

#include "include/KittyMemory/KittyMemory.hpp"
#include "include/KittyMemory/MemoryBackup.hpp"
#include "include/KittyMemory/MemoryPatch.hpp"

#include "utilities/Macros.h"

extern void* g_GrowtopiaHandle;
extern ProcMap g_GrowtopiaMap;
