#include "character_select.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include <random>



DWORD GetRandomZone()
{
	// Seed with a random device for better randomness
	static std::random_device rd;
	static std::mt19937 generator(rd());  // Mersenne Twister engine
	static std::uniform_int_distribution<DWORD> distribution(1, 77);

	return distribution(generator);
}

void __fastcall StartWorldDisplay(DWORD t, DWORD unused, DWORD zone_index, DWORD uhh)
{
	if (zone_index == 0xB9 && ZealService::get_instance()->charselect->ZoneIndex.get()!=-1) //loading (character select)
		zone_index = ZealService::get_instance()->charselect->ZoneIndex.get();
	
	if (Zeal::EqGame::get_zone_name_from_index(zone_index).length() == 0) //validate the zone id by checking the name
		zone_index = 0xB9;

	ZealService::get_instance()->hooks->hook_map["StartWorldDisplay"]->original(StartWorldDisplay)(t, unused, zone_index, uhh);
}
void MovePlayerLocalSafeCoords()
{
	if (Zeal::EqGame::get_gamestate() != GAMESTATE_CHARSELECT)
		ZealService::get_instance()->hooks->hook_map["MovePlayerLocalSafeCoords"]->original(MovePlayerLocalSafeCoords)();
}
void __fastcall SelectCharacter(DWORD t, DWORD unused, DWORD unk1, DWORD unk2)
{
	ZealService::get_instance()->hooks->hook_map["SelectCharacter"]->original(SelectCharacter)(t, unused, unk1, unk2);
	Vec3* SafeCoords = (Vec3*)(0x79896C);
	*SafeCoords = ZealService::get_instance()->charselect->ZoneData[ZealService::get_instance()->charselect->ZoneIndex.get()].Position;
	ZealService::get_instance()->hooks->hook_map["MovePlayerLocalSafeCoords"]->original(MovePlayerLocalSafeCoords)();
	*Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::CharacterSelect;
	Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
	if (self)
	{
		self->Heading = ZealService::get_instance()->charselect->ZoneData[ZealService::get_instance()->charselect->ZoneIndex.get()].Heading;
		self->Position = ZealService::get_instance()->charselect->ZoneData[ZealService::get_instance()->charselect->ZoneIndex.get()].Position;
	}

}

void CharacterSelect::load_bmp_font() {
	if (bmp_font)
		return;

	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (device == nullptr)
		return;

	std::string font_filename = "arial_12";

	bmp_font = BitmapFont::create_bitmap_font(*device, font_filename);
	if (!bmp_font)
		return;  // Let caller deal with the failure.
	bmp_font->set_drop_shadow(true);
	bmp_font->set_shadow_offset_factor(0.03f);
}

void CharacterSelect::render()
{
	if (Zeal::EqGame::get_gamestate() != GAMESTATE_CHARSELECT)
		return;
	if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->CharacterSelect && Zeal::EqGame::Windows->CharacterSelect->Explore)
	{
		std::stringstream Location;
		Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
		if (self)
		{
			if (!bmp_font)
				load_bmp_font();
			if (!bmp_font)
				return;
			Location << std::fixed << std::setprecision(0) << std::dec << "[" << self->Position.x << ", " << self->Position.y << ", " << self->Position.z << "] Heading: " << self->Heading << std::endl;
			bmp_font->queue_string(Location.str().c_str(), { 10, 50, 0 }, false, D3DCOLOR_XRGB(0x00, 0xff, 0x00));
			bmp_font->flush_queue_to_screen();
		}
	}
}

