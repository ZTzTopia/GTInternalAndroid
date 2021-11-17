#include "Game.h"
#include "../gui/Gui.h"
#include "../gui/GuiUtils.h"
#include "../utilities/Logging.h"
#include "../utilities/Macros.h"

#ifdef __arm__
    std::string RETFALSE = "0000A0E31EFF2FE1";
    std::string RETTRUE = "0100A0E31EFF2FE1";
    std::string RETFLOAT1 = "FE05A0E31EFF2FE1";
#elif __aarch64__
    std::string RETFALSE = "000080D2C0035FD6";
    std::string RETTRUE = "200080D2C0035FD6";
    std::string RETFLOAT1 = "00F0A7D2C0035FD6";
#endif

Game::Game() {
    // NetMoving::collide() -> WorldTileMap::Collide()
    m_gameHack.ModFly = MemoryPatch::nopPatch(GT(0xb5da1c), 1);

    // Tile::IsCheckpoint()
    m_gameHack.AntiCheckpoint = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(GTS("_ZN4Tile12IsCheckpointEv")), RETFALSE);
	
    // NetMoving::collide()
    m_gameHack.FastFall = MemoryPatch::createWithHex(reinterpret_cast<uintptr_t>(GTS("_ZN9NetMoving7collideESsffP11CollideInfoib")), RETFALSE);

    m_gameHackState.ModFlyChecked = false;
    m_gameHackState.AntiCheckpointChecked = false;
	m_gameHackState.FastFallChecked = false;
}

void Game::HackRender() {
    // Begin imgui window
    ImGui::Begin("Growtopia", nullptr, ImGuiWindowFlags_NoResize);

    // Make only move if the mouse pos on title bar
    if (GuiUtils::MouseOnImguiTitleBarWindow()) {
        ImGuiContext& g = *GImGui;
        g.MovingWindow = nullptr;
    }

    // Checkbox
    ImGui::Checkbox("Mod Fly", &m_gameHackState.ModFlyChecked);
    ModFly(m_gameHackState.ModFlyChecked);

    ImGui::Checkbox("Anti Checkpoint", &m_gameHackState.AntiCheckpointChecked);
    AntiCheckpoint(m_gameHackState.AntiCheckpointChecked);

	ImGui::Checkbox("Fast Fall", &m_gameHackState.FastFallChecked);
    AntiCheckpoint(m_gameHackState.FastFallChecked);

    // Scrolling without press ScrollBar
    GuiUtils::ScrollWhenDraggingOnVoid();

    // End imgui window
    ImGui::End();
}

void Game::ModFly(bool checked) {
    static bool old = false;
    if (old != checked) {
        checked ? m_gameHack.ModFly.Modify() : m_gameHack.ModFly.Restore();
        old = checked;
    }
}

void Game::AntiCheckpoint(bool checked) {
    static bool old = false;
    if (old != checked) {
        checked ? m_gameHack.AntiCheckpoint.Modify() : m_gameHack.AntiCheckpoint.Restore();
        old = checked;
    }
}

void Game::FastFall(bool checked) {
	static bool old = false;
    if (old != checked) {
        checked ? m_gameHack.FastFall.Modify() : m_gameHack.FastFall.Restore();
        old = checked;
    }
}
