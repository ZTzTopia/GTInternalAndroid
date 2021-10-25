#include "Game.h"
#include "../gui/Gui.h"
#include "../gui/GuiUtils.h"
#include "../utilities/Logging.h"

Game::Game() {
    m_cheatList.clear();

    m_fpsLimit = 60.0f;
    m_loginSpoof = true;
    m_growtopiaVersion = "3.71";
    m_growtopiaFlags = "??";
    m_growtopiaServerIp = "??";
}

void Game::Init() {
    LOGD("Initializing Game..");

#ifdef __arm__
    std::string RETFALSE = "0000A0E31EFF2FE1";
    std::string RETTRUE = "0100A0E31EFF2FE1";
    std::string RETFLOAT1 = "FE05A0E31EFF2FE1";
#elif __aarch64__
    std::string RETFALSE = "000080D2C0035FD6";
    std::string RETTRUE = "200080D2C0035FD6";
    std::string RETFLOAT1 = "00F0A7D2C0035FD6";
#endif

    struct stCheatList cheatList = {};
    cheatList.cheatName = OBFUSCATE("Mod Fly");
#ifdef __arm__
    cheatList.cheatAddr = GT("0xb5da1c"); // NetMoving::collide() -> WorldTileMap::Collide()
#elif __aarch64__
    cheatList.cheatAddr = GT("0xd022dc"); // NetMoving::collide() -> WorldTileMap::Collide()
#endif
    cheatList.cheatMemPatch = MemoryPatch::nopPatch(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), 1);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Anti Checkpoint";
    cheatList.cheatAddr = GTS("_ZN4Tile12IsCheckpointEv"); // Tile::IsCheckpoint()
    cheatList.cheatHex = RETFALSE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Anti Portal";
    cheatList.cheatAddr = GTS("_ZN9NetAvatar9OnUseDoorEP4Tileb"); // NetAvatar::OnUseDoor()
    cheatList.cheatHex = RETFALSE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Anti Punch";
    cheatList.cheatAddr = GTS("_ZN9NetAvatar9OnPunchedE7CL_Vec2IfEPS_"); // NetAvatar::OnPunched()
    cheatList.cheatHex = RETFALSE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Fast Fall";
    cheatList.cheatAddr = GTS("_ZN9NetMoving7collideESsffP11CollideInfoib"); // NetMoving::collide()
    cheatList.cheatHex = RETFALSE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "See Ghost";
    cheatList.cheatAddr = GTS("_Z12CanSeeGhostsi"); // CanSeeGhosts()
    cheatList.cheatHex = RETTRUE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "See Fruits";
    cheatList.cheatAddr = GTS("_ZN4Tile28GetFruitBloomProgressPercentEv"); // Tile::GetFruitBloomProgressPercent()
    cheatList.cheatHex = RETFLOAT1;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Anti Bounce";
    cheatList.cheatAddr = GTS("_ZN9NetAvatar8OnDamageEv"); // NetAvatar::OnDamage()
    cheatList.cheatHex = RETFALSE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);

    cheatList = {};
    cheatList.cheatName = "Night Vision";
    cheatList.cheatAddr = GTS("_ZN11PlayerItems13NightVisionOnEv"); // PlayerItems::NightVisionOn()
    cheatList.cheatHex = RETTRUE;
    cheatList.cheatMemPatch = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(cheatList.cheatAddr), cheatList.cheatHex);
    m_cheatList.push_back(cheatList);
}

bool Game::GetCheatState(const std::string& cheatName) {
    for (const stCheatList& cheatList : m_cheatList) {
        if (cheatList.cheatName.find(cheatName) != -1) {
            return cheatList.cheatState;
        }
    }

    return false;
}