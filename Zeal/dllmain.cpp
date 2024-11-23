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



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!this_module)
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

