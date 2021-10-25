#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLES2/gl2.h>

class Gui {
public:
    Gui();
    ~Gui();

    void Init();
    void SetupImGuiStyle() const;
    void Render();
    void OnTouchEvent(int type, __unused  bool multi, float x, float y);

private:
    enum eTouchEvent {
        TOUCH_MOVE,
        TOUCH_POP,
        TOUCH_PUSH
    };

public:
    ImVec2 m_screenSize;
    ImVec2 m_scale;

private:
    bool m_initialized;
    bool m_needClearMousePos;
};

namespace ImGui {
    static bool custom_UseFontOutline;
    static unsigned int custom_FontOutlineColor;
    static unsigned int custom_FontOutlineSize;

    inline static void PushFontOutline(unsigned int col, unsigned int outlineSize = 1) {
        custom_UseFontOutline = true;
        custom_FontOutlineColor = col;
        custom_FontOutlineSize = outlineSize;
    }

    inline static void PopFontOutline(void) {
        custom_UseFontOutline = false;
    }

    struct TabsDesc {
        __int32	lableCount;
        ImVec2 lableSize;
        float lableSpacing;
        bool leftTabs;
        __int32 currentidx;
    };

    struct Tabs {
        TabsDesc* tbd;
        ImGuiID ID;
        __int32 selectedIdx;
    };

    static ImVector<Tabs*> CacheTabs;
    static Tabs* CurTabs;

    inline void BeginTabs(const char* name, int lablesCount, bool leftTabs, ImVec2 lableSize, float lableSpacing = 0.0f) {
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
        tbd->lableSize.y = lableSize.y;
        tbd->lableSize.x = leftTabs ? lableSize.x : ((windowSize.x - style.WindowPadding.x * 2.0f) - lableSpacing) / lablesCount;
        CurTabs->tbd = tbd;
    }

    inline void BeginTabs(const char* name, int lablesCount, float lableSpacing = 0.0f) {
        BeginTabs(name, lablesCount, false, { 0.0f, 0.0f }, lableSpacing);
    }

    inline void BeginLeftTabs(const char* name, ImVec2 lableSize = { 0.0f, 0.0f }, float lableSpacing = 0.0f) {
        BeginTabs(name, 0, true, lableSize, lableSpacing);
    }

    inline void EndTabs() {
        MemFree(CurTabs->tbd);
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

        tbs->currentidx++;
        return CurTabs->selectedIdx == tbs->currentidx;
    }
}