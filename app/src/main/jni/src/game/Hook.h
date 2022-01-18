#pragma once
#include <enet/enet.h>

#include "Common.h"
#include "Game.h"
#include "gui/Gui.h"

extern gui::Gui *g_gui;
extern game::Game *g_game;
extern enet_uint16 g_port;

namespace game {
    namespace hook {
        void init();
    } // namespace hook
} // namespace game