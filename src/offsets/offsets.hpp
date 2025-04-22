#pragma once

namespace offsets {
	constexpr std::ptrdiff_t m_iHealth = 0x344; // int32
	constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8
	constexpr std::ptrdiff_t m_hPawn = 0x62C; // CHandle<C_BasePlayerPawn>
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1324; // Vector3
	constexpr std::ptrdiff_t m_vecViewOffset = 0xCB0;
	constexpr std::ptrdiff_t m_iIDEntIndex = 0x1458;
	constexpr std::ptrdiff_t m_pClippingWeapon = 0x13A0;
	constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA;
	constexpr std::ptrdiff_t m_pEntity = 0x10;
	constexpr std::ptrdiff_t m_AttributeManager = 0x1148;
	constexpr std::ptrdiff_t m_Item = 0x50;

	constexpr std::ptrdiff_t dwEntityList = 0x1A1F670;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1874040;
	constexpr std::ptrdiff_t dwViewAngles = 0x1A93300;
	constexpr std::ptrdiff_t dwViewMatrix = 0x1A89070;
}