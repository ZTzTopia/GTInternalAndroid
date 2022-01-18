#pragma once
#include <vector>

namespace utilities {
    namespace utils {
        uintptr_t String2Offset(const char *c);

        std::string GenerateRandomName();
        std::string GenerateRandomNumber(size_t length);
        std::string GenerateRandomHex(size_t length, bool uppercase = false);
        std::string GenerateRandomMac();

        int HashString(const char* data, int length);
        int GetDeviceHash();
        int GetDeviceSecondaryHash();

        void string_replace(std::string &str, const std::string &from, const std::string &to);
        std::vector<std::string> string_tokenize(const std::string &str, const std::string &delimiters);
    } // namespace utils
} // namespace utilities