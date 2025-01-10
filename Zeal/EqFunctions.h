#pragma once
#include "memory.h"
#include "EqStructures.h"
#include "EqUI.h"
#include <cstdarg>

#define CON_WHITE ZealService::get_instance()->ui->options->GetColor(15) //D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0xf0)
#define CON_RED ZealService::get_instance()->ui->options->GetColor(17) //D3DCOLOR_ARGB(0xff, 0xf0, 0x0, 0x0)
#define CON_BLUE ZealService::get_instance()->ui->options->GetColor(14)//D3DCOLOR_ARGB(0xff, 0x0, 0x40, 0xf0) Slightly lighter by default
#define CON_YELLOW ZealService::get_instance()->ui->options->GetColor(16) //D3DCOLOR_ARGB(0xff, 0xf0, 0xf0, 0x0)
#define CON_LIGHTBLUE ZealService::get_instance()->ui->options->GetColor(13) //D3DCOLOR_ARGB(0xff, 0x0, 0xf0, 0xf0)
#define CON_GREEN ZealService::get_instance()->ui->options->GetColor(12) //D3DCOLOR_ARGB(0xff, 0x0, 0xf0, 0x0)

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
			//static int fn_main_loop = 0x5473c3;
			/*inline int fn_loadoptions = 0x536CE0;*/
			static int fn_KeyboardPageHandleKeyboardMsg = 0x42c4fb;

			static mem::function<void __fastcall(int t, int unused, const char* data, short color, bool un)> print_chat = 0x537f99;
			//static mem::function<void __stdcall(const char* data)> log = 0x5240dc;
			static mem::function<char __fastcall(int display, int unused, Zeal::EqStructures::Entity*, Zeal::EqStructures::Entity*)> can_target = 0x4afa90;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_world_visible_actor_list = 0x7f9850;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_camera_location = 0x7f99d4;
			static mem::function<char __fastcall(int, int, float, float, float, float, float, float, float*, float*, float* , char) > s3dCollideSphereWithWorld = 0x4b3c45;
			static mem::function<short __fastcall(int, int)> get_max_mana = 0x4B9483;
			static mem::function<short __fastcall(int, int)> get_cur_mana = 0x4b9450;
			static mem::function<int __cdecl(int, Vec3*)> t3dGetRegionNumberFromWorldAndXYZ = 0x0;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> ui_something = 0x536bae;
			static mem::function<float __stdcall(float input_heading)> fix_heading = 0x4a2eed;
			static mem::function<void __stdcall()> ProcessMouseEvent = 0x525db4;
			static mem::function<void __fastcall(int, int, float)> MouseLook = 0x4db384;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> proc_mouse = 0x537707;
			static mem::function<void __fastcall(DWORD, int unused, int cmd , int str_id, int category)> InitKeyBind = 0x42B21D; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<void __fastcall(DWORD, int unused, char* str)> InitKeyBindStr = 0x576190; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<int __cdecl(Zeal::EqUI::CXSTR*, const char* format)> CXStr_PrintString = 0x578110;
			static mem::function<int __stdcall()> LoadOptions = 0x536CE0;
			static mem::function<int __fastcall(int t, int unk, int key, int type)> readKeyMapFromIni = 0x525520;
			static mem::function<void __cdecl(Zeal::EqStructures::EQCHARINFO* _char, Zeal::EqStructures::_EQITEMINFO** Item, int)> auto_inventory = 0x4F0EEB;
			static mem::function<int __cdecl()> UI_ChatInputCheck = 0x54042d;
			static mem::function<bool __cdecl()> IsNoSlashWndActive = 0x00545bbd;  // Returns false if merchant, loot, trade, and bank windows are not activated.
			static mem::function<bool __cdecl()> IsPlayerABardAndSingingASong = 0x0040a74a;  // Does internal pointer checking.
			static mem::function<int __cdecl(Zeal::EqStructures::Entity*, const char*)> do_say = 0x4f8172;
			static mem::function<float __fastcall(int, int)> encum_factor = 0x4bb9c7;
			static mem::function<float __fastcall(int, int, int, int)> OpenContainer = 0x4168bd;
			static mem::function<float __fastcall(int, int)> CloseAllContainers = 0x416a43;
			static mem::function<int __fastcall(int, int)> GetFocusWnd = 0x5a07c0;
			static mem::function<int __fastcall(int, int, int)> CXWndIsType = 0x571300;
			static mem::function<int __fastcall(int, int, int, int)> CXWndShowContextMenu = 0x5A02F0;
			static mem::function<char __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, int left, int top, int right, int bottom)> CXWndMoveAndInvalidate = 0x00573600;
			static mem::function<Zeal::EqUI::CXRect* __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect* rect)> CXWndGetMinimizedRect = 0x00573730;
			static mem::function<int __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, int32_t mouse_x, int32_t mouse_y, uint32_t unknown3)> CXWndHandleRButtonDown = 0x005703f0;
			static mem::function<void __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CXWndDeactivate = 0x0056e0b0;  // Does nothing.
			static mem::function<char __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect rect)> CXWndOnMove = 0x00402301;
			static mem::function<int __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, int width, int height)> CXWndOnResize = 0x0056eb00;
			static mem::function<void __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CXWndOnMinimizeBox = 0x00402306;
			static mem::function<int __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CXWndOnActivate = 0x00402317;  // Just returns zero. Shared call.
			static mem::function<int __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CXWndAboutToDeleteWnd = 0x00402317;  // Just returns zero. Shared call.
			static mem::function<Zeal::EqUI::CXRect* __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect* rect, int region)> CXWndGetHitTestRect = 0x00571540;
			static mem::function<Zeal::EqUI::CXRect* __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect* rect)> CXWndGetInnerRect = 0x005714b0;
			static mem::function<Zeal::EqUI::CXRect* __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX, Zeal::EqUI::CXRect* rect)> CXWndGetClientRect = 0x00572390;
			static mem::function<int __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CSidlScreenWndOnShow = 0x0056f590;
			static mem::function<void __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CSidlScreenWndLoadIniInfo = 0x0056f5a0;
			static mem::function<void __fastcall(Zeal::EqUI::EQWND* wnd, int unusedEDX)> CSidlScreenWndStoreIniInfo = 0x0056fe10;
			static mem::function<int* __fastcall(Zeal::EqUI::EQWND*, int unusedEDX, Zeal::EqUI::BasicWnd*, Zeal::EqUI::CXSTR name)> CSidlScreenWndConstructor = 0x0056e2b0;
			static mem::function<int* __fastcall(Zeal::EqUI::EQWND*, int unusedEDX, bool delete_me)> CSidlScreenWndDestructor = 0x0056e0c0;
			static mem::function<Zeal::EqUI::ItemDisplayWnd* __fastcall(const Zeal::EqUI::ItemDisplayWnd*, int unusedEDX, int unk)> CItemDisplayWndConstructor = 0x00423331;
			static mem::function<int* __fastcall(Zeal::EqUI::ItemDisplayWnd*, int unusedEDX, bool delete_me)> CItemDisplayWndDestructor = 0x004234e8;
			static mem::function<int __fastcall(int, int)> CLootWndDeactivate = 0x42651f;
			static mem::function<int __cdecl()> MessageEvent = 0x52437F;
			static mem::function<int __fastcall(int, int)> ProcessControls = 0x53F337;
			static mem::function<int __cdecl(Zeal::EqStructures::Entity*, const char*)> ReplyTarget = 0x4ff62d;
			static mem::function<Zeal::EqStructures::Entity* __cdecl(const char* name)> GetPlayerFromName = 0x005081db;  // Entity list traversal with name stricmp.
		}
		namespace Spells //some wrappers for simplifying
		{
			void OpenBook();
			void StopSpellBookAction();
			void Memorize(int book_index, int gem_index);
			void Forget(int index);
			void UpdateGems(int index);
		}
		namespace OldUI
		{
			bool spellbook_window_open();
		}
		int GetSpellCastingTime();  // Used by CCastingWnd. Returns -1 if done otherwise in units of 0.1% of time left.
		DWORD GetLevelCon(Zeal::EqStructures::Entity* ent);
		const char* get_aa_title_name(BYTE class_id, int aa_rank, BYTE gender_id);
		float CalcCombatRange(Zeal::EqStructures::Entity* entity1, Zeal::EqStructures::Entity* entity2);
		float CalcZOffset(Zeal::EqStructures::Entity* ent);
		float CalcBoundingRadius(Zeal::EqStructures::Entity* ent);
		void DoPercentConvert(std::string& str);
		void move_item(int a1, int slot, int a2, int a3);
		bool can_inventory_item(Zeal::EqStructures::EQITEMINFO* item);
		// Checks if the race/class/deity etc can equip this item
		bool can_use_item(Zeal::EqStructures::EQCHARINFO* c, Zeal::EqStructures::EQITEMINFO* item);
		// Checks if the item can currently be equipped in this inventory slot.
		// This also prevents equipping an offhand while holding a 2Hander. Or equipping a mainhand with an instrument, etc.
		// Use [1..21] for slot number.
		bool can_item_equip_in_slot(Zeal::EqStructures::EQCHARINFO* c, Zeal::EqStructures::EQITEMINFO* item, int slot);
		bool can_equip_item(Zeal::EqStructures::EQITEMINFO* item);
		void print_debug(const char* format, ...);
		UINT get_eq_time();
		int get_eq_main();
		void do_autoattack(bool enabled);
		bool CanIHitTarget(float dist);
		bool do_attack(uint8_t type, uint8_t p2);
		void do_inspect(Zeal::EqStructures::Entity* player);
		void execute_cmd(UINT cmd, bool isdown, int unk2);
		EqStructures::Everquest* get_eq();
		int get_gamestate();
		void SetMusicSelection(int number, bool enabled);
		bool is_new_ui();
		HWND get_game_window();
		bool show_context_menu();
		bool game_wants_input(); //returns true if the game wants text input so it doesn't run binds
		void CXStr_PrintString(Zeal::EqUI::CXSTR* str, const char* format, ...);
		Vec3 get_player_head_pos();
		Vec3 get_view_actor_head_pos();
		void pet_command(int cmd, short spawn_id);
		float encum_factor();
		Zeal::EqStructures::Entity* get_view_actor_entity();
		inline Zeal::EqStructures::GuildName* guild_names = (Zeal::EqStructures::GuildName*)0x7F9C94;
		bool collide_with_world(Vec3 start, Vec3 end, Vec3& result, char collision_type = 0x3, bool debug = false);
		void get_camera_location();
		std::vector<Zeal::EqStructures::Entity*> get_world_visible_actor_list(float max_dist, bool only_targetable = true);
		Zeal::EqStructures::ActorLocation get_actor_location(int actor);
		bool can_target(Zeal::EqStructures::Entity* ent);
		bool is_view_actor_me();
		void print_chat_hook(const char* format, ...);
		void print_chat(std::string data);
		void print_chat(const char* format, ...);
		void print_chat(short color, const char* format, ...);
		void print_chat_wnd(Zeal::EqUI::ChatWnd*, short color, const char* format, ...);
		long get_user_color(int index);
		void set_target(Zeal::EqStructures::Entity* target); //this will target an entity without question
		void do_target(const char* name); //this function is used by /target and /rt with range limitations
		bool can_move();
		bool is_on_ground(Zeal::EqStructures::Entity* ent);
		void log(std::string& data);
		Zeal::EqStructures::EQCHARINFO* get_char_info();
		Zeal::EqStructures::Entity* get_active_corpse();
		Zeal::EqStructures::Entity* get_target();
		Zeal::EqStructures::Entity* get_entity_list();
		Zeal::EqStructures::Entity* get_self();
		Zeal::EqStructures::Entity* get_pet();
		Zeal::EqStructures::SPELLMGR* get_spell_mgr();
		Zeal::EqStructures::Entity* get_controlled();
		Zeal::EqStructures::CameraInfo* get_camera();
		Zeal::EqStructures::Entity* get_entity_by_id(short id);  // Returns nullptr if invalid id.
		Zeal::EqStructures::Entity* get_entity_by_parent_id(short parent_id);
		void send_message(UINT opcode, int* buffer, UINT size, int unknown);
		// trim_name() and strip_name() both generate temporary modified string copies in global buffers that will get
		// re-used, so the result should be consumed immediately (copy to std::string or compare immediately).
		const char* trim_name(const char* name);  // Cleans but leaves suffixes. Use for name's corpse123 => name's corpse.
		const char* strip_name(const char* name);  // Strips numbers and any text past an apostrophe ('s corpse).
		char* get_guildName_from_guildId(int guildId);
		char* get_string(UINT id);
		//void set_camera_position(Vec3* pos);
		int* get_display();
		float heading_to_yaw(float heading);
		bool is_mouse_hovering_window();
		std::string class_name_short(int class_id);
		std::string class_name(int class_id);
		bool is_game_ui_window_hovered();
		bool is_targetable(Zeal::EqStructures::Entity* ent);
		bool is_in_game();
		void do_say(bool hide_local, const char* format, ...);
		void do_say(bool hide_local, std::string data);
		int get_region_from_pos(Vec3* pos);
		EqUI::CXWndManager* get_wnd_manager();
		std::vector<Zeal::EqStructures::RaidMember*> get_raid_list();
		DWORD get_raid_group_number();
		int get_raid_group_count(DWORD group_number);
		std::string get_raid_group_leader(DWORD group_number);
		void print_group_leader();
		void print_raid_leaders(bool show_all_groups = false, bool show_open_groups = false);
		void print_raid_ungrouped();
		void dump_raid_state();
		std::string generateTimestamp();
		bool use_item(int item_index);
		void sort_list_wnd(Zeal::EqUI::ListWnd* list_wnd, int sort_column);
	}
}
