#include "Ui.h"
#include "Gui.h"
#include "TextEditor.h"
#include "Utils.h"
#include "game/Game.h"
#include "game/Hook.h"
#include "include/KittyMemory/MemoryPatch.h"
#include "font/IconsFontAwesome5.h"

namespace gui {
    namespace ui {
        void render_main() {
            static uint8_t tab_page = 0;

            ImGuiStyle &style = ImGui::GetStyle();

            // Title bar.
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(p.x, p.y),
                    ImVec2(p.x + ImGui::GetWindowWidth(), p.y + ImGui::GetCursorPosY() + 7 +
                    style.ItemSpacing.y + ImGui::GetFontSize() + ImGui::GetCursorPosY() + 12 +
                    ImGui::CalcTextSize("ABC").x),
                    ImColor(47, 54, 64, 255),
                    4.0f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::Text("GTInternal");
            ImGui::SameLine();
            ImGui::TextColored(ImColor(117, 119, 123, 255), "v0.1.0");
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

            utils::BeginTab("##ModMenuTab", 3, false, ImVec2(0.0f, ImGui::CalcTextSize("ABC").x));

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            if (utils::AddTab(ICON_FA_ROCKET" Cheats")) {
                tab_page = 0;
            }

            if (utils::AddTab(ICON_FA_ROBOT" Bot")) {
                tab_page = 1;
            }

            if (utils::AddTab(ICON_FA_CODE" Lua Executor")) {
                tab_page = 2;
            }
            ImGui::PopStyleColor(3);

            utils::EndTabs();

            // The line below tab.
            p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(p.x, p.y + 1),
                    ImVec2(p.x + ImGui::GetWindowSize().x, p.y + 4),
                    ImColor(37, 43, 51, 255));
            ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(p.x + (((ImGui::GetWindowSize().x - style.WindowPadding.x * 2.0f) - 0.0f) / 3) * tab_page, p.y + 2),
                    ImVec2(p.x + (((ImGui::GetWindowSize().x - style.WindowPadding.x * 2.0f) - 0.0f) / 3) * (tab_page + 1), p.y + 4),
                    ImColor(156, 136, 255, 255));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, g_gui->m_scale.x * 8.0f);
            ImGui::BeginChild("##CheatChild", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y * 0.75f), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

            switch (tab_page) {
                case 0:
                    render_cheat_content();
                    break;
                case 1:
                    render_bot_content();
                    break;
                case 2:
                    render_lua_executor_content();
                    break;
                default:
                    break;
            }

            ImGui::EndChild();
            ImGui::PopStyleVar(2);
        }

        void render_cheat_content() {
            utils::category("All");

            // Checkbox
            MemoryPatch memory_patch{};
            for (auto& cheatList : g_game->m_cheat_list) {
                if (utils::check_box(cheatList.name.c_str(), &cheatList.state)) {
                    if (cheatList.active != nullptr) {
                        cheatList.active();
                    }
                }
                else {
                    if (cheatList.deactive != nullptr) {
                        cheatList.deactive();
                    }
                }

                if (cheatList.old_state != cheatList.state) {
                    if (!cheatList.memory_patch_list.empty()) {
                        for (auto& memoryPatch2 : cheatList.memory_patch_list) {
                            cheatList.state ? memoryPatch2.Modify() : memoryPatch2.Restore();
                        }
                    }

                    cheatList.old_state = cheatList.state;
                }
            }
        }

        void render_bot_content() {
            utils::category("All");

            static char buf[32]{ 0 };
            utils::input_text("Name", "##Name", buf, IM_ARRAYSIZE(buf),
                              ImGuiInputTextFlags_CharsNoBlank);

            static char buf2[18]{ 0 };
            utils::input_text("Password", "##Password", buf2, IM_ARRAYSIZE(buf2),
                              ImGuiInputTextFlags_Password);

            if (g_port != 65535) {
                if (utils::button("Login", ImVec2(0.0f, ImGui::GetFontSize() * 2.0f))) {
                    // Connect to server.
                }
            }
            else {
                utils::text_small_colored_wrapped(ImColor(255, 92, 92, 255), "This is use auto port "
                                                                "via growtopia hook so "
                                                                "you need to login with "
                                                                "your account first!");
            }
        }

        void render_lua_executor_content() {
            utils::category("All");

            if (utils::button(ICON_FA_FILE" Load from file", ImVec2(0.0f, ImGui::GetFontSize() * 2.0f))) {
                // Load a file.
            }

            static TextEditor editor;

            editor.SetShowWhitespaces(false);
            editor.SetReadOnly(true);
            editor.SetPalette(TextEditor::GetDarkPalette());
            editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());

            ImVec2 frame_padding_before = ImGui::GetStyle().FramePadding;
            editor.Render("##EditorWindow", ImVec2(ImGui::GetWindowWidth() - (frame_padding_before.x * 2.0f), 300));

            ImGui::Dummy(ImVec2(0.0f, ImGui::GetStyle().ItemSpacing.y * 0.15f));
            if (utils::button(ICON_FA_CHECK" Execute", ImVec2(0.0f, ImGui::GetFontSize() * 2.0f))) {
                // Run script.
            }
            ImGui::SameLine();
            if (utils::button(ICON_FA_STOP" Stop", ImVec2(0.0f, ImGui::GetFontSize() * 2.0f))) {
                // Stop script.
            }
            ImGui::SameLine();
            if (utils::button(ICON_FA_TRASH" Clear", ImVec2(0.0f, ImGui::GetFontSize() * 2.0f))) {
                editor.SetText("");
            }
        }
    } // namespace ui
} // namespace gui