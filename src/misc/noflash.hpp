#pragma once

#include "../mem/mem.h"
#include "../game/game.hpp"
#include "../offsets/offsets.hpp"

namespace noflash
{
	static bool enabled = false;

	void Run()
	{
		enabled = true;
		memory::Write<float>(game::localPlayerPawn + offsets::m_flFlashMaxAlpha, 0.0f);
	}

	void Disable()
	{
		if (enabled)
		{
			enabled = false;
			memory::Write<float>(game::localPlayerPawn + offsets::m_flFlashMaxAlpha, 255.0f);
		}
	}
}