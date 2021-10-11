#include "Game.h"
#include "../gui/Gui.h"
#include "../gui/GuiUtils.h"

Game::Game() {
    // NetMoving::collide() -> WorldTileMap::Collide()
    GameMemoryPatch.ModFly = MemoryPatch::createWithHex(GT(0xb5da1c), "00F020E3");
    GameHackState.ModFlyChecked = false;
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
    ImGui::Checkbox("Mod Fly", &GameHackState.ModFlyChecked);
    ModFly(GameHackState.ModFlyChecked);

    // Scrolling without press ScrollBar
    GuiUtils::ScrollWhenDraggingOnVoid();

    // End imgui window
    ImGui::End();
}

void Game::ModFly(bool checked) {
    static bool old = false;
    if (old != checked) {
        checked ? GameMemoryPatch.ModFly.Modify() : GameMemoryPatch.ModFly.Restore();
        old = checked;
    }
}
