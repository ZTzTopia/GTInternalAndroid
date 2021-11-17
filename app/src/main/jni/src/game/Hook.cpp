#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <pthread.h>

#include "../main.h"
#include "../gui/Gui.h"
#include "../utilities/Logging.h"

static struct _stOnTouchEvent {
    void* ontouch_a1;
    void* ontouch_a2;
    int ontouch_type;
    float ontouch_x;
    float ontouch_y;
    int ontouch_multi;
} stOnTouchEvent;

extern Gui* g_Gui;

void InitHook();

void (*BaseApp_draw)(void* thiz);
void BaseApp_draw_hook(void* thiz) {
    BaseApp_draw(thiz);
    DoMainPulse();
}

void (*AppOnTouch)(void* a1, void* a2, int a3, float a4, float a5, int a6);
void* ontouch_thread(void*) {
    do {
        sleep(0);
    } while (g_Gui->m_sendOnTouchEvent == 0);

    if (g_Gui->m_sendOnTouchEvent == 2) {
        AppOnTouch(stOnTouchEvent.ontouch_a1, stOnTouchEvent.ontouch_a2, stOnTouchEvent.ontouch_type,
                   stOnTouchEvent.ontouch_x, stOnTouchEvent.ontouch_y, stOnTouchEvent.ontouch_multi);
    }
    else if (g_Gui->m_sendOnTouchEvent == 1) {
        // Type: Pop, X: 0.0, Y: 0.0, Multi: 0
        AppOnTouch(stOnTouchEvent.ontouch_a1, stOnTouchEvent.ontouch_a2, 1, 0.0, 0.0, 0);
    }

    g_Gui->m_sendOnTouchEvent = 0;

    // Now we can exit the thread
    pthread_exit(nullptr);
}

void AppOnTouch_hook(void* a1, void* a2, int type, float x, float y, int multi) {
    if (g_Gui && (x > 0.0 || y > 0.0)) {
        g_Gui->OnTouchEvent(type, multi, x, y);
    }

    stOnTouchEvent.ontouch_a1 = a1;
    stOnTouchEvent.ontouch_a2 = a2;
    stOnTouchEvent.ontouch_type = type;
    stOnTouchEvent.ontouch_x = x;
    stOnTouchEvent.ontouch_y = y;
    stOnTouchEvent.ontouch_multi = multi;
    
    // Create a new thread because we dont want do while loop make ontouch thread stuck
    pthread_t ptid;
    pthread_create(&ptid, nullptr, ontouch_thread, nullptr);
}

void InitHook() {
    LOGD("Initializing Hook..");

    // Hard to explain btw :D
#if defined(__arm__)
    MSHookFunction(GTS("_ZN7BaseApp4DrawEv"), (void*)BaseApp_draw_hook, (void**)&BaseApp_draw);
    MSHookFunction(GTS("_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void*)AppOnTouch_hook, (void**)&AppOnTouch);
#else
	A64HookFunction(GTS("_ZN7BaseApp4DrawEv"), (void*)BaseApp_draw_hook, (void**)&BaseApp_draw);
    A64HookFunction(GTS("_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi"), (void*)AppOnTouch_hook, (void**)&AppOnTouch);
#endif
}