#pragma once

class CrashDump {
public:
    static void Init();

private:
    static void Handler(__unused int signum, siginfo_t *info, void* contextPtr);
    static void CrashReport(ucontext* context, siginfo_t *info, const char* szTypeName);
};