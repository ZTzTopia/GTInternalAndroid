#pragma once

#define GT(x) KittyMemory::getAbsoluteAddress(g_GrowtopiaMap, x)
#define GTS(x) dlsym(g_GrowtopiaHandle, x)
