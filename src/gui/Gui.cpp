#include "Gui.h"
#include "GuiUtils.h"
#include "../game/Game.h"
#include "../main.h"
#include "../utilities/Logging.h"

#define MULT_X	0.00052083333f	// 1/1920
#define MULT_Y	0.00092592592f 	// 1/1080

extern Game* g_Game;

Gui::Gui() {
    m_initialized = false;
    m_screenSize = { 0.0, 0.0 };
    m_needClearMousePos = false;
}

Gui::~Gui() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void Gui::Init() {
    if (m_initialized) {
        return;
    }

    LOGD("Initializing Gui..");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    do {
        Gui::m_screenSize.x = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeXfv"));
        Gui::m_screenSize.y = KittyMemory::callFunction<float>(GTS("_Z15GetScreenSizeYfv"));
    } while (Gui::m_screenSize.x == 0.0 || Gui::m_screenSize.y == 0.0);

    io.DisplaySize = ImVec2(Gui::m_screenSize.x, Gui::m_screenSize.y);

    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark(); // it will make style colors classic
    ImGui::StyleColorsClassic(); // it will make style colors dark

    // Setup Renderer backends
    ImGui_ImplOpenGL3_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Android: The TTF files have to be placed into the assets/ directory (android/app/src/main/assets), we use our GetAssetData() helper to retrieve them.

    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    // FIXME: Put some effort into DPI awareness.
    // Important: when calling AddFontFromMemoryTTF(), ownership of font_data is transfered by Dear ImGui by default (deleted is handled by Dear ImGui), unless we set FontDataOwnedByAtlas=false in ImFontConfig
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);
    //void* font_data;
    //int font_data_size;
    //ImFont* font;
    //font_data_size = GetAssetData("Roboto-Medium.ttf", &font_data);
    //font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);
    //IM_ASSERT(font != NULL);
    //font_data_size = GetAssetData("Cousine-Regular.ttf", &font_data);
    //font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 15.0f);
    //IM_ASSERT(font != NULL);
    //font_data_size = GetAssetData("DroidSans.ttf", &font_data);
    //font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);
    //IM_ASSERT(font != NULL);
    //font_data_size = GetAssetData("ProggyTiny.ttf", &font_data);
    //font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 10.0f);
    //IM_ASSERT(font != NULL);
    //font_data_size = GetAssetData("ArialUni.ttf", &font_data);
    //font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Arbitrary scale-up
    // FIXME: Put some effort into DPI awareness
    ImGui::GetStyle().ScaleAllSizes(3.0f);

    m_initialized = true;
}

void Gui::Render() {
    if (!m_initialized) {
        return;
    }

    // For imgui checkbox
    static bool checked = false;

    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    // Render game hack imgui window
    if (g_Game) {
        g_Game->HackRender();
    }

    // Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (m_needClearMousePos) {
        io.MousePos = ImVec2(-1, -1);
        m_needClearMousePos = false;
    }
}

bool Gui::OnTouchEvent(int type, bool multi, float x, float y) {
    if (!m_initialized) {
        return false;
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

    /*if (ImGui::IsWindowFocused()) {
        return false;
    }*/

    return true;
}
