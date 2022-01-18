#pragma once
#include "Gui.h"

namespace gui {
    namespace utils {
        void text_colored_wrapped(const ImVec4& col, const char* fmt, ...);
        void text_small_colored_wrapped(const ImVec4& col, const char* fmt, ...);
        void category(const char *label);
        bool button(const char *label, const ImVec2 &size = ImVec2(0.0f, 0.0f));
        bool input_text(const char *label, const char *label_id, char *buf, size_t buf_size,
                        ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = __null,
                        void *user_data = __null);
        bool check_box(const char* label, bool* v);
    } // namespace menu

    namespace utils {
        struct TabsDesc {
            __int32_t lableCount;
            ImVec2 lableSize;
            float lableSpacing;
            bool leftTabs;
            __int32_t currentidx;
        };

        struct Tabs {
            TabsDesc* tbd;
            ImGuiID ID;
            __int32_t selectedIdx;
        };

        static ImVector<Tabs*> CacheTabs;
        static Tabs* CurTabs;

        inline void BeginTab(const char* name, int lablesCount, bool leftTabs, ImVec2 lableSize, float lableSpacing = 0.0f) {
            //Find exists Tabs
            Tabs* exsisttbs = NULL;
            ImGuiID id = ImHash(name, 0);
            for (int i = 0; i < CacheTabs.Size; i++) {
                if (CacheTabs[i]->ID == id) {
                    exsisttbs = CacheTabs[i];
                }
            }

            if (exsisttbs == NULL) {
                Tabs* tbs = (Tabs*)ImGui::MemAlloc(sizeof(Tabs));
                tbs->selectedIdx = 0;
                tbs->ID = id;
                CacheTabs.insert(CacheTabs.begin(), tbs);
                CurTabs = tbs;
            }
            else {
                CurTabs = exsisttbs;
            }

            TabsDesc* tbd = (TabsDesc*)ImGui::MemAlloc(sizeof(TabsDesc));
            tbd->lableCount = lablesCount;
            tbd->leftTabs = leftTabs;
            tbd->lableSpacing = lableSpacing;
            tbd->currentidx = 0;
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGuiStyle& style = ImGui::GetStyle();
            tbd->lableSize.x = leftTabs ? lableSize.x : ((windowSize.x - style.WindowPadding.x * 2.0f) - lableSpacing) / lablesCount;
            tbd->lableSize.y = lableSize.y;
            CurTabs->tbd = tbd;
        }

        inline void BeginTab(const char* name, int lablesCount, float lableSpacing = 0.0f) {
            BeginTab(name, lablesCount, false, { 0.0f, 0.0f }, lableSpacing);
        }

        inline void BeginLeftTab(const char* name, ImVec2 lableSize = { 0.0f, 0.0f }, float lableSpacing = 0.0f) {
            BeginTab(name, 0, true, lableSize, lableSpacing);
        }

        inline void EndTabs() {
            ImGui::MemFree(CurTabs->tbd);
            CurTabs = NULL;
        }

        inline bool AddTab(const char* label, bool newLine = false) {
            TabsDesc* tbs = CurTabs->tbd;
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec2 itemSpacing = style.ItemSpacing;
            ImVec4 color = style.Colors[ImGuiCol_Button];
            ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
            ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];

            style.ItemSpacing.x = tbs->lableSpacing;
            style.ItemSpacing.y = tbs->lableSpacing;

            if (!newLine && !tbs->leftTabs && tbs->currentidx > 0) {
                ImGui::SameLine();
            }

            // push the style
            if (tbs->currentidx == CurTabs->selectedIdx) {
                style.Colors[ImGuiCol_Button] = colorActive;
                style.Colors[ImGuiCol_ButtonActive] = colorActive;
                style.Colors[ImGuiCol_ButtonHovered] = colorActive;
            }
            else {
                style.Colors[ImGuiCol_Button] = color;
                style.Colors[ImGuiCol_ButtonActive] = colorActive;
                style.Colors[ImGuiCol_ButtonHovered] = colorHover;
            }

            // Draw the button
            if (ImGui::Button(label, ImVec2(tbs->lableSize.x, tbs->lableSize.y))) {
                CurTabs->selectedIdx = tbs->currentidx;
            }

            // Restore the style
            style.Colors[ImGuiCol_Button] = color;
            style.Colors[ImGuiCol_ButtonActive] = colorActive;
            style.Colors[ImGuiCol_ButtonHovered] = colorHover;
            style.ItemSpacing = itemSpacing;
            return CurTabs->selectedIdx == tbs->currentidx++;
        }
    } // namespace utils
} // namespace gui