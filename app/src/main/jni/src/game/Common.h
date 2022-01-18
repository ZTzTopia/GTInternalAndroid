#pragma once
#include <unordered_map>
#include <list>
#include <string>

#include "include/KittyMemory/MemoryPatch.h"
#include "proton/shared/common.h"

typedef struct _NetHTTP {
    uint64_t vtable;
    std::string serverName;
} NetHTTP;

typedef struct _CheatList {
    std::string name;
    bool state;
    bool old_state;
    std::function<void (void)> active;
    std::function<void (void)> deactive;
    std::unordered_map<void*, std::string> memory_patch;
    std::list<MemoryPatch> memory_patch_list;
} CheatList;

typedef struct _BaseApp {
    uint8_t pad0[232]; // 0
    unsigned int m_tick; // 232
    uint8_t pad1[4]; // 23
    unsigned int m_gameTick; // 240
} BaseApp;

typedef struct _ObjectData {
    int net_id; // Used to interact with stuff in world
    std::string name;
    CL_Vec2i vec_pos;
    bool is_moderator;
    bool is_local;
} ObjectData;