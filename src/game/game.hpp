#pragma once
#include <cstdint>
#include "../math/math.hpp"

namespace game
{
    inline uintptr_t clientBase;
    inline static constexpr float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    inline uintptr_t localPlayerPawn = 0, entityList = 0;
    inline int localTeam = 0;
    inline view_matrix_t view_matrix = {};
    inline Vector3 localPosition = {}, position = {}, centerPosition = {}, headPosition = {};
    inline uintptr_t listEntry = 0, controller = 0, pawnListEntry = 0, pawn = 0;
    inline uint32_t pawnHandle = 0, flags = 0;
    inline int team = 0, health = 0;
    inline float distanceToPlayer = 0.0f;
    inline Vector3 centerPos = {}, feetPos = {}, headPos = {};
    inline float height = 0.0f, width = 0.0f, headHalfHeight = 0.0f, headThickness = 0.0f, centerThickness = 0.0f;
    inline char detailsText[100] = {};
    inline int crosshairTouch = 0;
}