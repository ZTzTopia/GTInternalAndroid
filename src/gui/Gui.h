#pragma once
#ifdef __ANDROID__
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <GLES2/gl2.h>
#endif

class Gui {
public:
    static void Init();
    static void Shutdown();
    static void Render();
    static bool OnTouchEvent(int type, float x, float y);

public:
    static ImVec2 m_screenSize;
    static bool m_needClearMousePos;
};