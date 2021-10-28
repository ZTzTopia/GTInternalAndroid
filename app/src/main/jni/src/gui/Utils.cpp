#include "Utils.h"
#include "Gui.h"
#include "../utilities/Logging.h"

bool Gui::Utils::IsAnyScrollBarActive() {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImGuiID active_id = ImGui::GetActiveID();
    return (active_id && (active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_X) || active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y)));
}

bool Gui::Utils::MouseOnImguiTitleBarWindow() {
    ImGuiContext& g = *GImGui;
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    ImRect title_bar_rect = window->TitleBarRect();

    // Maybe we no need g.HoveredWindow == window?
    if (g.HoveredWindow == window && g.HoveredId == 0 && g.HoveredIdPreviousFrame == 0 &&
        io.MousePos.x >= title_bar_rect.Min.x && io.MousePos.x <= title_bar_rect.Max.x &&
        io.MousePos.y >= title_bar_rect.Min.y && io.MousePos.y <= title_bar_rect.Max.y
    ) {
        // I don't know why if return true is detect the body.
        return false;
    }

    return true;
}

void Gui::Utils::ScrollWhenDraggingOnVoid() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    if (!IsAnyScrollBarActive() &&
        io.MousePos.x >= windowPos.x && io.MousePos.x <= windowPos.x + windowSize.x &&
        io.MousePos.y >= windowPos.y && io.MousePos.y <= windowPos.y + windowSize.y
    ) {
        ImVec2 mouse_delta = io.MouseDelta;
        if (mouse_delta.x != 0.0f && mouse_delta.x <= 100.0f) {
            ImGui::SetScrollX(ImGui::GetScrollX() + -mouse_delta.x);
        }

        if (mouse_delta.y != 0.0f && mouse_delta.y <= 100.0f) {
            ImGui::SetScrollY(ImGui::GetScrollY() + -mouse_delta.y);
        }
    }
}