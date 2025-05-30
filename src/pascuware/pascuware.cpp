#include "pascuware.hpp"

#include <Windows.h>

#include "../render/render.hpp"
#include "../triggerbot/triggerbot.hpp"
#include "../game/game.hpp"
#include "../mem/mem.h"
#include "../offsets/offsets.hpp"
#include "../esp/esp.hpp"
#include "../misc/bunnyhop.hpp"
#include "../misc/noflash.hpp"

using namespace game;


void pascuware::Run()
{
	static auto lastTime = std::chrono::steady_clock::now();
	auto currentTime = std::chrono::steady_clock::now();

	if ((GetAsyncKeyState(VK_INSERT) & 1) && currentTime - lastTime >= std::chrono::milliseconds(200))
	{
		config::activeMenu = !config::activeMenu;
		lastTime = currentTime;
	}

	if (config::activeMenu)
	{
		RunMenu();
	}

	localPlayerPawn = memory::Read<uintptr_t>(clientBase + offsets::dwLocalPlayerPawn);
	if (!localPlayerPawn)
		return;

	entityList = memory::Read<uintptr_t>(clientBase + offsets::dwEntityList);
	if (!entityList)
		return;

	localTeam = memory::Read<int>(localPlayerPawn + offsets::m_iTeamNum);
	if (!localTeam)
		return;

	view_matrix = memory::Read<view_matrix_t>(clientBase + offsets::dwViewMatrix);

	localPosition = memory::Read<Vector3>(localPlayerPawn + offsets::m_vOldOrigin);



	if (config::activeESP)
		esp::Run();

	if (config::activeTriggerbot &&  ( triggerbot::key == -1 || ( GetAsyncKeyState(triggerbot::key) != 0 ) ))
		triggerbot::Run();

	if (config::activeBunnyhop && (GetAsyncKeyState(VK_SPACE) & 0x8000))
		bunnyhop::Run();

	if (config::activeNoFlash)
		noflash::Run();
	else noflash::Disable();


	int x_debugDraw = config::screenSize.GetWidth() * 3 / 4;
	render::DrawTextColored(x_debugDraw, 5, "Running", { 255, 125, 125, 255 });

	char buffer[100];
	sprintf_s(buffer, "ESP: %d Triggerbot: %d, Bunnyhop: %d, NoFlash: %d", config::activeESP, config::activeTriggerbot, config::activeBunnyhop, config::activeNoFlash);
	render::DrawTextColored(x_debugDraw, 20, buffer, { 255, 125, 125, 255 });


	return;
}

void pascuware::RunMenu()
{
	ImGui::Begin("PASCUWARE", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::BeginTabBar("Tabs");
		if (ImGui::BeginTabItem("ESP"))
		{
			ImGui::Checkbox("ESP", &config::activeESP);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Triggerbot"))
		{
			ImGui::Checkbox("Triggerbot", &config::activeTriggerbot);

			if (triggerbot::key == -1)
				triggerbot::keyIndex = 0;
			else if (triggerbot::key == VK_LMENU)
				triggerbot::keyIndex = 1;

			if (ImGui::Combo("Triggerbot Key", &triggerbot::keyIndex, "Automatic\0Left Alt\0"))
			{
				switch (triggerbot::keyIndex)
				{
					case 0:
						triggerbot::key = -1;
						break;	

					case 1:
						triggerbot::key = VK_LMENU;
						break;
				}
			}

            ImGui::InputInt("Triggerbot Delay(ms)", (int*)&triggerbot::delay, 0, 0, ImGuiInputTextFlags_None);
			
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Misc"))
		{
			ImGui::Checkbox("Bunnyhop", &config::activeBunnyhop);
			ImGui::Checkbox("No Flash", &config::activeNoFlash);
			ImGui::EndTabItem();
		}

		ImGui::Separator();
		ImGui::Text("[Insert] - Toggle Menu");
		ImGui::EndTabBar();
	}ImGui::End();


	return;
}