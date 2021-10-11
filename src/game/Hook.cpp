#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>

#include "../main.h"
#include "../gui/Gui.h"
#include "../utilities/Logging.h"

void InitHook();

float (*GetScreenSizeX)();
float GetScreenSizeX_hook() {
    if (!pGui) {
        return GetScreenSizeX();
    }

    pGui->m_screenSize.x = GetScreenSizeX();
    return pGui->m_screenSize.x;
}

float (*GetScreenSizeY)();
float GetScreenSizeY_hook() {
    if (!pGui) {
        return GetScreenSizeY();
    }

    pGui->m_screenSize.y = GetScreenSizeY();
    return pGui->m_screenSize.y;
}

void (*BaseApp_draw)(void* thiz);
void BaseApp_draw_hook(void* thiz) {
    BaseApp_draw(thiz);
    DoMainPulse();
}

void (*AppOnTouch)(void* a1, void* a2, int a3, float a4, float a5, int a6);
void AppOnTouch_hook(void* a1, void* a2, int a3, float a4, float a5, int a6) {
    bool ret = true;
    if (pGui && (a4 > 0.0 || a5 > 0.0)) {
        ret = pGui->OnTouchEvent(a3, a4, a5);
    }

    if (ret) {
        AppOnTouch(a1, a2, a3, a4, a5, a6);
    }
}

void InitHook() {
    LOGD("Initializing Hook..");

    // TODO: call it directly
    // idk why call directly make app crash :(
    MSHookFunction(dlsym(g_GrowtopiaAddress, "_Z15GetScreenSizeXfv"), (void*)GetScreenSizeX_hook, (void**)&GetScreenSizeX);
    MSHookFunction(dlsym(g_GrowtopiaAddress, "_Z15GetScreenSizeYfv"), (void*)GetScreenSizeY_hook, (void**)&GetScreenSizeY);

    MSHookFunction(dlsym(g_GrowtopiaAddress, "_ZN7BaseApp4DrawEv"), (void*)BaseApp_draw_hook, (void**)&BaseApp_draw);

    MSHookFunction(dlsym(g_GrowtopiaAddress, "_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void*)AppOnTouch_hook, (void**)&AppOnTouch);
}