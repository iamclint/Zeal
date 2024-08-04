#include "nameplate.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

struct CachedNameData
{
	int unk1;
	int unk2;
	int name_len;
	int unk3;
	int fontptr;
	char name[30];
};

int DeferCachedNameTagTextW(int font_size, CachedNameData* cnd, int a1, int a2, int a3, int a4, double a5, int a6)
{
	//if (GetAsyncKeyState(VK_END))
	//{
	//	Zeal::EqGame::print_chat("font: [%i] name: [%s] a1: [%i] a2: [%i] a3: [%i] a4: [%i] a5: [%f] a6: [%i]", font_size, cnd->name, a1, a2, a3, a4, a5, a6);
	//}
	//if (Zeal::EqGame::get_self())
	//{
	//	std::string name = cnd->name;
	//	name = Zeal::String::split(name, " ").front();
	//	std::string me = Zeal::EqGame::get_self()->Name;
	//	me = Zeal::String::trim_and_reduce_spaces(me);
	//	if (name == me)
	//	{
	//		cnd->unk1 = 0;
	//	}
	//	return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
	//}
	//else
	//{
		return ZealService::get_instance()->hooks->hook_map["DeferCachedNameTagTextW"]->original(DeferCachedNameTagTextW)(font_size, cnd, a1, a2, a3, a4, a5, a6);
	//}
}

NamePlate::NamePlate(ZealService* zeal, IO_ini* ini)
{
	HMODULE eqfx = GetModuleHandleA("eqgfx_dx8.dll");
	if (eqfx)
		zeal->hooks->Add("DeferCachedNameTagTextW", (DWORD)eqfx + 0x70A00, DeferCachedNameTagTextW, hook_type_detour);
}

NamePlate::~NamePlate()
{
}
