#pragma once
#include "../main.h"

class Game {
public:
    Game();
    ~Game() {};

    void HackRender();

    void ModFly(bool checked);
    void AntiCheckpoint(bool checked);
	void FastFall(bool checked);

private:
    struct {
        MemoryPatch ModFly;
        MemoryPatch AntiCheckpoint;
		MemoryPatch FastFall;
    } m_gameHack;

    struct _stGameHackState {
        bool ModFlyChecked;
        bool AntiCheckpointChecked;
		bool FastFallChecked;
    } m_gameHackState;
};
