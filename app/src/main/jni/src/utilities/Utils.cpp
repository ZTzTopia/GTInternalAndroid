#include <array>
#include <random>

#include "Utils.h"
#include "Logging.h"
#include "../include/obfuscate.h"
#include "../include/randutils.hpp"

std::mt19937 g_generator; // NOLINT(cert-msc51-cpp)

void Utils::RandomSeed() {
    g_generator.seed(randutils::auto_seed_128{}.base());
}

int Utils::Random(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(g_generator);
}

float Utils::RandomFloat(float min, float max) {
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(g_generator);
}

uintptr_t Utils::String2Offset(const char *c) {
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

/*Utils::Split Utils::Split::Parse(std::string string, const std::string& delimiter) {
    Utils::Split split{};

    bool isKey = true;
    std::size_t pos;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        std::string cropped = string.substr(0, pos);
        isKey ? split.m_array.push_back(cropped) : split.m_arrayValue.push_back(cropped);
        string.erase(0, pos + delimiter.length());
        isKey = !isKey;
    }

    if (!isKey) {
        split.m_arrayValue.push_back(string);
    }

    return split;
}

std::string Utils::Split::Get(const std::string& key) {
    if (!IsValid()) {
        return "";
    }

    for (int i = 0; i < m_array.size(); i++) {
        if (m_array.at(i).find(key) != std::string::npos) {
            return m_arrayValue.at(i);
        }
    }

    return "";
}

void Utils::Split::Set(const std::string& key, const std::string& value) {
    if (!IsValid()) {
        return;
    }

    for (int i = 0; i < m_array.size(); i++) {
        if (m_array.at(i).find(key) != std::string::npos) {
            m_arrayValue.at(i) = value;
            break;
        }
    }
}

std::string Utils::Split::Serialize(const std::string& delimiter) {
    if (!IsValid()) {
        return "";
    }

    std::string string;
    for (int i = 0; i < m_array.size(); i++) {
        string += m_array.at(i) + delimiter + m_arrayValue.at(i) + (i + 1 < m_array.size() ? delimiter : "");
    }

    return string;
}*/
