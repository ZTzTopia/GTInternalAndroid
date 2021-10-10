#include <jni.h>
#include <android/log.h>

#include "../main.h"
#include "../gui/Gui.h"
#include "../utilities/Logging.h";

void InitHook();

float (*GetScreenSizeX)();
float GetScreenSizeX_hook() {
    Gui::m_screenSize.x = GetScreenSizeX();
    return Gui::m_screenSize.x;
}

float (*GetScreenSizeY)();
float GetScreenSizeY_hook() {
    Gui::m_screenSize.y = GetScreenSizeY();
    return Gui::m_screenSize.y;
}

void (*BaseApp_init)(void* thiz);
void BaseApp_init_hook(void* thiz) {
    BaseApp_init(thiz);
    Gui::Init();
}

void (*BaseApp_draw)(void* thiz);
void BaseApp_draw_hook(void* thiz) {
    BaseApp_draw(thiz);
    Gui::Init();
    Gui::Render();
}

void (*BaseApp_kill)(void* thiz);
void BaseApp_kill_hook(void* thiz) {
    BaseApp_kill(thiz);
    Gui::Shutdown();
}

void (*AppOnTouch)(void* a1, void* a2, int a3, float a4, float a5, int a6);
void AppOnTouch_hook(void* a1, void* a2, int a3, float a4, float a5, int a6) {
    bool ret = true;
    if (a4 > 0.0 && a5 > 0.0) {
        ret = Gui::OnTouchEvent(a3, a4, a5);
    }

    if (ret) {
        AppOnTouch(a1, a2, a3, a4, a5, a6);
    }
}

void InitHook() {
    LOGD("Initializing Hook..");

    // TODO: call it directly
    // idk why call directly make app crash :(
    MSHookFunction(dlsym(g_GrowtopiaHandle, "_Z15GetScreenSizeXfv"), (void*)GetScreenSizeX_hook, (void**)&GetScreenSizeX);
    MSHookFunction(dlsym(g_GrowtopiaHandle, "_Z15GetScreenSizeYfv"), (void*)GetScreenSizeY_hook, (void**)&GetScreenSizeY);

    // MSHookFunction(dlsym(g_GrowtopiaHandle, "_ZN7BaseApp4InitEv"), (void*)BaseApp_init_hook, (void**)&BaseApp_init);
    MSHookFunction(dlsym(g_GrowtopiaHandle, "_ZN7BaseApp4DrawEv"), (void*)BaseApp_draw_hook, (void**)&BaseApp_draw);
    // MSHookFunction(dlsym(g_GrowtopiaHandle, "_ZN7BaseApp4KillEv"), (void*)BaseApp_kill_hook, (void**)&BaseApp_kill);

    MSHookFunction(dlsym(g_GrowtopiaHandle, "_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void*)AppOnTouch_hook, (void**)&AppOnTouch);
}