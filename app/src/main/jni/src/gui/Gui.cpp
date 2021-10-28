#include "Gui.h"
#include "Ui.h"
#include "game/Game.h"
#include "utilities/Macros.h"

#define MULT_X	0.00052083333f	// 1/1920
#define MULT_Y	0.00092592592f 	// 1/1080

extern Game::Game* g_Game;

void SendOnTouchEvent(bool sendTruePos);

Gui::Gui::Gui() {
    m_initialized = false;
    m_screenSize = ImVec2(0.0, 0.0);

    // We need to clear the pos at first time
    m_needClearMousePos = true;
}

Gui::Gui::~Gui() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void Gui::Gui::Init() {
    if (m_initialized) {
        return;
    }

    LOGD("Initializing Gui..");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size
    Gui::m_screenSize.x = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeXfv"));
    Gui::m_screenSize.y = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeYfv"));
    io.DisplaySize = ImVec2(Gui::m_screenSize.x, Gui::m_screenSize.y);

    // Disable loading/saving of .ini file from disk.
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    SetupImGuiStyle();

    // Setup Renderer backends
    ImGui_ImplOpenGL3_Init();

    // Load Fonts
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);

    /*void* font_data = nullptr;
    int font_data_size = GetAssetData("Arial.ttf", &font_data);
    ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);*/

    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(3.0f);

    m_scale = { Gui::m_screenSize.x * MULT_X, Gui::m_screenSize.y * MULT_Y };
    m_initialized = true;
}

void Gui::Gui::SetupImGuiStyle() const {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsClassic();

    style.ScrollbarSize = m_scale.y * 25.0f;
    style.FramePadding = ImVec2(m_scale.y * 16.0f, 0.0f);
    style.WindowPadding = ImVec2(0.0f, 0.0f);
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.WindowRounding = 2.75f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(53.0f / 255.0f, 59.0f / 255.0f, 72.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(45.0f / 255.0f, 49.0f / 255.0f, 58.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Gui::Gui::Render() {
    if (!m_initialized) {
        return;
    }

    static ImGuiWindow* lastImGuiWindow = nullptr;

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // Set the next window size.
    ImVec2 basedWindowSize = { ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRS").x * 2.0f, 420 };
    ImGui::SetNextWindowSize(basedWindowSize);

    // Begin the ui window.
    ImGui::Begin("GTInternal", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    // Make only move if the mouse pos on custom title bar.
    /*if (GuiUtils::MouseOnImguiTitleBarWindow()) {
        ImGuiContext& g = *GImGui;
        g.MovingWindow = nullptr;
    }*/

    // The main ui render.
    Ui::MainRender();

    // Get last imgui window.
    lastImGuiWindow = ImGui::GetCurrentWindow();

    // Scrolling without press ScrollBar.
    // GuiUtils::ScrollWhenDraggingOnVoid();

    // End the ui window.
    ImGui::End();

    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // WTF?
    /*if (lastImGuiWindow) {
        ImGuiContext& g = *GImGui;
        if (g.NavWindow != nullptr) {
            // Auto minimize when click outside imgui.
            if (!lastImGuiWindow->Collapsed) {
                lastImGuiWindow->Collapsed = true;
            }
        } else {
            if (lastImGuiWindow->Collapsed) {
                lastImGuiWindow->Collapsed = false;
            }
        }
    }*/

    if (m_needClearMousePos) {
        io.MousePos = ImVec2(-1, -1);
        m_needClearMousePos = false;
    }
}

void Gui::Gui::OnTouchEvent(int type, __unused bool multi, float x, float y) {
    if (!m_initialized) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    switch(type) {
        case eTouchEvent::TOUCH_MOVE:
            io.MousePos = ImVec2(x, y);
            io.MouseDown[0] = true;
            break;

        case eTouchEvent::TOUCH_POP:
            io.MouseDown[0] = false;
            m_needClearMousePos = true;
            break;

        case eTouchEvent::TOUCH_PUSH:
            io.MousePos = ImVec2(x, y);
            break;

        default:
            break;
    }
}