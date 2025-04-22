#include "esp.hpp"

#include "../game/game.hpp"
#include "../render/render.hpp"
#include "../mem/mem.h"
#include "../offsets/offsets.hpp"

using namespace game;

void esp::Run()
{
    for (int i = 1; i <= 64; i++)
    {
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

        health = memory::Read<int>(pawn + offsets::m_iHealth);
        if (health <= 0 || health > 100)
            continue;

        position = memory::Read<Vector3>(pawn + offsets::m_vOldOrigin);
        if (!position.x && !position.y && !position.z)
            continue;

        distanceToPlayer = position.distance(localPosition);
        if (distanceToPlayer > 2500.f)
            continue;

        centerPosition = { position.x, position.y, position.z + 32.5f };
        headPosition = { position.x, position.y, position.z + 65.f };

        centerPos = centerPosition.WorldToScreen(view_matrix);
        if (centerPos.z < 0.01f)
            continue;

        feetPos = position.WorldToScreen(view_matrix);
        headPos = headPosition.WorldToScreen(view_matrix);

        height = feetPos.y - headPos.y;
        width = height * 0.416f;
        headHalfHeight = height * 0.0675f;

        headThickness = (distanceToPlayer > 1000.f) ? 2.5f : 5.0f;
        centerThickness = (distanceToPlayer > 1000.f) ? 0.3f : 1.f;

        render::DrawCircle(headPos.x, headPos.y + headHalfHeight, headThickness, { 255, 125, 0, 255 }, 10);
        render::DrawRect(headPos.x - width * 0.5f, headPos.y, width, height, { 255, 0, 0, 255 }, centerThickness);

        snprintf(detailsText, sizeof(detailsText), "HP: %d\n", health);
        render::DrawTextColored(centerPos.x, centerPos.y - 20, detailsText, { 255, 255, 255, 255 });
    }
}