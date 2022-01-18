#include <unordered_map>

#include "Utils.h"
#include "game/Hook.h"
#include "include/ini.h"

namespace gui {
    namespace utils {
        void text_colored_wrapped(const ImVec4& col, const char* fmt, ...) {
            ImGui::PushStyleColor(ImGuiCol_Text, col);

            va_list args;
            va_start(args, fmt);
            ImGui::TextWrappedV(fmt, args);
            va_end(args);

            ImGui::PopStyleColor();
        }

        void text_small_colored_wrapped(const ImVec4& col, const char* fmt, ...) {
            ImGui::PushFont(g_gui->m_small_font);
            ImGui::PushStyleColor(ImGuiCol_Text, col);

            va_list args;
            va_start(args, fmt);
            ImGui::TextWrappedV(fmt, args);
            va_end(args);

            ImGui::PopStyleColor();
            ImGui::PopFont();
        }

        void category(const char *label) {
            ImGui::Dummy(ImVec2(0.0f, g_gui->m_scale.y * 8.0f));
            ImGui::PushFont(g_gui->m_bold_font);
            ImGui::TextColored(ImColor(123, 129, 138, 255), label);
            ImGui::PopFont();
        }

        bool button(const char *label, const ImVec2 &size) {
            bool ret{ false };

            // ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, g_gui->m_scale.x * 2.0f);
            ImGui::PushFont(g_gui->m_small_font);
            ret = ImGui::Button(label, size);
            ImGui::PopFont();
            // ImGui::PopStyleVar();
            return ret;
        }

        static std::unordered_map<ImGuiID, std::string> g_input_text_buffer;
        static std::list<ImGuiID> g_input_text_done;

        bool input_text(const char *label, const char *label_id, char *buf, size_t buf_size,
                        ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
                        void *user_data) {
            bool ret{ false };

            const ImGuiID id{ ImGui::GetID(label_id) };

            // If we aleady have a buffer for this ID, use it.
            if (auto input_text_buffer = g_input_text_buffer.find(id); input_text_buffer != g_input_text_buffer.end()) {
                strncpy(buf, strdup(input_text_buffer->second.c_str()),
                       input_text_buffer->second.size());

                // Remove the buffer and mark it as done.
                g_input_text_buffer.erase(input_text_buffer);
                g_input_text_done.push_back(id);
            }
            else {
                // Get buffer from save file and store it.
                if (std::find(g_input_text_done.begin(), g_input_text_done.end(), id) == g_input_text_done.end()) {
                    mINI::INIFile file("/storage/emulated/0/Android/data/com.gt.launcher/files/imgui_save.ini");
                    mINI::INIStructure ini;
                    file.read(ini);

                    std::string &value = ini["userdata"]["input_text_" + std::to_string(id)];
                    g_input_text_buffer.insert_or_assign(id, value);
                }
            }

            ImVec2 frame_padding_before = ImGui::GetStyle().FramePadding;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(g_gui->m_scale.x * 16.0f, g_gui->m_scale.y * 12.0f));
            ImGui::PushFont(g_gui->m_small_font);
            ImGui::TextColored(ImColor(123, 129, 138, 255), label);
            ImGui::PopFont();

            ImGui::PushItemWidth(ImGui::GetWindowWidth() - (frame_padding_before.x * 2.0f));
            ret = ImGui::InputText(label_id, buf, buf_size, flags, callback, user_data);
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            return ret;
        }

        bool check_box(const char* label, bool* v) {
            bool ret{ false };

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, g_gui->m_scale.y * 4.0f));
            ret = ImGui::Checkbox(label, v);
            ImGui::PopStyleVar();
            return ret;
        }
    } // namespace menu
} // namespace gui