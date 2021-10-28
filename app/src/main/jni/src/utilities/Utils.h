#pragma once

namespace Utilities {
    namespace Utils {
        void RandomSeed();
        int Random(int min, int max);
        float RandomFloat(float min, float max);
        uintptr_t String2Offset(const char *c);
    }

    /*class Split {
    public:
        static Split Parse(std::string string, const std::string& delimiter);
        std::string Get(const std::string& key);
        void Set(const std::string& key, const std::string& value);
        std::string Serialize(const std::string& delimiter);

        bool IsValid() { return (!m_array.empty() || !m_arrayValue.empty()); };

    private:
        std::vector<std::string> m_array;
        std::vector<std::string> m_arrayValue;
    };*/
}