#include "directx.h"
#include "Zeal.h"
#pragma comment(lib, "d3dx8/d3d8.lib")
#pragma comment(lib, "d3dx8/d3dx8.lib")

void directx::update_device()
{
    HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
    if (eqfx)
        device = *(IDirect3DDevice8**)((DWORD)eqfx + 0xa4f92c);
    else
        device = nullptr;
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

bool directx::WorldToScreen(Vec3 worldPos, Vec2& screenPos) 
{
    update_device();
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

directx::directx()
{
    update_device();
}