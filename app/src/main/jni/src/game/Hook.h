#pragma once
#include "Common.h"

namespace Game {
    class Hook {
    public:
        static void Init();

    public:
        static OnTouchEvent *g_onTouchEvent;
    };
}