#pragma once
#include "imgui_wrapper.h"

namespace ui {
class Ui : public ImGuiWrapper {
public:
    Ui(ImVec2 display_size);
    ~Ui() = default;

    void render() override;
    void draw() override;

    void on_touch(int type, bool multi, float x, float y);

private:
    bool m_clear_pos;
};
} // ui
