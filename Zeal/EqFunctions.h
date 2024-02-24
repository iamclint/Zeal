#pragma once
#include "memory.h"
#include "EqStructures.h"
#include <cstdarg>

enum Stance
{
	Stand = 0x64,
	Bind = 0x69,
	Sit = 0x6E,
	Duck = 0x6F,
	Feign = 0x73,
	Dead = 0x78
};

namespace Zeal
{
	namespace EqGame
	{
		namespace EqGameInternal
		{
			static int fn_GetLabelFromEQ = 0x436680;
			static int fn_GetGaugeLabelFromEQ = 0x43600d;
			static int fn_finalizeloot = 0x47f2bd;
			static int fn_releaseloot = 0x426576;
			static int fn_targetnearestnpc = 0x4ac17f;
			static int* fn_s_t3dGetActorLocation_ptr = (int*)0x7f99c8;
			static int* fn_s_s3dGetWorldVisibleActorList = (int*)0x7f9850;
			static int* fn_s_s3dGetCameraLocation = (int*)0x7f99D4;
			static int* fn_s_t3dSetCameraLocation = (int*)0x7f9ae4;
			static int fn_handle_mouseweheel = 0x55B2E0;
			static int fn_handle_mouselook = 0x4DB384;
			static int fn_initkeyboardassignments = 0x42A9D7;
			static int fn_executecmd = 0x54050c;
			static int fn_interpretcmd = 0x54572f;
			static int fn_main_loop = 0x5473c3;
			/*inline int fn_loadoptions = 0x536CE0;*/
			static int fn_KeyboardPageHandleKeyboardMsg = 0x42c4fb;


			static mem::function<void __fastcall(int t, int unused, const char* data, short color, bool un)> print_chat = 0x537f99;
			static mem::function<char __fastcall(int display, int unused, Zeal::EqStructures::Entity*, Zeal::EqStructures::Entity*)> can_target = 0x4afa90;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_world_visible_actor_list = 0x7f9850;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_camera_location = 0x7f99d4;
			static mem::function<char __fastcall(int, int, float, float, float, float, float, float, float*, float*, float* , char) > s3dCollideSphereWithWorld = 0x4b3c45;
			static mem::function<short __fastcall(int, int)> get_max_mana = 0x4B9483;
			static mem::function<short __fastcall(int, int)> get_cur_mana = 0x4b9450;
			static mem::function<int __stdcall(int, Vec3*)> t3dGetRegionNumberFromWorldAndXYZ = 0x0;
			static mem::function<short __fastcall(Zeal::EqStructures::Entity*,int unused, unsigned char)> change_stance = 0x50be3c;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> ui_something = 0x536bae;
			static mem::function<float __stdcall(float input_heading)> fix_heading = 0x4a2eed;
			static mem::function<void __stdcall()> ProcessMouseEvent = 0x525db4;
			static mem::function<void __fastcall(int, int, float)> MouseLook = 0x4db384;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> proc_mouse = 0x537707;
			static mem::function<void __fastcall(DWORD, int unused, int cmd , int str_id, int category)> InitKeyBind = 0x42B21D; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<void __fastcall(DWORD, int unused, char* str)> InitKeyBindStr = 0x576190; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<int __cdecl(Zeal::EqStructures::CXSTR*, const char* format)> CXStr_PrintString = 0x578110;
			static mem::function<int __stdcall()> LoadOptions = 0x536CE0;
			static mem::function<int __fastcall(int t, int unk, int key, int type)> readKeyMapFromIni = 0x525520;
			static mem::function<void __cdecl(int _char, int item, int)> auto_inventory = 0x4F0EEB;
			static mem::function<int __cdecl()> UI_ChatInputCheck = 0x54042d;
			static mem::function<int __cdecl(Zeal::EqStructures::Entity*, const char*)> do_say = 0x4f8172;
			//inline mem::functiona<int, Zeal::EqStructures::CXSTR*, const char*, ...> CXStr_PrintString(reinterpret_cast<int(__cdecl*)(Zeal::EqStructures::CXSTR*, const char*, ...)>(0x578110));
		}
		//// Wrapper function to call CXStr_PrintString
		//int PrintStringWrapper(Zeal::EqStructures::CXSTR* cxstr, const char* format, ...) {
		//	va_list args;
		//	va_start(args, format);
		//	int result = EqGameInternal::CXStr_PrintString(cxstr, format, args);
		//	va_end(args);
		//	return result;
		//}
		bool is_in_char_select();
		bool game_wants_input(); //returns true if the game wants text input so it doesn't run binds
		void CXStr_PrintString(Zeal::EqStructures::CXSTR* str, const char* format, ...);
		Vec3 get_player_head_pos();
		inline Zeal::EqStructures::GuildName* guild_names = (Zeal::EqStructures::GuildName*)0x7F9C94;
		bool collide_with_world(Vec3 start, Vec3 end, Vec3& result, char collision_type = 0x3, bool debug = false);
		void get_camera_location();
		std::vector<Zeal::EqStructures::Entity*> get_world_visible_actor_list(float max_dist, bool only_targetable = true);
		Zeal::EqStructures::ActorLocation get_actor_location(int actor);
		bool can_target(Zeal::EqStructures::Entity* ent);
		void print_chat(std::string data);
		void print_chat(const char* format, ...);
		void print_chat_zeal(const char* data, short color, bool un);
		void set_target(Zeal::EqStructures::Entity* target);
		Zeal::EqStructures::Entity* get_active_corpse();
		Zeal::EqStructures::Entity* get_target();
		Zeal::EqStructures::Entity* get_entity_list();
		Zeal::EqStructures::Entity* get_self();
		Zeal::EqStructures::CameraInfo* get_camera();
		Zeal::EqStructures::Entity* get_entity_by_id(short id);
		Zeal::EqStructures::Entity* get_entity_by_parent_id(short parent_id);
		//void set_camera_position(Vec3* pos);
		int* get_display();
		float heading_to_yaw(float heading);
		bool is_mouse_hovering_window();
		std::string class_name_short(int class_id);
		std::string class_name(int class_id);
		bool is_targetable(Zeal::EqStructures::Entity* ent);
		bool is_in_game();
		void change_stance(Stance new_stance);
		void do_say(bool hide_local, const char* format, ...);
		void do_say(bool hide_local, std::string data);
		int get_region_from_pos(Vec3* pos);
	}
}