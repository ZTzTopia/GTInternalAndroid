#pragma once
#include "Common.h"
#include "utilities/Macros.h"

namespace game {
    class Game {
    public:
        Game();
        ~Game() {};

        void init();

        bool get_cheat_state(const std::string &cheat_name);

    public:
        float m_fpsLimit;
        std::vector<_CheatList> m_cheat_list;
    }; // class Game
} // namespace game