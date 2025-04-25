#pragma once

#include <thread>
#include <chrono>
#include "../mem/mem.h"
#include "../game/game.hpp"
#include "../offsets/offsets.hpp"

namespace bunnyhop
{
	void Run()
	{
		memory::Write<DWORD>(game::clientBase + offsets::dwForceJump, 65537);
		std::this_thread::sleep_for(std::chrono::microseconds(1600)); //pt 1.6
		memory::Write<DWORD>(game::clientBase + offsets::dwForceJump, 256);
		std::this_thread::sleep_for(std::chrono::microseconds(1600));
	}
}