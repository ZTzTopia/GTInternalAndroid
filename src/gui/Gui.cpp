#include "Gui.h"
#include "../main.h"
#include "../utilities/Logging.h"

static bool g_initialized = false;

#define MULT_X	0.00052083333f	// 1/1920
#define MULT_Y	0.00092592592f 	// 1/1080

void Gui::Init() {
    if (g_initialized || Gui::m_screenSize.x == 0.0 && Gui::m_screenSize.y == 0.0) {
        return;
    }

    LOGD("Initializing Gui..");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // TODO: find screen size in growtopia lib
    io.DisplaySize = ImVec2(Gui::m_screenSize.x, Gui::m_screenSize.y);

    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Renderer backends
    ImGui_ImplOpenGL3_Init("#version 100");

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

    g_initialized = true;
}

void Gui::Shutdown() {
    if (!g_initialized) {
        return;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    g_initialized = false;
}

void Gui::Render() {
    if (!g_initialized) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!", nullptr);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

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

bool Gui::OnTouchEvent(int type, float x, float y) {
    if (!g_initialized) {
        return false;
    }
    
    ImGuiIO& io = ImGui::GetIO();

    // 2 = MOVE, 1 = POP, 0 = PUSH
    switch(type) {
        case 0:
            io.MousePos = ImVec2(x, y);
            io.MouseDown[0] = true;
            break;

        case 1:
            io.MouseDown[0] = false;
            m_needClearMousePos = true;
            break;

        case 2:
            io.MousePos = ImVec2(x, y);
            break;

        default:
            break;
    }

    return true;
}

ImVec2 Gui::m_screenSize {0.0, 0.0};
bool Gui::m_needClearMousePos {false};