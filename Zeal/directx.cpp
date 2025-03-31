#include "directx.h"
#include "Zeal.h"
#pragma comment(lib, "d3dx8/d3d8.lib")
#pragma comment(lib, "d3dx8/d3dx8.lib")
#include <chrono>
HRESULT WINAPI Local_BeginScene(LPDIRECT3DDEVICE8 pDevice)
{

    if (pDevice)
    {
        static LARGE_INTEGER last_frame = {};
        static LARGE_INTEGER frequency = {};

        if (frequency.QuadPart == 0)
            QueryPerformanceFrequency(&frequency);

        int fps_limit_val = ZealService::get_instance()->dx->fps_limit.get();
        HRESULT ret = ZealService::get_instance()->hooks->hook_map["BeginScene"]->original(Local_BeginScene)(pDevice);

        if (fps_limit_val > 0)
        {
            double frame_time = 1.0 / fps_limit_val; // Desired frame time in seconds
            LARGE_INTEGER current_time;
            QueryPerformanceCounter(&current_time);

            double elapsed_time = (double)(current_time.QuadPart - last_frame.QuadPart) / frequency.QuadPart;
            double sleep_time = frame_time - elapsed_time;

            //Use this while loop for precise waiting, sleep has too much variation
            if (sleep_time > 0)
            {
                LARGE_INTEGER wait_until;
                QueryPerformanceCounter(&wait_until);
                while (((double)(wait_until.QuadPart - current_time.QuadPart) / frequency.QuadPart) < sleep_time)
                    QueryPerformanceCounter(&wait_until);
            }
        }

        QueryPerformanceCounter(&last_frame);
        return ret;
    }
    return ZealService::get_instance()->hooks->hook_map["BeginScene"]->original(Local_BeginScene)(pDevice);
}


HRESULT WINAPI Local_EndScene(LPDIRECT3DDEVICE8 pDevice)
{
    HRESULT ret = ZealService::get_instance()->hooks->hook_map["EndScene"]->original(Local_EndScene)(pDevice);
    if (ZealService::get_instance()->callbacks)
        ZealService::get_instance()->callbacks->invoke_generic(callback_type::EndScene);
    return ret;
}

HRESULT WINAPI Local_Reset(IDirect3DDevice8* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    if (ZealService::get_instance()->callbacks)
        ZealService::get_instance()->callbacks->invoke_generic(callback_type::DXReset);
    HRESULT ret = ZealService::get_instance()->hooks->hook_map["Reset"]->original(Local_Reset)(pDevice, pPresentationParameters);
    if (ZealService::get_instance()->callbacks)
        ZealService::get_instance()->callbacks->invoke_generic(callback_type::DXResetComplete);
    return ret;
}

void directx::update_device()
{
    HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
    if (eqfx)
        device = *(IDirect3DDevice8**)((DWORD)eqfx + 0xa4f92c);
    else
        device = nullptr;

    if (device)
    {
        uintptr_t* vtable = *(uintptr_t**)device;
        if (vtable)
        {
            DWORD endscene_addr = (DWORD)vtable[35];
            DWORD beginscene_addr = (DWORD)vtable[34];
            DWORD reset_addr = (DWORD)vtable[14];
            if (!ZealService::get_instance()->hooks->hook_map.count("EndScene"))
                ZealService::get_instance()->hooks->Add("EndScene", endscene_addr, Local_EndScene, hook_type_detour);
            if (!ZealService::get_instance()->hooks->hook_map.count("BeginScene"))
                ZealService::get_instance()->hooks->Add("BeginScene", beginscene_addr, Local_BeginScene, hook_type_detour);
            if (!ZealService::get_instance()->hooks->hook_map.count("Reset"))
                ZealService::get_instance()->hooks->Add("Reset", reset_addr, Local_Reset, hook_type_detour);
        }
     //   ZealService::get_instance()->hooks->Add("Reset", reset_addr, Local_Reset, hook_type_detour);
    }
}

Vec2 directx::GetScreenRect()
{
    update_device();
    if (!device)
        return { 0,0 };
    D3DVIEWPORT8 viewport;
    device->GetViewport(&viewport);
    return { (float)viewport.Width, (float)viewport.Height };
}


bool IsOffScreen(Vec2 screenPos, const D3DVIEWPORT8& viewport) {
    return screenPos.x < viewport.X || screenPos.x > viewport.X + viewport.Width ||
        screenPos.y < viewport.Y || screenPos.y > viewport.Y + viewport.Height;
}

IDirect3DDevice8* directx::GetDevice()
{
    HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
    if (eqfx)
        device = *(IDirect3DDevice8**)((DWORD)eqfx + 0xa4f92c);
    else
        device = nullptr;
    return device;
}

bool directx::WorldToScreen(Vec3 worldPos, Vec2& screenPos) 
{
    GetDevice();
    if (!device)
        return false;
    // Transform the world coordinates by the combined matrix
    D3DXMATRIX matWorld, matView, matProj;
    D3DVIEWPORT8 viewport;

    device->GetTransform(D3DTS_WORLD, &matWorld);
    device->GetTransform(D3DTS_VIEW, &matView);
    device->GetTransform(D3DTS_PROJECTION, &matProj);
    device->GetViewport(&viewport);
    D3DXVECTOR3 screen;
    D3DXVECTOR3 d3dPOS = { worldPos.x,worldPos.y, worldPos.z };
    D3DXVec3Project(&screen, &d3dPOS, &viewport, &matProj, &matView, &matWorld);
    screenPos.x = screen.y;
    screenPos.y = screen.x;
    return true;
    //if (IsOffScreen(screenPos, viewport)) {
    //    return false;
    //}
    //else {
    //    return true;
    //}
}
//int RenderPartialScene(float a, int* b, int c, int d)
//{
//    if (ZealService::get_instance()->callbacks)
//        ZealService::get_instance()->callbacks->invoke_generic(callback_type::EndScene);
//    ZealService::get_instance()->hooks->hook_map["RenderPartialScene"]->original(RenderPartialScene)(a, b, c, d);
//}

directx::directx()
{
  /*  FARPROC partial_scene = GetProcAddress(GetModuleHandleA("eqgfx_dx8.dll"), "t3dRenderPartialScene");
    if (partial_scene)
        ZealService::get_instance()->hooks->Add("RenderPartialScene", (int)partial_scene, RenderPartialScene, hook_type_detour);*/
    update_device();

}