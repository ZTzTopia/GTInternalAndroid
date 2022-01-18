#include "Gui.h"
#include "Ui.h"
#include "game/Game.h"
#include "game/Hook.h"
#include "include/ini.h"
#include "utilities/Macros.h"
#include "font/IconsFontAwesome5.h"
#include "font/FontsData.h"

#define MULT_X	0.00052083333f	// 1/1920
#define MULT_Y	0.00092592592f 	// 1/1080

namespace gui {
    Gui::Gui() 
        : m_screenSize(ImVec2()),
        m_scale(ImVec2()),
        m_needClearMousePos(true) {}

    Gui::~Gui() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void Gui::init() {
        LOGD("Initializing Gui..");

        // Setup Dear Gui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        // Setup display size
        Gui::m_screenSize.x = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeXfv"));
        Gui::m_screenSize.y = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeYfv"));
        io.DisplaySize = ImVec2(Gui::m_screenSize.x, Gui::m_screenSize.y);

        m_scale = ImVec2(io.DisplaySize.x * MULT_X, io.DisplaySize.y * MULT_Y);

        LOGD("Screen size: %f, %f", io.DisplaySize.x, io.DisplaySize.y);
        LOGD("Scale: %f, %f", m_scale.x, m_scale.y);

        // Disable loading/saving of .ini file from disk.
        io.IniFilename = nullptr;

        // Setup Dear Gui style
        SetupImGuiStyle();

        // Setup Renderer backends
        ImGui_ImplOpenGL3_Init();

        // Font awesome config.
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphMinAdvanceX = 13.0f;
        icons_config.PixelSnapH = true;
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        // Load default font.
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(arialce_compressed_data_base85,30.0f);
        // Load font awesome. (Merge to above font)
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(fasolid_compressed_data_base85, 24.0f, &icons_config, icons_ranges);

        // Load small font.
        m_small_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(arialce_compressed_data_base85, 24.0f);
        // Load font awesome. (Merge to above font)
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(fasolid_compressed_data_base85, 18.0f, &icons_config, icons_ranges);

        // Load bold font.
        m_bold_font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(arialce_bold_compressed_data_base85, 30.0f);
        // Load font awesome. (Merge to above font)
        io.Fonts->AddFontFromMemoryCompressedBase85TTF(fasolid_compressed_data_base85, 24.0f, &icons_config, icons_ranges);

        // Arbitrary scale-up
        // ImGui::GetStyle().ScaleAllSizes(3.0f);
    }

    void Gui::SetupImGuiStyle() const {
        ImGuiStyle &style = ImGui::GetStyle();
        ImGui::StyleColorsClassic();

        style.WindowPadding = ImVec2(0.0f, 0.0f);
        style.WindowBorderSize = 0.0f;
        style.WindowRounding = 4.0f;

        style.ScrollbarSize = m_scale.y * 35.0f;

        style.FramePadding = ImVec2(m_scale.x * 38.0f, 0.0f);

        style.FrameBorderSize = 0.0f;
        style.ChildBorderSize = 1.0f;

        style.ItemSpacing = ImVec2(m_scale.x * 12.0f, m_scale.y * 12.0f);

        // Colors
        style.Colors[ImGuiCol_WindowBg] = ImVec4(53.0f / 255.0f, 59.0f / 255.0f, 72.0f / 255.0f, 1.0f);

        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(47.0f / 255.0f, 54.0f / 255.0f, 64.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 0.5f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);

        style.Colors[ImGuiCol_FrameBg] = ImVec4(45.0f / 255.0f, 49.0f / 255.0f, 58.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);

        // style.Colors[ImGuiCol_Button] = ImVec4(53.0f / 255.0f, 59.0f / 255.0f, 72.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(45.0f / 255.0f, 49.0f / 255.0f, 58.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);

        style.Colors[ImGuiCol_Border] = ImVec4(156.0f / 255.0f, 136.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Gui::Begin() {
        // Start the Dear Gui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void Gui::End() {
        // Rendering
        ImGuiIO &io = ImGui::GetIO();
        ImGui::EndFrame();
        ImGui::Render();
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Gui::Render() {
        handle_soft_input();

        Gui::Begin();
        {
            ImVec2 basedWindowSize = ImVec2(m_screenSize.x / 2.0f, m_screenSize.y / 1.5f);
            ImGui::SetNextWindowSize(basedWindowSize);

            ImGui::Begin("##GTInternalAndroid", nullptr, ImGuiWindowFlags_NoDecoration);
            {
                ui::render_main();
            }
            ImGui::End();
        }
        Gui::End();

        if (m_needClearMousePos) {
            ImGuiIO &io = ImGui::GetIO();
            io.MousePos = ImVec2(-1, -1);
            m_needClearMousePos = false;
        }
    }

    void Gui::handle_soft_input() {
        // oh sad soft input :(
    }

    void Gui::OnTouchEvent(int type, bool multi, float x, float y) {
        ImGuiIO &io = ImGui::GetIO();
        switch (type) {
            case eTouchEvent::TOUCH_PUSH:
                io.MousePos = ImVec2(x, y);
                io.MouseDown[0] = true;
                break;
            case eTouchEvent::TOUCH_POP:
                io.MouseDown[0] = false;
                m_needClearMousePos = true;
                break;
            case eTouchEvent::TOUCH_MOVE:
                io.MousePos = ImVec2(x, y);
                break;
            default:
                break;
        }
    }
} // namespace Gui
