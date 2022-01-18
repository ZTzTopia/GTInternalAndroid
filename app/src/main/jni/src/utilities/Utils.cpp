#include <array>
#include <random>

#include "Utils.h"
#include "Random.h"
#include "include/obfuscate.h"
#include "Macros.h"

namespace utilities {
    namespace utils {
        uintptr_t String2Offset(const char *c) {
            int base = 16;
            // See if this function catches all possibilities.
            // If it doesn't, the function would have to be amended
            // whenever you add a combination of architecture and
            // compiler that is not yet addressed.
            static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                          || sizeof(uintptr_t) == sizeof(unsigned long long),
                          "Please add string to handle conversion for this architecture.");

            // Now choose the correct function ...
            if (sizeof(uintptr_t) == sizeof(unsigned long)) {
                return strtoul(c, nullptr, base);
            }

            // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
            return strtoull(c, nullptr, base);
        }

        std::string GenerateRandomName() {
            static std::array<std::string, 26> random_name = {
                    "Alfa",
                    "Bravo",
                    "Charlie",
                    "Delta",
                    "Echo",
                    "Foxtrot",
                    "Golf",
                    "Hotel",
                    "India",
                    "Juliet",
                    "Kilo",
                    "Lima",
                    "Mike",
                    "November",
                    "Oscar",
                    "Papa",
                    "Quebec",
                    "Romeo",
                    "Sierra",
                    "Tango",
                    "Uniform",
                    "Victor",
                    "Whiskey",
                    "X-ray",
                    "Yankee",
                    "Zulu"
            };
            return utilities::random::generator_static().pick(random_name);
        }

        std::string GenerateRandomNumber(size_t length) {
            std::string string{};
            string.reserve(length);
            while (length-- > 0) {
                string += std::to_string(utilities::random::generator_static().uniform(0, 9));
            }

            return string;
        }

        std::string GenerateRandomHex(size_t length, bool uppercase) {
            std::string string{};
            string.reserve(length);
            while (length-- > 0) {
                string += std::to_string(utilities::random::generator_static().uniform(0, 15));
            }

            if (uppercase) {
                std::transform(string.begin(), string.end(), string.begin(), ::toupper);
            }

            return string;
        }

        std::string GenerateRandomMac() {
            std::string string{};
            string.reserve(17);
            for (int i = 0; i < 6; i++) {
                string += GenerateRandomHex(2, false);
                if (i < 5) {
                    string += ":";
                }
            }

            return string;
        }

        int HashString(const char* data, int length) {
            int hash = 0x55555555;
            if (data) {
                if (length >= 1) {
                    while (length) {
                        hash = (hash >> 27) + (hash << 5) + *data++;
                        length--;
                    }
                }
                else {
                    while (*data) {
                        hash = (hash >> 27) + (hash << 5) + *data++;
                    }
                }
            }

            return hash;
        }

        int GetDeviceHash() {
            std::string deviceId = GenerateRandomNumber(8);
            if (!deviceId.empty()) {
                deviceId.append("RT");
                return HashString(deviceId.c_str(), 0);
            }

            return 0;
        }

        int GetDeviceSecondaryHash() {
            std::string mac = GenerateRandomMac();
            if (!mac.empty()) {
                mac.append("RT");
                return HashString(mac.c_str(), 0);
            }

            return 0;
        }

        void string_replace(std::string &str, const std::string &from, const std::string &to) {
            size_t start_pos = 0;
            while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
        }

        std::vector<std::string> string_tokenize(const std::string &str, const std::string &delimiters) {
            std::vector<std::string> tokens;
            std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
            std::string::size_type pos = str.find_first_of(delimiters, lastPos);
            while (std::string::npos != pos || std::string::npos != lastPos) {
                tokens.push_back(str.substr(lastPos, pos - lastPos));
                lastPos = str.find_first_not_of(delimiters, pos);
                pos = str.find_first_of(delimiters, lastPos);
            }

            return tokens;
        }
    } // namespace utils
} // namespace utilities