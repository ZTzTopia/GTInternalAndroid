#include "Game.h"
#include "packet/Common.h"
#include "utilities/Macros.h"
#include "utilities/Utils.h"

namespace game {
    Game::Game()
        : m_fpsLimit(60.0f) {}

    void Game::init() {
        LOGD("Initializing Game..");

#ifdef __arm__
        std::string RETFALSE = "0000A0E31EFF2FE1";
        std::string RETTRUE = "0100A0E31EFF2FE1";
        std::string RETFLOAT1 = "FE05A0E31EFF2FE1";
        std::string NOP = "00F020E3";
#elif __aarch64__
        std::string RETFALSE = "000080D2C0035FD6";
        std::string RETTRUE = "200080D2C0035FD6";
        std::string RETFLOAT1 = "00F0A7D2C0035FD6";
        std::string NOP = "1F2003D5";
#endif

        struct _CheatList cheat_list{};
        cheat_list.name = "Mod Fly";
#ifdef __arm__
        cheat_list.memory_patch.insert({reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GTS("_ZN9NetMoving7collideESsffP11CollideInfoib")) + 84), NOP}); // NetMoving::collide() -> WorldTileMap::Collide()
#elif __aarch64__
        cheat_list.memory_patch.insert({reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(GTS("_ZN9NetMoving7collideESsffP11CollideInfoib")) + 80), NOP}); // NetMoving::collide() -> WorldTileMap::Collide()
#endif
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Anti Checkpoint";
        cheat_list.memory_patch.insert({GTS("_ZN4Tile12IsCheckpointEv"), RETFALSE}); // Tile::IsCheckpoint()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Anti Portal";
        cheat_list.memory_patch.insert({GTS("_ZN9NetAvatar9OnUseDoorEP4Tileb"), RETFALSE}); // NetAvatar::OnUseDoor()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Anti Punch";
        cheat_list.memory_patch.insert({GTS("_ZN9NetAvatar9OnPunchedE7CL_Vec2IfEPS_"), RETFALSE}); // NetAvatar::OnPunched()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Fast Fall";
        cheat_list.memory_patch.insert({GTS("_ZN9NetMoving7collideESsffP11CollideInfoib"), RETFALSE}); // NetMoving::collide()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "See Ghost";
        cheat_list.memory_patch.insert({GTS("_Z12CanSeeGhostsi"), RETTRUE}); // CanSeeGhosts()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "See Fruits";
        cheat_list.memory_patch.insert({GTS("_ZN4Tile28GetFruitBloomProgressPercentEv"), RETFLOAT1}); // Tile::GetFruitBloomProgressPercent()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Night Vision";
        cheat_list.memory_patch.insert({GTS("_ZN11PlayerItems13NightVisionOnEv"), RETTRUE}); // PlayerItems::NightVisionOn()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Fast Fall v2";
        cheat_list.memory_patch.insert({GTS("_ZN9NetMoving7collideESsffP11CollideInfoib"), RETTRUE}); // NetMoving::collide()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Can't Jump";
        cheat_list.memory_patch.insert({GTS("_ZN9NetAvatar6OnJumpEv"), RETFALSE}); // NetAvatar::OnJump()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Frog Mode";
        cheat_list.memory_patch.insert({GTS("_ZN9NetMoving8collideYER11CollideInfo"), RETFALSE}); // NetMoving::collideY()
        m_cheat_list.push_back(cheat_list);

        cheat_list = {};
        cheat_list.name = "Float";
        cheat_list.memory_patch.insert({GTS("_ZN9NetMoving7collideESsffP11CollideInfoib"), RETTRUE}); // NetMoving::collide()
        cheat_list.memory_patch.insert({GTS("_ZN9NetAvatar6OnJumpEv"), RETFALSE}); // NetAvatar::OnJump()
        m_cheat_list.push_back(cheat_list);

        for (auto& cheat_list2 : m_cheat_list) {
            for (auto& memory_patch : cheat_list2.memory_patch) {
                cheat_list2.memory_patch_list.push_back(MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(memory_patch.first), memory_patch.second));
            }
        }
    }

    bool Game::get_cheat_state(const std::string& cheat_name) {
        for (auto& cheat_list : m_cheat_list) {
            if (cheat_list.name.find(cheat_name) != std::string::npos) {
                return cheat_list.state;
            }
        }

        return false;
    }
} // namespace game