#pragma once
#include "Common.h"

class Hook {
public:
    static void Init();

public:
    static OnTouchEvent* g_onTouchEvent;
};