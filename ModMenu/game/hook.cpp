#include <cstdio>
#include <cstring>
#include <android/log.h>
#include <dobby.h>
#include <KittyMemory.h>

#include "../helper/hook.h"
#include "../ui/ui.h"
#include "../mod_menu.h"

extern ModMenu* g_mod_menu;

INSTALL_HOOK(BaseApp__Draw, void, void* thiz)
{
    static bool once{ false };
    if (!once) {
        auto width =
            KittyMemory::callFunction<float>(DobbySymbolResolver(nullptr, "_Z15GetScreenSizeXfv"));
        auto height =
            KittyMemory::callFunction<float>(DobbySymbolResolver(nullptr, "_Z15GetScreenSizeYfv"));

        g_mod_menu->m_ui = new ui::Ui{ ImVec2{ width, height } };
        g_mod_menu->m_ui->init();

        once = true;
    }

    g_mod_menu->m_ui->render();
    orig_BaseApp__Draw(thiz);
}

INSTALL_HOOK(AppOnTouch, void, void *a1, void *a2, int type, float x, float y, bool multi)
{
    if (g_mod_menu->m_ui && (x > 0.0f || y > 0.0f)) {
        g_mod_menu->m_ui->on_touch(type, multi, x, y);
    }

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {
        orig_AppOnTouch(a1, a2, type, x, y, multi);
    }
    else {
        orig_AppOnTouch(a1, a2, 1, 0.0f, 0.0f, false);
    }
}

namespace game {
namespace hook {
void init()
{
    // set Dobby logging level.
    log_set_level(0);

    // BaseApp::Draw(void)
    install_hook_BaseApp__Draw("_ZN7BaseApp4DrawEv");

    // AppOnTouch(_JNIEnv *,_jobject *,int,float,float,int)
    install_hook_AppOnTouch("_Z10AppOnTouchP7_JNIEnvP8_jobjectiffi");
}
} // hook
} // game
