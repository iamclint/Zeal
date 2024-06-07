#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "directx.h"
#define NUM_VERTICES 4

struct vertexdata
{
//	float unk1;
//	float unk2;
	float vertexData[NUM_VERTICES * 3];
	float invW1, invW2, invW3, invW4;
	float zero1, zero2, zero3;
	DWORD color;
	vertexdata(Zeal::EqStructures::Entity* target)
	{

		// Define the vertices of the quad (target ring)
		float halfSize = 100 / 2.0f;
		vertexData[0] = target->Position.x - halfSize;
		vertexData[1] = target->Position.y - halfSize;
		vertexData[2] = target->Position.z;

		vertexData[3] = target->Position.x + halfSize;
		vertexData[4] = target->Position.y - halfSize;
		vertexData[5] = target->Position.z;

		vertexData[6] = target->Position.x - halfSize;
		vertexData[7] = target->Position.y + halfSize;
		vertexData[8] = target->Position.z;

		vertexData[9] = target->Position.x + halfSize;
		vertexData[10] = target->Position.y + halfSize;
		vertexData[11] = target->Position.z;
		invW1 = 1.0f / 1.0f; // Placeholder values for inverse W coordinates
		invW2 = 1.0f / 1.0f;
		invW3 = 1.0f / 1.0f;
		invW4 = 1.0f / 1.0f;
		zero1 = 0.0f;
		zero2 = 0.0f;
		zero3 = 0.0f;
		color = 0xff00FFFF; // ARGB format for black color
	}
};

void TargetRing::callback_render()
{
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	IDirect3DDevice8* d3ddev = ZealService::get_instance()->dx->device;
	if (!Zeal::EqGame::is_in_game() || !enabled || !target || !d3ddev)
		return;
	UINT col = 0xFF00FFFF;

	vertexdata vd(target);
	FARPROC eqfx = GetProcAddress(GetModuleHandleA("eqgfx_dx8.dll"), "t3dDeferQuad");
	if (eqfx != NULL)
	{
		reinterpret_cast<int(__cdecl*)(int, UINT)>((int)eqfx)((int)&vd, col);
	}
}

void TargetRing::set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "TargetRing", _enabled);
	enabled = _enabled;
}

//don't get too excited this isn't functioning
TargetRing::TargetRing(ZealService* zeal, IO_ini* ini)
{
	//mem::write<BYTE>(0x4A594B, 0x14);
	/*if (!ini->exists("Zeal", "TargetRing"))
		ini->setValue<bool>("Zeal", "TargetRing", true);
	enabled = ini->getValue<bool>("Zeal", "TargetRing");
	zeal->callbacks->add_generic([this]() { callback_render(); }, callback_type::RenderUI);
	zeal->commands_hook->add("/targetring", {}, "Toggles target ring",
		[this](std::vector<std::string>& args) {
			set_enabled(!enabled);
			Zeal::EqGame::print_chat("Target ring is %s", enabled ? "Enabled" : "Disabled");
			return true;
		});*/
}

TargetRing::~TargetRing()
{
}