#include "Game.h"
#include "../gui/Gui.h"
#include "../gui/GuiUtils.h"
#include "../utilities/Logging.h"

Game::Game() {
    /*
        mov r0, #0x0
        bx lr
     */
    // 0000A0E31EFF2FE1 - To return false

    /*
        mov r0, #0x1
        bx lr
     */
    // 0100A0E31EFF2FE1 - To return true

    /*
        mov r0, #0x7f000000
        bx lr
     */
    // 7F04A0E31EFF2FE1 - To return high value

    /*
        nop
     */
    // 00F020E3 - NOP

    // NetMoving::collide() -> WorldTileMap::Collide()
    m_gameHack.ModFly = MemoryPatch::nopPatch(GT(0xb5da1c), 1);

    // Tile::IsCheckpoint()
    m_gameHack.AntiCheckpoint = MemoryPatch::createWithHex(GT(0x88c4b0), "0000A0E31EFF2FE1");

    m_gameHackState.ModFlyChecked = false;
    m_gameHackState.AntiCheckpointChecked = false;
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
