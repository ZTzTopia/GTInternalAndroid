#include "ui.h"

namespace ui {
Ui::Ui(ImVec2 display_size)
    : ImGuiWrapper(display_size)
    , m_clear_pos(true) {}

void Ui::render()
{
    ImGuiWrapper::render();

    if (m_clear_pos) {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        m_clear_pos = false;
    }
}

void Ui::draw()
{
    static bool open = true;
    ImGui::ShowDemoWindow(&open);
}

void Ui::on_touch(int type, bool multi, float x, float y) {
    ImGuiIO &io = ImGui::GetIO();
    switch (type) {
    case 1:
        io.MouseDown[0] = false;
        m_clear_pos = true;
        break;
    case 2:
        io.MousePos = ImVec2(x, y);
        io.MouseDown[0] = true;
        break;
    case 3:
        io.MousePos = ImVec2(x, y);
        break;
    default:
        break;
    }
}
} // ui
