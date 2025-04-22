#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

struct RGB {
	ImU32 R, G, B, A;
};


inline ImU32 Color(RGB color)
{
	return IM_COL32(color.R, color.G, color.B, color.A);
}

namespace render {
	void DrawRect(int x, int y, int w, int h, RGB color, int thickness);

	void DrawLine(int x, int y, int x2, int y2, RGB color, int thickness);

	void DrawCircle(int x, int y, int radius, RGB color, int segments);

	void DrawTextColored(int x, int y, const char* text, RGB color);
}
