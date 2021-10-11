#include <unistd.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>

#include "main.h"
#include "utilities/Logging.h"
#include "utilities/CrashDump.h"
#include "utilities/Utils.h"

void* g_GrowtopiaAddress = nullptr;
ProcMap g_GrowtopiaMap;

Gui* pGui = nullptr;

void InitHook();

void InitInMenu() {
    static bool initialized = false;

    if (!initialized) {
        pGui = new Gui();
        pGui->Init();

        initialized = true;
    }
}

void DoMainPulse() {
    InitInMenu();

    if (pGui) {
        pGui->Render();
    }
}

void* main_thread(void*) {
    do {
        g_GrowtopiaMap = KittyMemory::getLibraryMap("libgrowtopia.so");
        sleep(1);
    } while (!g_GrowtopiaMap.isValid());

    g_GrowtopiaAddress = g_GrowtopiaMap.startAddr;
    if (g_GrowtopiaAddress == nullptr) {
        LOGE("Can not find growtopia start address!");
    }

    LOGI("libgrowtopia.so has been loaded.");

    InitHook();

    pthread_exit(nullptr);
}

__attribute__((constructor)) void constructor_main() {
    LOGD("Starting Growtopia ModMenu.. Build time: " __DATE__ " " __TIME__);
    CrashDump::Init();

    pthread_t ptid;
    pthread_create(&ptid, nullptr, main_thread, nullptr);
}

// why need this function?
int main() {
    return 0;
}