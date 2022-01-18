#pragma once

namespace utilities {
    namespace crash_dump {
        void init();
        void handler(int signum, siginfo_t *info, void *context);
        void crash_report(ucontext *context, siginfo_t *info, const char *type_name);
    } // namespace crash_dump
} // namespace utilities