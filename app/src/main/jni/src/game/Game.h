#pragma once
#include "../main.h"

class Game {
public:
    Game();
    ~Game() {};

    void HackRender();

    void ModFly(bool checked);
    void AntiCheckpoint(bool checked);

private:
    struct {
        MemoryPatch ModFly;
        MemoryPatch AntiCheckpoint;
    } m_gameHack;

    struct _stGameHackState {
        bool ModFlyChecked;
        bool AntiCheckpointChecked;
    } m_gameHackState;
};
