#include "framework.h"
#include <thread>
#include "Zeal.h"
static HINSTANCE this_module{};
static std::thread MainLoop{};


void init()
{
    ZealService zeal;
    while (!exitFlag.load(std::memory_order_acquire))
    {
        if (GetAsyncKeyState(VK_PAUSE) & 0x8000 && GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000)
            break;
        Sleep(5);
    }
    if (!exitFlag.load(std::memory_order_acquire))
    {
        exitFlag.store(true, std::memory_order_release);
        zeal.~ZealService();
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
            MainLoop = std::thread(init);
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

