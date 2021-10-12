#include <unistd.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>

#include "main.h"
#include "game/Game.h"
#include "gui/Gui.h"
#include "utilities/Logging.h"
#include "utilities/CrashDump.h"
#include "utilities/Utils.h"

bool g_initialized = false;

void* g_GrowtopiaHandle = nullptr;
ProcMap g_GrowtopiaMap;

Game* g_Game = nullptr;
Gui* g_Gui = nullptr;

void InitHook();

void DoInitStuff() {
    if (!g_initialized) {
        g_Game = new Game;
        g_Gui = new Gui;

        // Init Gui
        g_Gui->Init();

        g_initialized = true;
    }
}

void DoMainPulse() {
    // Init stuff
    DoInitStuff();

    // Render the gui (ImGui)
    if (g_Gui) {
        g_Gui->Render();
    }
}

void* main_thread(void*) {
    // Load library
    do {
        g_GrowtopiaMap = KittyMemory::getLibraryMap("libgrowtopia.so");
        sleep(1);
    } while (!g_GrowtopiaMap.isValid());

    // This is for dlsym, dladdr
    g_GrowtopiaHandle = dlopen("/data/data/com.rtsoft.growtopia/lib/libgrowtopia.so", RTLD_NOLOAD);
    if (g_GrowtopiaHandle == nullptr) {
        LOGE("dlopen error: %s", dlerror());
    }

    LOGI("libgrowtopia.so has been loaded.");

    // Init hook
    InitHook();

    // Now we can exit the thread
    pthread_exit(nullptr);
}

__unused __attribute__((constructor)) void constructor_main() {
    LOGD("Starting Growtopia ModMenu.. Build time: " __DATE__ " " __TIME__);

    // Init crash dump
    CrashDump::Init();

    // Create a new thread because we dont want do while loop make main thread stuck
    pthread_t ptid;
    pthread_create(&ptid, nullptr, main_thread, nullptr);
}

// why need this function?
int main() {
    return 0;
}