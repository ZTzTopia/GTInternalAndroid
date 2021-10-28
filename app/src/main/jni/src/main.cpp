/*
 * TODO:
 * Add log to file.
 * Fix Mod Fly cheat.
 * Fix arm64 need 2 times Modify() to modify the hex.
 * Fix KittyMemory.
 */

#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>
#include <pthread.h>
#include <unistd.h>

#include "main.h"
#include "game/Hook.h"
#include "utilities/CrashDump.h"
#include "utilities/Utils.h"

void *g_GrowtopiaHandle = nullptr;
KittyMemory::ProcMap g_GrowtopiaMap = {};

void *main_thread(void *) {
    sleep(1);

    Utilities::CrashDump::Init();

    // Initialize random seed for Random and RandomFloat. We need to give time for the
    // seed random generator to work.
    Utilities::Utils::RandomSeed();

    do {
        g_GrowtopiaMap = KittyMemory::getLibraryMap("libgrowtopia.so");
        sleep(1);
    } while (!g_GrowtopiaMap.isValid());

    // This is used for dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym.
    // Just open the dynamic library don't load it.
    g_GrowtopiaHandle = dlopen("libgrowtopia.so", RTLD_NOLOAD);
    if (g_GrowtopiaHandle == nullptr) {
        LOGE("%s", dlerror());
    }

    // Check if growtopia map and growtopia handle is valid.
    if (g_GrowtopiaMap.isValid() && g_GrowtopiaHandle != nullptr) {
        Game::Hook::Init();
    }

    // Now we can exit the thread.
    pthread_exit(nullptr);
}

__unused __attribute__((constructor)) void constructor_main() {
    LOGI("Starting Growtopia ModMenu.. Build time: " __DATE__ " " __TIME__);

    // Create a new thread because we don't want do while loop make main thread
    // stuck.
    pthread_t ptid;
    pthread_create(&ptid, nullptr, main_thread, nullptr);
}
