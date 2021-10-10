#include "Utils.h"
#include "Logging.h"
#include "../include/obfuscate.h"

bool Utils::IsLibraryLoaded(const char *libraryName) {
    char line[512] = {0};
    FILE *fp = fopen(OBFUSCATE("/proc/self/maps"), OBFUSCATE("rt"));
    if (fp != nullptr) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (strstr(line, libraryName)) {
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}