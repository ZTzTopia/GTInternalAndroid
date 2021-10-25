#include "Ui.h"
#include "Gui.h"
#include "GuiUtils.h"
#include "../game/Game.h"
#include "../game/Common.h"

extern Game* g_Game;

void Ui::MainRender() {
    ImGuiStyle& style = ImGui::GetStyle();

    // TitleBar.
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 58), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(47, 54, 64, 255));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("GTInternal");
    ImGui::SameLine();
    ImGui::TextColored(ImColor(117, 119, 123, 255), "v0.0.2");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    // Begin the ModMenu tab.
    ImGui::BeginTabs("MMTab", 3, false, { 0.0f, ImGui::CalcTextSize("ABC").x });

    ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::AddTab("Cheats");
    ImGui::AddTab("Players");
    ImGui::AddTab("Items");
    ImGui::PopStyleColor(3);

    // End the ModMenu tab.
    ImGui::EndTabs();

    p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 2), ImVec2(p.x + ImGui::GetWindowSize().x, p.y + 3), ImColor(37, 43, 51, 255));
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 2), ImVec2(p.x + ((ImGui::GetWindowSize().x - style.WindowPadding.x * 2.0f)) / 3, p.y + 3), ImColor(156, 136, 255, 255));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 12);

    // Begin the cheat child.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.75f);
    ImGui::BeginChild("CheatChild", { ImGui::GetWindowSize().x, 260}, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    // Render cheat ui.
    Ui::CheatRender();

    // End the cheat child.
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
}

void Ui::CheatRender() {
    ImGui::TextColored(ImColor(123, 129, 138, 255), "All");

    // Checkbox
    for (stCheatList& cheatList : g_Game->m_cheatList) {
        if (ImGui::Checkbox(cheatList.cheatName.c_str(), &cheatList.cheatState)) {
            if (cheatList.cheatActive != nullptr) {
                cheatList.cheatActive();
            }
        }
        else {
            if (cheatList.cheatDeactive != nullptr) {
                cheatList.cheatDeactive();
            }
        }

//#ifdef __arm__
        if (cheatList.cheatOldState != cheatList.cheatState) {
//#endif
            cheatList.cheatState ? cheatList.cheatMemPatch.Modify() : cheatList.cheatMemPatch.Restore();
//#ifdef __arm__
            cheatList.cheatOldState = cheatList.cheatState;
        }
//#endif
    }
}
