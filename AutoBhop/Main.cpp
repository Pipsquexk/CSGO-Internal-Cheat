#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <thread>

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


struct Color
{
    constexpr Color(float r, float g, float b, float a = 1.0f) noexcept :
        r(r), g(g), b(b), a(a) {}

    float r, g, b, a;
};

void MainThread(const HMODULE instance) noexcept
{
    const Color red = Color(1.f, 0.f, 0.f);
    const Color green = Color(0.f, 1.f, 0.f);

    const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("client.dll"));

    while (!GetAsyncKeyState(VK_END))
    {
        const auto pLocalPlayer = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwLocalPlayer);

        if (!pLocalPlayer) continue;

        if (GetAsyncKeyState(VK_SPACE))
        {
            const auto health = *reinterpret_cast<std::int32_t*>(pLocalPlayer + offsets::m_iHealth);

            if (!health) continue;

            const auto flags = *reinterpret_cast<std::int32_t*>(pLocalPlayer + offsets::m_fFlags);

            *reinterpret_cast<std::uintptr_t*>(client + offsets::dwForceJump) = ((flags & (1 << 0)) ? 6 : 4);
        }


        const auto glowObjectManager = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwGlowObjectManager);

        for (int i = 0; i < 64; ++i)
        {
            const auto entity = *reinterpret_cast<std::uintptr_t*>(client + offsets::dwEntityList + i * 0x10);

            if (!entity) continue;

            const auto glowIndex = *reinterpret_cast<std::uintptr_t*>(entity + offsets::m_iGlowIndex);

            *reinterpret_cast<bool*>(glowObjectManager + (glowIndex * 0x38) + 0x27) = true;
            *reinterpret_cast<bool*>(glowObjectManager + (glowIndex * 0x38) + 0x28) = true;

            if (*reinterpret_cast<std::uintptr_t*>(entity + offsets::m_iTeamNum) != *reinterpret_cast<std::uintptr_t*>(pLocalPlayer + offsets::m_iTeamNum))
            {
                *reinterpret_cast<Color*>(glowObjectManager + (glowIndex * 0x38) + 0x8) = red;
            }
            else
            {
                *reinterpret_cast<Color*>(glowObjectManager + (glowIndex * 0x38) + 0x8) = green;
            }

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }

    FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(
    const HMODULE instance,  // handle to DLL module
    const std::uintptr_t reason,     // reason for calling function
    const void* reserved)  // reserved
{
    if (DLL_PROCESS_ATTACH == reason)
    {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread),
            instance,
            0,
            nullptr
        );

        if (thread) CloseHandle(thread);
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}