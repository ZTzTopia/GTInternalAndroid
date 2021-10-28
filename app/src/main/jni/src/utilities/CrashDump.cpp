#include <csignal>
#include <dlfcn.h>

#include "CrashDump.h"
#include "Macros.h"

void Utilities::CrashDump::Init() {
    LOGD("Initializing CrashDump..");

    struct sigaction act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, nullptr);

    act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGBUS, &act, nullptr);

    act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGILL, &act, nullptr);

    act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGFPE, &act, nullptr);

    act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGABRT, &act, nullptr);

    act = {};
    act.sa_sigaction = CrashDump::Handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGTERM, &act, nullptr);
}

void Utilities::CrashDump::Handler(__unused int signum, siginfo_t *info, void* contextPtr) {
    auto* context = reinterpret_cast<ucontext_t *>(contextPtr);
    switch (info->si_signo) {
        case SIGSEGV:
            CrashReport(context, info, "SIGSEGV");
            break;
        case SIGBUS:
            CrashReport(context, info, "SIGBUS");
            break;
        case SIGILL:
            CrashReport(context, info, "SIGILL");
            break;
        case SIGFPE:
            CrashReport(context, info, "SIGFPE");
            break;
        case SIGABRT:
            CrashReport(context, info, "SIGABRT");
            break;
        case SIGTERM:
            CrashReport(context, info, "SIGTERM");
            break;
    }
}

void Utilities::CrashDump::CrashReport(ucontext* context, siginfo_t *info, const char* typeName) {
    Dl_info info_pc, info_lr;

#ifdef __arm__
    LOGE("SIGNO: %s | Fault address: 0x%X", typeName, info->si_addr);
    LOGE("SIGACTION signal %d (si_code: %d si_errno: %d si_signo: %d)", info, info->si_code, info->si_errno, info->si_signo);

    LOGE("register states:");
    LOGE("r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X", context->uc_mcontext.arm_r0, context->uc_mcontext.arm_r1, context->uc_mcontext.arm_r2, context->uc_mcontext.arm_r3);
    LOGE("r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.arm_r4, context->uc_mcontext.arm_r5, context->uc_mcontext.arm_r6, context->uc_mcontext.arm_r7);
    LOGE("r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.arm_r8, context->uc_mcontext.arm_r9, context->uc_mcontext.arm_r10, context->uc_mcontext.arm_fp);
    LOGE("ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.arm_ip, context->uc_mcontext.arm_sp, context->uc_mcontext.arm_lr, context->uc_mcontext.arm_pc);

    LOGE("backtrace:");
    LOGE("pc: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.arm_pc, context->uc_mcontext.arm_pc - (uintptr_t)g_GrowtopiaMap.startAddr);
    LOGE("lr: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.arm_lr, context->uc_mcontext.arm_lr - (uintptr_t)g_GrowtopiaMap.startAddr);

    LOGE("backtrace symbol:");
    if (dladdr(reinterpret_cast<void *>(context->uc_mcontext.arm_pc), &info_pc) != 0) {
        LOGE("pc: %s", info_pc.dli_sname);
    }

    if (dladdr(reinterpret_cast<void *>(context->uc_mcontext.arm_lr), &info_lr) != 0) {
        LOGE("lr: %s", info_lr.dli_sname);
    }
#elif __aarch64__
    LOGE("SIGNO: %s | Fault address: 0x%X", typeName, info->si_addr);
    LOGE("SIGACTION signal %d (si_code: %d si_errno: %d si_signo: %d)", info, info->si_code, info->si_errno, info->si_signo);

    LOGE("register states:");
    LOGE("r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X", context->uc_mcontext.regs[0], context->uc_mcontext.regs[1], context->uc_mcontext.regs[2], context->uc_mcontext.regs[3]);
    LOGE("r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x", context->uc_mcontext.regs[4], context->uc_mcontext.regs[5], context->uc_mcontext.regs[6], context->uc_mcontext.regs[7]);
    LOGE("r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x", context->uc_mcontext.regs[8], context->uc_mcontext.regs[9], context->uc_mcontext.regs[10], context->uc_mcontext.regs[11]);
    LOGE("ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x", context->uc_mcontext.regs[12], context->uc_mcontext.sp, context->uc_mcontext.regs[14], context->uc_mcontext.pc);
    LOGE("r16: 0x%X, r17: 0x%X, r18: 0x%X, r19: 0x%X", context->uc_mcontext.regs[16], context->uc_mcontext.regs[17], context->uc_mcontext.regs[18], context->uc_mcontext.regs[19]);
    LOGE("r22: 0x%X, r23: 0x%X, r24: 0x%X, r25: 0x%X", context->uc_mcontext.regs[22], context->uc_mcontext.regs[23], context->uc_mcontext.regs[24], context->uc_mcontext.regs[25]);
    LOGE("r24: 0x%X, r25: 0x%X, r26: 0x%X, r27: 0x%X", context->uc_mcontext.regs[24], context->uc_mcontext.regs[24], context->uc_mcontext.regs[25], context->uc_mcontext.regs[26]);
    LOGE("r28: 0x%X, r29: 0x%X, r30: 0x%X, r31: 0x%X", context->uc_mcontext.regs[28], context->uc_mcontext.regs[29], context->uc_mcontext.regs[30], context->uc_mcontext.regs[31]);

    LOGE("backtrace:");
    LOGE("pc: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.pc, context->uc_mcontext.pc - (int)g_GrowtopiaMap.startAddr);
    LOGE("lr: 0x%X (libgrowtopia: 0x%X)", context->uc_mcontext.regs[14], context->uc_mcontext.regs[14] - (int)g_GrowtopiaMap.startAddr);

    LOGE("backtrace symbol:");
    if (dladdr(reinterpret_cast<void *>(context->uc_mcontext.pc), &info_pc) != 0) {
        LOGE("pc: %s", info_pc.dli_sname);
    }

    if (dladdr(reinterpret_cast<void *>(context->uc_mcontext.regs[14]), &info_lr) != 0) {
        LOGE("lr: %s", info_lr.dli_sname);
    }
#endif

    exit(0);
}