#pragma once

#define WIN32_LEAN_AND_MEAN

#include <cstddef>
#include <Windows.h>


namespace offsets 
{
	//client
	constexpr std::ptrdiff_t dwLocalPlayer = 0xDB35EC;
	constexpr std::ptrdiff_t dwForceJump = 0x52789F8;
	constexpr std::ptrdiff_t dwGlowObjectManager = 0x5316E98;
	constexpr std::ptrdiff_t dwEntityList = 0x4DCEB7C;
	 
	//player
	constexpr std::ptrdiff_t m_iHealth = 0x100;
	constexpr std::ptrdiff_t m_fFlags = 0x104;
	constexpr std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr std::ptrdiff_t m_iGlowIndex = 0x10488;
}

