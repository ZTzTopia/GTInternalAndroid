#include "imgui_wrapper.h"

namespace ui {
ImGuiWrapper::ImGuiWrapper(ImVec2 display_size)
    : m_display_size(display_size)
{
    m_display_scale = ImVec2(m_display_size.x * 0.00052083333f, m_display_size.y * 0.00092592592f);
}

ImGuiWrapper::~ImGuiWrapper()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

bool ImGuiWrapper::init()
{
    // Setup Dear Gui context
    IMGUI_CHECKVERSION();
    if (!ImGui::CreateContext()) {
        return false;
    }

    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = m_display_size;

    // Setup Renderer backends
    ImGui_ImplOpenGL3_Init();

    // Setup Font
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 14.0f * (m_display_scale.x + m_display_scale.y);
    io.Fonts->AddFontDefault(&font_cfg);

    // Scale All Widgets Size
    ImGui::GetStyle().ScaleAllSizes(m_display_scale.x + m_display_scale.y);
    return true;
}

void ImGuiWrapper::render() {
    // Start the Dear Gui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    draw();

    // Rendering
    ImGuiIO &io = ImGui::GetIO();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // ui
