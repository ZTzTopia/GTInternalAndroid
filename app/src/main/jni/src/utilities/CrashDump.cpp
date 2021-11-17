#include <iostream>
#include <csignal>
#include <dlfcn.h>

#include "CrashDump.h"
#include "../main.h"
#include "Logging.h"

void PrintSymbol(void* pc, void* lr) {
    Dl_info info_pc, info_lr;
    if (dladdr(pc, &info_pc) != 0) {
        if (info_pc.dli_sname != "(null)") {
            LOGE("> lr: %s", info_pc.dli_sname);
        }
    }

    if (dladdr(lr, &info_lr) != 0) {
        if (info_lr.dli_sname != "(null)") {
            LOGE("> lr: %s", info_lr.dli_sname);
        }
    }
}

void CrashReport(ucontext* context, siginfo_t *info, const char* szTypeName) {
#if defined(__arm__)
    LOGE("SIGNO: %s | Fault address: 0x%X", szTypeName, info->si_addr);
    LOGE("SIGACTION signal %d (si_code: %d si_errno: %d si_signo: %d)", info, info->si_code, info->si_errno, info->si_signo);

    LOGE("register states:");
    LOGE("r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X", context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
    LOGE("r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
    LOGE("r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
    LOGE("ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);

    LOGE("backtrace:");
    LOGE("1: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.arm_pc, context->uc_mcontext.arm_pc - (int)g_GrowtopiaMap.startAddr);
    LOGE("2: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.arm_lr, context->uc_mcontext.arm_lr - (int)g_GrowtopiaMap.startAddr);

    PrintSymbol((void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));
#endif
// TODO: for arm64
    exit(0);
}

void handler1(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGSEGV) {
        CrashReport(context, info, "SIGSEGV");
    }
}

void handler2(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGABRT) {
        CrashReport(context, info, "SIGABRT");
    }
}

void handler3(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGFPE) {
        CrashReport(context, info, "SIGFPE");
    }
}

void handler4(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGBUS) {
        CrashReport(context, info, "SIGBUS");
    }
}

void handler5(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGILL) {
        CrashReport(context, info, "SIGILL");
    }
}

void handler6(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if (info->si_signo == SIGPIPE) {
        CrashReport(context, info, "SIGPIPE");
    }
}

void handler7(int signum, siginfo_t *info, void* contextPtr) {
    ucontext* context = (ucontext_t*)contextPtr;
    if(info->si_signo == SIGSTKFLT) {
        CrashReport(context, info, "SIGSTKFLT");
    }
}

void CrashDump::Init() {
    LOGD("Initializing CrashDump..");

    struct sigaction act;
    act.sa_sigaction = handler1;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, nullptr);

    struct sigaction act2;
    act2.sa_sigaction = handler2;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act2, nullptr);

    struct sigaction act3;
    act3.sa_sigaction = handler3;
    sigemptyset(&act3.sa_mask);
    act3.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &act3, nullptr);

    struct sigaction act4;
    act4.sa_sigaction = handler4;
    sigemptyset(&act4.sa_mask);
    act4.sa_flags = SA_SIGINFO;
    sigaction(SIGBUS, &act4, nullptr);

    // inline hook caused sigill:(
    /*struct sigaction act5;
    act5.sa_sigaction = handler5;
    sigemptyset(&act5.sa_mask);
    act5.sa_flags = SA_SIGINFO;
    sigaction(SIGILL, &act5, nullptr);*/

    struct sigaction act6;
    act6.sa_sigaction = handler6;
    sigemptyset(&act6.sa_mask);
    act6.sa_flags = SA_SIGINFO;
    sigaction(SIGPIPE, &act6, nullptr);

    struct sigaction act7;
    act7.sa_sigaction = handler7;
    sigemptyset(&act7.sa_mask);
    act7.sa_flags = SA_SIGINFO;
    sigaction(SIGSTKFLT, &act7, nullptr);
}
