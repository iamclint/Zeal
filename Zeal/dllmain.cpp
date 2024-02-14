#include "framework.h"
#include <thread>
#include "Zeal.h"
HINSTANCE this_module;

void init()
{
    ZealService zeal{};
    while (!zeal.exit && !(GetAsyncKeyState(VK_PAUSE) & 0x01 && GetAsyncKeyState(VK_SHIFT)))
    {
        Sleep(100);
    }

     zeal.~ZealService(); //destructor
    
     Sleep(1000);

    while (!FreeLibrary(this_module))
    {
        Sleep(10);
    }
    FreeLibraryAndExitThread(this_module, 0);
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
            std::thread t(init);
            t.detach();
        }
		break;
	}
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        ZealService::get_instance()->exit = true;
        break;
    }
    return TRUE;
}

