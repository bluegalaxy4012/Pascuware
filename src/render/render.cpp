#include "render.hpp"

void render::DrawRect(int x, int y, int w, int h, RGB color, int thickness)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), Color(color), 0.0f, 0, thickness);
}

void render::DrawLine(int x, int y, int x2, int y2, RGB color, int thickness)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddLine(ImVec2(x, y), ImVec2(x2, y2), Color(color), thickness);
}

void render::DrawCircle(int x, int y, int radius, RGB color, int segments)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddCircle(ImVec2(x, y), radius, Color(color), segments);
}

void render::DrawTextColored(int x, int y, const char* text, RGB color)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	draw_list->AddText(ImVec2(x, y), Color(color), text);
}