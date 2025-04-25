#include "triggerbot.hpp"

#include "../game/game.hpp"
#include "../render/render.hpp"
#include "../mem/mem.h"
#include "../offsets/offsets.hpp"

using namespace game;

void triggerbot::Run()
{
    crosshairTouch = memory::Read<int>(localPlayerPawn + offsets::m_iIDEntIndex);

    for (int i = 1; i <= 64; i++)
    {

        if (!crosshairTouch || crosshairTouch < 0)
            continue;

        listEntry = memory::Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!listEntry)
            continue;

        controller = memory::Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
        if (!controller)
            continue;

        pawnHandle = memory::Read<uint32_t>(controller + offsets::m_hPawn);
        if (!pawnHandle)
            continue;

        pawnListEntry = memory::Read<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
        if (!pawnListEntry)
            continue;

        pawn = memory::Read<uintptr_t>(pawnListEntry + 120 * (pawnHandle & 0x1FF));
        if (!pawn || pawn == localPlayerPawn)
            continue;

        team = memory::Read<int>(pawn + offsets::m_iTeamNum);
        if (!team)
            continue;

        if (team == localTeam)
            continue;

        health = memory::Read<int>(pawn + offsets::m_iHealth);
        if (health <= 0 || health > 100)
            continue;

		static std::chrono::steady_clock::time_point lastClickTime;
		auto currentTime = std::chrono::steady_clock::now();

        if (currentTime - lastClickTime >= std::chrono::milliseconds(triggerbot::delay))
        {
			const bool isShooting = GetAsyncKeyState(VK_LBUTTON) < 0;
			if (!isShooting)
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0), mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }

    }

}