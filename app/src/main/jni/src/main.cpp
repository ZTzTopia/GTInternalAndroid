/*
 *
 * DEVELOPER: ZTz
 * PROJECT: GTInternalAndroid
 * VERSION: 0.3
 *
 * ============================================================================
 *
 * Features:
 * Many useful cheats.
 * Debug enet client packet.
 * Multi bot.
 * And many useful features.
 *
 */

#include <android/log.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>

#include "Main.h"
#include "game/Hook.h"
#include "utilities/CrashDump.h"

JavaVM *g_java_vm{ nullptr };
void *g_growtopia_handle{ nullptr };
KittyMemory::ProcMap g_growtopia_map{};

void *main_thread(void *) {
    utilities::crash_dump::init();

    do {
        g_growtopia_map = KittyMemory::getLibraryMap(AY_OBFUSCATE("libgrowtopia.so"));
        sleep(1);
    } while (!g_growtopia_map.isValid());

    do {
        // This is used for dladdr, dlclose, dlerror, dlopen, dlsym, dlvsym.
        // Just open the dynamic library don't load it.
        g_growtopia_handle = dlopen(AY_OBFUSCATE("libgrowtopia.so"), RTLD_NOLOAD);
        sleep(1);
    } while (g_growtopia_handle == nullptr);

    game::hook::init();

    // Now we can exit the thread.
    pthread_exit(nullptr);
}

__unused __attribute__((constructor))
void constructor_main() {
    LOGI(AY_OBFUSCATE("Starting Growtopia ModMenu.. Build time: " __DATE__ " " __TIME__));

    // Create a new thread because we don't want do while loop make main thread
    // stuck.
    pthread_t pthread_id{};
    pthread_create(&pthread_id, nullptr, main_thread, nullptr);
}
