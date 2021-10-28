#pragma once

namespace Utilities {
    namespace CrashDump {
        void Init();
        void Handler(__unused int signum, siginfo_t *info, void *contextPtr);
        void CrashReport(ucontext *context, siginfo_t *info, const char *szTypeName);
    }
}