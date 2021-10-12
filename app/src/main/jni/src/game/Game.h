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
    struct _stGameMemoryPatch {
        MemoryPatch ModFly, AntiCheckpoint;
    } stGameMemoryPatch;

    struct _stGameHackState {
        bool ModFlyChecked, AntiCheckpointChecked;
    } stGameHackState;
};
