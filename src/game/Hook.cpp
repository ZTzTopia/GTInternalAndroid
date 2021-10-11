#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>

#include "../main.h"
#include "../gui/Gui.h"
#include "../utilities/Logging.h"

extern Gui* g_Gui;

void InitHook();

void (*BaseApp_draw)(void* thiz);
void BaseApp_draw_hook(void* thiz) {
    BaseApp_draw(thiz);
    DoMainPulse();
}

void (*AppOnTouch)(void* a1, void* a2, int a3, float a4, float a5, int a6);
void AppOnTouch_hook(void* a1, void* a2, int a3, float a4, float a5, int a6) {
    bool ret = true;
    if (g_Gui && (a4 > 0.0 || a5 > 0.0)) {
        ret = g_Gui->OnTouchEvent(a3, a6, a4, a5);
    }

    if (ret) {
        AppOnTouch(a1, a2, a3, a4, a5, a6);
    } else {
        AppOnTouch(a1, a2, 1, 0.0, 0.0, a6);
    }
}

void InitHook() {
    LOGD("Initializing Hook..");

    // Hard to explain btw :D
    MSHookFunction(GTS("_ZN7BaseApp4DrawEv"), (void*)BaseApp_draw_hook, (void**)&BaseApp_draw);
    MSHookFunction(GTS("_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void*)AppOnTouch_hook, (void**)&AppOnTouch);
}