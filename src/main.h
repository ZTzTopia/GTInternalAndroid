#pragma once

#include "include/obfuscate.h"

#if defined(__arm__)
#include "include/Substrate/SubstrateHook.h"
#include "include/Substrate/CydiaSubstrate.h"
#endif

#include "include/KittyMemory/KittyMemory.hpp"
#include "include/KittyMemory/MemoryBackup.hpp"
#include "include/KittyMemory/MemoryPatch.hpp"

#include "gui/Gui.h"

extern void* g_GrowtopiaAddress;
extern ProcMap g_GrowtopiaMap;

extern Gui* pGui;

void DoMainPulse();