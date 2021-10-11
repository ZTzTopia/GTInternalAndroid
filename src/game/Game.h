#pragma once
#include "../main.h"

class Game {
public:
    Game();
    ~Game() {};

    void HackRender();

    void ModFly(bool checked);

private:
    struct _GameMemoryPatch {
        MemoryPatch ModFly;
    } GameMemoryPatch;

    struct _GameHackState {
        bool ModFlyChecked;
    } GameHackState;
};
