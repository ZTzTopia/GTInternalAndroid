#pragma once

// We no need the fucking whole file proton.
typedef struct _NetHTTP {
    uint64_t vtable;
    std::string serverName;
} NetHTTP;

typedef struct _OnTouchEvent {
    void* ontouchThiz;
    void* ontouchObject;
    int ontouchType;
    float ontouchX;
    float ontouchY;
    int ontouchMulti;
} OnTouchEvent;

typedef struct stCheatList {
    std::string cheatName;
    bool cheatOldState;
    bool cheatState;
    void (*cheatActive)();
    void (*cheatDeactive)();
    void* cheatAddr;
    std::string cheatHex;
    MemoryPatch cheatMemPatch;
};