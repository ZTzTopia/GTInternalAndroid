// TODO: obfuscate all string (ida 7.6 can deobfuscate the string)

#include <unistd.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>
#include <dirent.h>

#include "main.h"
#include "utilities/Logging.h"
#include "utilities/CrashDump.h"
#include "utilities/Utils.h"

void* g_GrowtopiaHandle = nullptr;
ProcMap g_GrowtopiaMap;

void InitHook();

void* main_thread(void*) {
    do {
        g_GrowtopiaMap = KittyMemory::getLibraryMap("libgrowtopia.so");
        sleep(1);
    } while (!g_GrowtopiaMap.isValid());

    g_GrowtopiaHandle = dlopen("/data/data/com.rtsoft.growtopia/lib/libgrowtopia.so", RTLD_NOLOAD);
    if (g_GrowtopiaHandle == nullptr) {
        LOGE("dlopen error: %s", dlerror());
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