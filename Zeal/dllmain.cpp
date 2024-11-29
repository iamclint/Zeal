#include "framework.h"
#include <thread>
#include "Zeal.h"
static HINSTANCE this_module{};
static std::thread MainLoop{};


static void zeal_lifetime_thread(std::unique_ptr<ZealService> zeal)
{
    while (!exitFlag.load(std::memory_order_acquire))
    {
        if (GetAsyncKeyState(VK_PAUSE) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000)
            break;
        Sleep(5);
    }
    if (!exitFlag.load(std::memory_order_acquire))
    {
        exitFlag.store(true, std::memory_order_release);
        zeal.reset();  // Deletes the ZealService (calling deconstructor). Note: cross-thread call with eqgame's thread.
        Sleep(1000);
        while (!FreeLibrary(this_module))
        {
            Sleep(10);
        }
        FreeLibraryAndExitThread(this_module, 0);
    }
}

static bool check_if_already_loaded()
{
    // Simple hack check is to see if client sided mana ticking was disabled (since before 0.3.0).
    // This only works if this newer Zeal is loaded second, but a 50% detection is better than none.
    //     mem::set(0x4C3F93, 0x90, 7);
    const uint8_t* ptr = reinterpret_cast<uint8_t*>(0x4C3F93);
    return *ptr == 0x90;  // Already loaded if set to a nop.
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (this_module || check_if_already_loaded())
            MessageBoxA(NULL, "Error: An extra zeal .asi file is loading. This is bad and could crash. Remove extra .asi files from eq root directory.",
                "Zeal installation error", MB_OK | MB_ICONERROR);
        else
        {
            this_module = hModule;
            DisableThreadLibraryCalls(hModule);
            auto zeal = std::make_unique<ZealService>();  // Construct before eqgame thread is started.
            MainLoop = std::thread(zeal_lifetime_thread, std::move(zeal));
            MainLoop.detach();
        }
		break;
	}
    case DLL_PROCESS_DETACH:
        exitFlag.store(true, std::memory_order_release);
        if (MainLoop.joinable())
            MainLoop.join();
        break;
    }
    return TRUE;
}

