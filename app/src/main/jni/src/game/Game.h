#pragma once
#include "Common.h"

namespace Game {
    class Game {
    public:
        Game();
        ~Game() {};

        void Init();
        bool GetCheatState(const std::string &cheatName);

    public:
        float m_fpsLimit;
        __unused bool m_loginSpoof;
        __unused std::string m_growtopiaVersion;
        __unused std::string m_growtopiaFlags;
        std::string m_growtopiaServerIp;
        std::vector<_CheatList> m_cheatList;
    };
}