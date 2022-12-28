#pragma once
#include <dlfcn.h>
#include <dobby.h>

#include "../utils/log.h"

#define INSTALL_HOOK(fn_name_t, fn_ret_t, fn_args_t...)                                                                    \
    fn_ret_t (*orig_##fn_name_t)(fn_args_t);                                                                               \
    fn_ret_t fake_##fn_name_t(fn_args_t);                                                                                  \
    static void install_hook_##fn_name_t(void* sym_addr)                                                                   \
    {                                                                                                                      \
        hook_function(sym_addr, (dobby_dummy_func_t)fake_##fn_name_t, (dobby_dummy_func_t*)&orig_##fn_name_t);             \
    }                                                                                                                      \
    static void install_hook_##fn_name_t(const char* lib, const char* name)                                                \
    {                                                                                                                      \
        void *sym_addr = DobbySymbolResolver(lib, name);                                                                   \
        install_hook_##fn_name_t(sym_addr);                                                                                \
    }                                                                                                                      \
    static void install_hook_##fn_name_t(const char* name)                                                                 \
    {                                                                                                                      \
        install_hook_##fn_name_t(nullptr, name);                                                                           \
    }                                                                                                                      \
    fn_ret_t fake_##fn_name_t(fn_args_t)

inline int hook_function(void* original, dobby_dummy_func_t replace, dobby_dummy_func_t* backup)
{
    Dl_info info;
    if (dladdr(original, &info)) {
        LOGI("Hooking %s (%p) from %s (%p)",
            info.dli_sname ? info.dli_sname : "(unknown symbol)", info.dli_saddr,
            info.dli_fname ? info.dli_fname : "(unknown file)", info.dli_fbase);
    }

    return DobbyHook(original, replace, backup);
}

inline int unhook_function(void* original)
{
    Dl_info info;
    if (dladdr(original, &info)) {
        LOGI("Unhooking %s (%p) from %s (%p)",
            info.dli_sname ? info.dli_sname : "(unknown symbol)", info.dli_saddr,
            info.dli_fname ? info.dli_fname : "(unknown file)", info.dli_fbase);
    }

    return DobbyDestroy(original);
}
