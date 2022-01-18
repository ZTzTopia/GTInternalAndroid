#pragma once
#include <GLES2/gl2.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>

namespace gui {
    class Gui {
    public:
        Gui();
        ~Gui();

        void init();
        void Begin();
        void End();

        void SetupImGuiStyle() const;
        void Render();

        void handle_soft_input();
        void OnTouchEvent(int type, __unused bool multi, float x, float y);

    private:
        enum eTouchEvent {
            TOUCH_PUSH,
            TOUCH_POP,
            TOUCH_MOVE
        };

    public:
        ImVec2 m_screenSize;
        ImVec2 m_scale;
        uint8_t m_wait_until_next_frame;
        ImFont *m_small_font;
        ImFont *m_bold_font;

    private:
        bool m_needClearMousePos;
    }; // class Gui
} // namespace gui