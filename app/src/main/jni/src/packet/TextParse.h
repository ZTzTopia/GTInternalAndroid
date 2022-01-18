#pragma once
#include <string>
#include <vector>

#include "utilities/Utils.h"

namespace packet {
    class TextParse {
    public:
        TextParse() {};
        TextParse(const std::string &data) {
            m_data = utilities::utils::string_tokenize(data, "\n");
            for (unsigned int i = 0; i < m_data.size(); i++) {
                utilities::utils::string_replace(m_data[i], "\r", "");
            }
        };
        ~TextParse() {};

        template <typename T = std::string>
        T get(const std::string &key, int index, const std::string &token = "|") {
            if (m_data.empty()) {
                return "";
            }

            for (unsigned int i = 0; i < m_data.size(); i++) {
                if (m_data[i].empty()) {
                    continue;
                }

                std::vector<std::string> data = utilities::utils::string_tokenize(m_data[i], token);
                if (data[0] == key) {
                    // Found it.
                    return data[index];
                }
            }

            return "";
        }

        template <typename T>
        void add(const std::string &key, const T &value, const std::string &token = "|") {
            // m_data.push_back(key + token + (std::is_integral<T>::value ? std::to_string(value) : value));
        }

        template <typename T>
        void set(const std::string &key, const T &value, const std::string &token = "|") {
            if (m_data.empty()) {
                return;
            }

            for (unsigned int i = 0; i < m_data.size(); i++) {
                std::vector<std::string> data = utilities::utils::string_tokenize(m_data[i], token);
                if (data[0] == key) {
                    m_data[i] = data[0];
                    m_data[i] += token;
                    // m_data[i] += std::is_integral<T>::value ? std::to_string(value) : value;
                    break;
                }
            }
        }

        void get_all_raw(std::string &data) {
            if (m_data.empty()) {
                return;
            }

            std::string string{};
            for (unsigned int i = 0; i < m_data.size(); i++) {
                string += m_data[i];
                if (!m_data[i + 1].empty()) {
                    string += "\n";
                }
            }

            data = string;
        }

        size_t get_line_count() {
            return m_data.size();
        }

    private:
        std::vector<std::string> m_data;
    }; // class TextParse
} // namespace packet