CharacterSelect::CharacterSelect(ZealService* zeal)
{
	//Credits for location cleanup to thetimberowl
	ZoneData =
	{
		{1, {{15.f, 59.f, 3.7f}, 126 }}, //qeynos
		{2, {{464.f, 119.f, 3.7f}, 0 }}, //qeynos2
		{3, {{52.f, 22.f, 3.7f}, 184 }}, //qrg
		{4, {{1022.f, -678.f, 18.f}, 152 }}, //qeytoqrg
		{5, {{-28.f, 31.f, 27.8f}, 146 }}, //highpass
		{6, {{-13.f, 21.f, -2.3f}, 130 }}, //highkeep
		{8, {{-25.f, -124.f, 3.7f}, 132 }}, //freportn
		{9, {{39.f, 189.f, -24.3f}, 148 }}, //freportw
		{10, {{-419.f, -683.f, -52.3f}, 258 }}, //freporte
		{11, {{-4.f, 3.f, 3.7f}, 160 }}, //runnyeye
		{12, {{-709.f, -4152.f, 36.5f}, 76 }}, //qey2hh1
		{13, {{-3336.f, 1220.f, 17.1f}, 502 }}, //northkarana
		{14, {{-6347.f, 1092.f, 0.1f}, 508 }}, //southkarana
		{15, {{12.f, 544.f, 0.7f}, 386 }}, //eastkarana
		{16, {{635.f, -484.f, -96.6f}, 62 }}, //beholder
		{17, {{-78.f, 7.f, 4.3f}, 460 }}, //blackburrow
		{18, {{212.f, 4.f, 3.7f}, 346 }}, //paw
		{19, {{-52.f, -110.f, 3.7f}, 142 }}, //rivervale
		{20, {{730.f, 2290.f, 269.1f}, 286 }}, //kithicor
		{21, {{8.f, 149.f, -51.5f}, 144 }}, //commons
		{22, {{-1045.f, 848.f, -1.4f}, 2 }}, //ecommons
		{23, {{641.f, 702.f, 3.7f}, 252 }}, //erudnint
		{24, {{-957.f, -237.f, 52.7f}, 508 }}, //erudnext
		{25, {{2336.f, -922.f, -4.8f}, 124 }}, //nektulos
		{26, {{75.f, 101.f, -6.3f}, 258 }}, //cshome
		{27, {{1123.f, 709.f, 181.7f}, 94 }}, //lavastorm
		{29, {{-20.f, -9.f, 3.7f}, 260 }}, //halas
		{30, {{1878.f, -6288.f, -60.2f}, 132 }}, //everfrost
		{31, {{-712.f, -678.f, 51.7f}, 124 }}, //soldunga
		{32, {{-1247.f, -826.f, 85.f}, 0 }}, //soldungb
		{33, {{-98.f, -155.f, 4.4f}, 386 }}, //misty
		{34, {{-707.f, -516.f, 8.2f}, 502 }}, //nro
		{35, {{-916.f, 44.f, 12.1f}, 452 }}, //sro
		{36, {{-136.f, -46.f, 3.7f}, 0 }}, //befallen
		{37, {{-439.f, -233.f, -0.3f}, 378 }}, //oasis
		{38, {{2279.f, 209.f, -44.6f}, 248 }}, //tox
		{39, {{655.f, 246.f, -905.1f}, 226 }}, //hole
		{40, {{3.f, 103.f, 28.7f}, 122 }}, //neriaka
		{41, {{-51.f, -887.f, -38.3f}, 134 }}, //neriakb
		{42, {{767.f, -897.f, -52.3f}, 260 }}, //neriakc
		{44, {{-15.f, 192.f, 3.7f}, 132 }}, //najena
		{45, {{356.f, -399.f, -38.3f}, 124 }}, //qcat
		{46, {{1269.f, 679.f, -17.f}, 88 }}, //innothule
		{47, {{-2273.f, 2609.f, 3.7f}, 4 }}, //feerrott
		{48, {{721.f, -280.f, 31.7f}, 258 }}, //cazicthule
		{49, {{484.f, 581.f, 3.7f}, 426 }}, //oggok
		{50, {{37.f, 1062.f, 131.1f}, 356 }}, //rathemtn
		{51, {{2483.f, 1003.f, 107.1f}, 168 }}, //lakerathe
		{52, {{312.f, -461.f, 6.3f}, 254 }}, //grobb
		{54, {{-160.f, -134.f, 77.7f}, 0 }}, //gfaydark
		{55, {{164.f, 20.f, -24.3f}, 254 }}, //akanon
		{56, {{277.f, -292.f, -20.9f}, 64 }}, //steamfont
		{57, {{-869.f, 3013.f, -1.3f}, 346 }}, //lfaydark
		{58, {{-72.f, -45.f, 3.7f}, 270 }}, //crushbone
		{59, {{146.f, 0.f, -192.2f}, 18 }}, //mistmoore
		{60, {{15.f, -2.f, 3.7f}, 266 }}, //kaladima
		{61, {{0.f, 0.f, 3.7f}, 126 }}, //felwithea
		{62, {{327.f, -696.f, -10.3f}, 316 }}, //felwitheb
		{63, {{373.f, 0.f, 3.7f}, 256 }}, //unrest
		{64, {{-194.f, -192.f, 31.7f}, 248 }}, //kedge
		{65, {{1145.f, 322.f, -66.3f}, 506 }}, //guktop
		{66, {{1498.f, -749.f, -178.2f}, 252 }}, //gukbot
		{67, {{704.f, 133.f, 3.7f}, 252 }}, //kaladimb
		{68, {{2674.f, -230.f, 49.4f}, 260 }}, //butcher
		{69, {{589.f, -6605.f, 11.3f}, 288 }}, //oot
		{70, {{1447.f, -154.f, 179.6f}, 502 }}, //cauldron
		{71, {{743.f, -325.f, 127.3f}, 126 }}, //airplane
		{72, {{454.f, -341.f, 2.2f}, 380 }}, //fearplane
		{73, {{983.f, -285.f, 3.7f}, 386 }}, //permafrost
		{74, {{648.f, 240.f, 3.7f}, 226 }}, //kerraridge
		{75, {{641.f, 450.f, -24.3f}, 108 }}, //paineel
		{76, {{308.f, 189.f, 3.7f}, 340 }}, //hateplane
		{77, {{-49.f, 1118.f, 3.7f}, 128 }}, //arena
		{78, {{-80.f, 1601.f, -17.3f}, 192 }}, //fieldofbone
		{79, {{-608.f, 1244.f, -17.9f}, 14 }}, //warslikswood
		{80, {{489.f, 5.f, 17.6f}, 0 }}, //soltemple
		{81, {{-68.f, -94.f, -172.2f}, 106 }}, //droga
		{82, {{171.f, 783.f, 3.7f}, 252 }}, //cabwest
		{83, {{-1205.f, -2006.f, 25.2f}, 408 }}, //swampofnohope
		{84, {{-3768.f, 2340.f, -77.9f}, 418 }}, //firiona
		{85, {{-3060.f, -2594.f, 192.7f}, 212 }}, //lakeofillomen
		{86, {{1137.f, 9669.f, 1160.2f}, 72 }}, //dreadlands
		{87, {{5990.f, -3818.f, -248.6f}, 246 }}, //burningwood
		{88, {{-14.f, 558.f, -289.2f}, 0 }}, //kaesora
		{89, {{-2087.f, -430.f, -140.2f}, 120 }}, //sebillis
		{90, {{-182.f, -165.f, 3.7f}, 246 }}, //citymist
		{91, {{2300.f, 2981.f, -80.f}, 254 }}, //skyfire
		{92, {{-27.f, -1639.f, -126.9f}, 374 }}, //frontiermtns
		{93, {{2530.f, 36040.f, -111.1f}, 172 }}, //overthere
		{94, {{306.f, -2222.f, -337.f}, 380 }}, //emeraldjungle
		{95, {{-1677.f, -4758.f, -472.8f}, 254 }}, //trakanon
		{96, {{-6194.f, 2601.f, 4.2f}, 440 }}, //timorous
		{97, {{2.f, 16.f, 45.7f}, 124 }}, //kurn
		{98, {{-1775.f, 745.f, 3.7f}, 306 }}, //erudsxing
		{100, {{1377.f, -772.f, 1029.f}, 398 }}, //stonebrunt
		{101, {{186.f, 178.f, -77.8f}, 382 }}, //warrens
		{102, {{20.f, 152.f, 3.7f}, 126 }}, //karnor
		{103, {{119.f, 386.f, 53.7f}, 126 }}, //chardok
		{104, {{95.f, -500.f, 4.7f}, 134 }}, //dalnir
		{105, {{1.f, -44.f, 3.7f}, 384 }}, //charasis
		{106, {{948.f, -190.f, 3.9f}, 254 }}, //cabeast
		{107, {{-1547.f, -599.f, -188.2f}, 382 }}, //nurga
		{108, {{319.f, -259.f, 447.5f}, 130 }}, //veeshan
		{109, {{-145.f, 83.f, -26.3f}, 330 }}, //veksar
		{110, {{1747.f, 3978.f, 332.4f}, 144 }}, //iceclad
		{111, {{284.f, 576.f, 23.7f}, 314 }}, //frozenshadow
		{112, {{-187.f, 0.f, 5.7f}, 0 }}, //velketor
		{113, {{-1339.f, 1759.f, -156.2f}, 18 }}, //kael
		{114, {{2076.f, 1976.f, -160.4f}, 256 }}, //skyshrine
		{115, {{-852.f, 5.f, 13.7f}, 254 }}, //thurgadina
		{116, {{-7069.f, 293.f, 151.f}, 62 }}, //eastwastes
		{117, {{-1054.f, 413.f, 297.5f}, 338 }}, //cobaltscar
		{118, {{-536.f, -52.f, 77.9f}, 252 }}, //greatdivide
		{119, {{712.f, -1025.f, -149.5f}, 506 }}, //wakening
		{120, {{577.f, 705.f, -196.4f}, 256 }}, //westwastes
		{121, {{-346.f, 756.f, -536.f}, 500 }}, //crystal
		{123, {{345.f, 1388.f, 3.7f}, 186 }}, //necropolis
		{124, {{-49.f, -779.f, 77.7f}, 254 }}, //templeveeshan
		{125, {{-636.f, -600.f, -76.3f}, 130 }}, //sirens
		{126, {{101.f, -393.f, 121.6f}, 254 }}, //mischiefplane
		{127, {{1904.f, -1607.f, 74.2f}, 4 }}, //growthplane
		{128, {{-2757.f, -1483.f, -992.3f}, 254 }}, //sleeper
		{129, {{267.f, 5.f, 2.7f}, 254 }}, //thurgadinb
		{150, {{-635.f, 186.f, -28.3f}, 252 }}, //shadowhaven
		{151, {{-730.f, -141.f, 3.7f}, 246 }}, //bazarr
		{152, {{-46.f, 0.f, -30.8f}, 2 }}, //nexus
		{153, {{1186.f, -539.f, -49.6f}, 258 }}, //echo
		{154, {{-651.f, -41.f, -14.5f}, 0 }}, //acrylia
		{155, {{2.f, 283.f, -152.3f}, 382 }}, //sharvahl
		{156, {{-1592.f, 990.f, -354.2f}, 410 }}, //paludal
		{157, {{-1543.f, -887.f, -282.1f}, 246 }}, //fungusgrove
		{158, {{1.f, 1682.f, 3.7f}, 382 }}, //vexthal
		{159, {{-1737.f, -233.f, 9.7f}, 254 }}, //sseru
		{160, {{-2.f, -371.f, 2.7f}, 390 }}, //katta
		{161, {{464.f, 683.f, 72.7f}, 130 }}, //netherbian
		{162, {{-325.f, 867.f, 403.2f}, 382 }}, //ssratemple
		{163, {{530.f, 2340.f, 161.7f}, 510 }}, //griegsend
		{164, {{-28.f, 1107.f, -62.2f}, 114 }}, //thedeep
		{165, {{-2133.f, -3211.f, -127 / 9.f}, 128 }}, //shadeweaver
		{166, {{903.f, 2026.f, 33.1f}, 220 }}, //hollowshade
		{167, {{-902.f, -145.f, 77.9f}, 298 }}, //grimling
		{168, {{-1361.f, -967.f, -20.4f}, 12 }}, //mseru
		{169, {{258.f, 580.f, 203.7f}, 26 }}, //letalis
		{170, {{3312.f, 972.f, 30.f}, 202 }}, //twilight
		{171, {{186.f, -277.f, -18.8f}, 412 }}, //thegrey
		{172, {{-12.f, 1585.f, -41.4f}, 348 }}, //tenebrous
		{173, {{248.f, -941.f, -118.f}, 460 }}, //maiden
		{174, {{-1431.f, -758.f, 132.6f}, 32 }}, //dawnshroud
		{175, {{963.f, -1138.f, 155.3f}, 486 }}, //scarlet
		{176, {{-507.f, -42.f, 0.2f}, 284 }}, //umbral
		{179, {{453.f, -931.f, 19.7f}, 510 }}, //akheva
		{181, {{837.f, 1967.f, -13.f}, 258 }}, //jaggedpine
		{184, {{5.f, -316.f, 8.7f}, 20 }}, //load
		{185, {{-4.f, -260.f, -724.3f}, 0 }}, //load2
		{200, {{252.f, 316.f, -69.2f}, 26 }}, //codecay
		{201, {{1218.f, 74.f, 10.2f}, 94 }}, //pojustice
		{202, {{335.f, 394.f, -125.3f}, 314 }}, //poknowledge
		{203, {{704.f, -1400.f, -876.2f}, 124 }}, //potranquility
		{204, {{-983.f, -1354.f, 121.f}, 452 }}, //ponightmare
		{205, {{447.f, 526.f, 59.8f}, 424 }}, //podisease
		{206, {{-5.f, 1125.f, 6.1f}, 508 }}, //poinnovation
		{207, {{-59.f, 5.f, 579.7f}, 258 }}, //potorment
		{208, {{118.f, 194.f, 151.5f}, 180 }}, //povalor
		{209, {{-1742.f, -868.f, 2251.7f}, 126 }}, //bothunder
		{210, {{1195.f, -1601.f, -451.f}, 118 }}, //postorms
		{211, {{2068.f, -1726.f, -113.4f}, 510 }}, //hohonora
		{212, {{-1163.f, 0.f, 243.7f}, 0 }}, //solrotower
		{213, {{-892.f, -179.f, -2.7f}, 0 }}, //powar
		{214, {{1979.f, -684.f, 195.7f}, 386 }}, //potactics
		{215, {{24.f, -25.f, 1439.7f}, 382 }}, //poair
		{216, {{-1176.f, 0.f, 5.4f}, 264 }}, //powater
		{217, {{-1057.f, -1504.f, -197.5f}, 252 }}, //pofire
		{218, {{-20.f, -882.f, 37.5f}, 458 }}, //poeartha
		{219, {{672.f, 131.f, 31.9f}, 16 }}, //potimea
		{220, {{2.f, 2545.f, 471.7f}, 384 }}, //hohonorb
		{221, {{-16.f, -1991.f, 133.7f}, 380 }}, //nightmareb
		{222, {{256.f, 2050.f, -218.3f}, 252 }}, //poearthb
		{223, {{3.f, 124.f, 3.7f}, 130 }}, //potimeb
	};
	mem::set(0x40f83d, 0x90, 5);
	mem::set(0x40c20a, 0x90, 5);
	zeal->callbacks->AddGeneric([this]() { bmp_font.reset(); }, callback_type::InitUI);  // Just release all resources.
	zeal->callbacks->AddGeneric([this]() { bmp_font.reset(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { bmp_font.reset(); }, callback_type::DXReset);  // Just release all resources.
	zeal->callbacks->AddGeneric([this]() { render(); }, callback_type::RenderUI);
	zeal->callbacks->AddGeneric([this]() {
		if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->CharacterSelect && !Zeal::EqGame::Windows->CharacterSelect->Explore)
			*Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::CharacterSelect; //this fixes the camera when camping
	}, callback_type::CharacterSelectLoop);
	zeal->hooks->Add("StartWorldDisplay", 0x4A849E, StartWorldDisplay, hook_type_detour);
	zeal->hooks->Add("SelectCharacter", 0x40F56D, SelectCharacter, hook_type_detour);
	zeal->hooks->Add("MovePlayerLocalSafeCoords", 0x4B459C, MovePlayerLocalSafeCoords, hook_type_detour);
	
	mem::set(0x55B4A1, 0x90, 2); //ignore connection state for mouse wheel
}
CharacterSelect::~CharacterSelect()
{

}