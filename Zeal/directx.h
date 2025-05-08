#pragma once
#include "vectors.h"
#include "d3dx8/d3d8.h"
#include "d3dx8/d3d8types.h"
#include "d3dx8/d3dx8math.h"
#include "ZealSettings.h"
#include <vector>


class DirectX
{
public:
	bool WorldToScreen(Vec3 worldPos, Vec2& screenPos);
	Vec2 GetScreenRect();
	IDirect3DDevice8* GetDevice();
    ZealSetting<int> fps_limit = { 0, "Zeal", "FPS", false };
	DirectX();
private:
	IDirect3DDevice8* device;
	void update_device();
};

// Convenience class for stashing the active D3D render state before modifying
// a list of parameters.
class D3DRenderStateStash {
public:
    struct Pair {
        D3DRENDERSTATETYPE state_type;
        DWORD value;
    };

    D3DRenderStateStash(IDirect3DDevice8& device) : device(device) {
    }

    // Stashes the current state and modifies the value.
    void store_and_modify(Pair pair) {
        DWORD value;
        device.GetRenderState(pair.state_type, &value);
        stored_pairs.push_back({ pair.state_type, value });
        device.SetRenderState(pair.state_type, pair.value);
    }

    // Copies the stashed state back.
    void restore_state() {
        for (const auto& pair : stored_pairs)
            device.SetRenderState(pair.state_type, pair.value);
        stored_pairs.clear();
    }

private:
    IDirect3DDevice8& device;
    std::vector<Pair> stored_pairs;
};


// Convenience class for stashing the active D3D texture state before modifying
// a list of parameters.
class D3DTextureStateStash {
public:
    struct Pair {
        D3DTEXTURESTAGESTATETYPE state_type;
        DWORD value;
    };

    D3DTextureStateStash(IDirect3DDevice8& device) : device(device) {
    }

    // Stashes the current state and modifies the value.
    void store_and_modify(Pair pair) {
        DWORD value;
        device.GetTextureStageState(0, pair.state_type, &value);
        stored_pairs.push_back({ pair.state_type, value });
        device.SetTextureStageState(0, pair.state_type, pair.value);
    }

    // Copies the stashed state back.
    void restore_state() {
        for (const auto& pair : stored_pairs)
            device.SetTextureStageState(0, pair.state_type, pair.value);
        stored_pairs.clear();
    }

private:
    IDirect3DDevice8& device;
    std::vector<Pair> stored_pairs;
};
