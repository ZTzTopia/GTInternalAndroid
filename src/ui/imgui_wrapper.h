#pragma once
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>

namespace ui {
class ImGuiWrapper {
public:
    ImGuiWrapper(ImVec2 display_size);
    ~ImGuiWrapper();

    virtual bool init();
    virtual void render();

protected:
    virtual void draw() = 0;

private:
    ImVec2 m_display_size;
    ImVec2 m_display_scale;
};
} // ui
