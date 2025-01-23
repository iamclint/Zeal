#include <windows.h>
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"
namespace Zeal
{
	namespace EqGame
	{
		int GetSpellCastingTime()  // GetSpellCastingTime() in eqmac.
		{
			return reinterpret_cast<int(__cdecl*)(void)>(0x00435f28)();
		}

		DWORD GetLevelCon(Zeal::EqStructures::Entity* ent) {
			if (!ent || !Zeal::EqGame::get_self())
				return 0;
			int mylevel = Zeal::EqGame::get_self()->Level;
			short diff = ent->Level - mylevel;
			DWORD conlevel = 0;

			if (diff == 0)
				return CON_WHITE;
			else if (diff >= 1 && diff <= 2)
				return CON_YELLOW;
			else if (diff >= 3)
				return CON_RED;

			if (mylevel <= 7)
			{
				if (diff <= -4)
					conlevel = CON_GREEN;
				else
					conlevel = CON_BLUE;// Zeal::EqGame::get_user_color(70);
			}
			else if (mylevel <= 8)
			{
				if (diff <= -5)
					conlevel = CON_GREEN;
				else if (diff <= -4)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 12)
			{
				if (diff <= -6)
					conlevel = CON_GREEN;
				else if (diff <= -4)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 16)
			{
				if (diff <= -7)
					conlevel = CON_GREEN;
				else if (diff <= -5)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 20)
			{
				if (diff <= -8)
					conlevel = CON_GREEN;
				else if (diff <= -6)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 24)
			{
				if (diff <= -9)
					conlevel = CON_GREEN;
				else if (diff <= -7)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 28)
			{
				if (diff <= -10)
					conlevel = CON_GREEN;
				else if (diff <= -8)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 30)
			{
				if (diff <= -11)
					conlevel = CON_GREEN;
				else if (diff <= -9)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 32)
			{
				if (diff <= -12)
					conlevel = CON_GREEN;
				else if (diff <= -9)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 36)
			{
				if (diff <= -13)
					conlevel = CON_GREEN;
				else if (diff <= -10)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 40)
			{
				if (diff <= -14)
					conlevel = CON_GREEN;
				else if (diff <= -11)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 44)
			{
				if (diff <= -16)
					conlevel = CON_GREEN;
				else if (diff <= -12)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 48)
			{
				if (diff <= -17)
					conlevel = CON_GREEN;
				else if (diff <= -13)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 52)
			{
				if (diff <= -18)

					conlevel = CON_GREEN;
				else if (diff <= -14)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 54)
			{
				if (diff <= -19)

					conlevel = CON_GREEN;
				else if (diff <= -15)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 56)
			{
				if (diff <= -20)

					conlevel = CON_GREEN;
				else if (diff <= -15)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 60)
			{
				if (diff <= -21)
					conlevel = CON_GREEN;
				else if (diff <= -16)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 61)
			{
				if (diff <= -19)
					conlevel = CON_GREEN;
				else if (diff <= -14)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else if (mylevel <= 62)
			{
				if (diff <= -17)
					conlevel = CON_GREEN;
				else if (diff <= -12)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}
			else
			{
				if (diff <= -16)
					conlevel = CON_GREEN;
				else if (diff <= -11)
					conlevel = CON_LIGHTBLUE;
				else
					conlevel = CON_BLUE;
			}

			return conlevel;
		}
		bool IsPlayableRace(WORD race) {
			switch (race) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 128:
			case 130:
				return true;
			}
			return false;
		}
		void move_item(int a1, int slot, int a2, int a3)
		{
			reinterpret_cast<bool (__thiscall*)(int t, int a1, int slot, int a2, int a3)>(0x422b1c)(*(int*)0x63d6b4, a1, slot, a2, a3);
		}
		bool is_on_ground(Zeal::EqStructures::Entity* ent)
		{
			if (ent->ActorInfo)
			{
				return (ent->Position.z - ent->ModelHeightOffset + ent->MovementSpeedZ) <= (ent->ActorInfo->Z + 0.5 + 0.001);
			}
			return true;
		}
		char* get_string(UINT id)
		{
			return reinterpret_cast<char* (__thiscall*)(int t, UINT id, bool*)>(0x550EFE)(*(int*)0x7f9490, id, nullptr);
		}
		float heading_to_yaw(float heading)
		{
			float y = 512 - heading;
			y *= 0.703125f;
			if (y < 0)
				y += 360;
			return y;
		}
		std::string equipSlotToString(int slot) {
			switch (slot) {
			case 0: return "LeftEar";
			case 1: return "Head";
			case 2: return "Face";
			case 3: return "RightEar";
			case 4: return "Neck";
			case 5: return "Shoulder";
			case 6: return "Arms";
			case 7: return "Back";
			case 8: return "LeftWrist";
			case 9: return "RightWrist";
			case 10: return "Range";
			case 11: return "Hands";
			case 12: return "Primary";
			case 13: return "Secondary";
			case 14: return "LeftFinger";
			case 15: return "RightFinger";
			case 16: return "Chest";
			case 17: return "Legs";
			case 18: return "Feet";
			case 19: return "Waist";
			case 20: return "Ammo";
			default: return "Unknown";
			}
		}
		bool can_use_item(Zeal::EqStructures::EQCHARINFO* c, Zeal::EqStructures::EQITEMINFO* item) {
			using FunctionType2 = bool(__thiscall*)(Zeal::EqStructures::EQCHARINFO* char_info, Zeal::EqStructures::EQITEMINFO* iItem);
			FunctionType2 can_use_item = reinterpret_cast<FunctionType2>(0x4BB8E8);
			return can_use_item(c, item);
		}
		bool can_item_equip_in_slot(Zeal::EqStructures::EQCHARINFO* c, Zeal::EqStructures::EQITEMINFO* item, int slot) {
			using FunctionType = bool(__cdecl*)(Zeal::EqStructures::EQCHARINFO* char_info, UINT equipable_slots, UINT slot, Zeal::EqStructures::EQITEMINFO* iItem);
			FunctionType check_loc = reinterpret_cast<FunctionType>(0x4F0DB4);
			return check_loc(c, item->EquipableSlots, slot, item);
		}
		bool can_equip_item(Zeal::EqStructures::EQITEMINFO* item)
		{
			Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
			if (!item || !c)
				return false;
			if (!can_use_item(c, item))
				return false;
			for (int i = 0; i < EQ_NUM_INVENTORY_SLOTS; i++)
			{
				if (can_item_equip_in_slot(c, item, i+1) && !c->InventoryItem[i])
				{
					//print_chat("equipable? slot: %i  %s   %i", i, equipSlotToString(i).c_str(), c->InventoryItem[i]);
					return true;
				}
					
			}
			return false;
		}

		bool can_stack(Zeal::EqStructures::EQITEMINFO* base_item, Zeal::EqStructures::EQITEMINFO* added_item)
		{
			if (!base_item || !added_item)
				return false;
			if (base_item->ID == added_item->ID && base_item->Common.IsStackable)
			{
				if (base_item->Common.StackCount + added_item->Common.StackCount <= 20)
					return true;
			}
			return false;
		}

		bool can_inventory_item(Zeal::EqStructures::EQITEMINFO* item)
		{
			Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
			if (!item || !c)
				return false;

			if (can_equip_item(item))
				return true;

			for (int i = 0; i < EQ_NUM_INVENTORY_PACK_SLOTS; i++)
			{
				Zeal::EqStructures::_EQITEMINFO* inventory_item = c->InventoryPackItem[i];
				if (!inventory_item)
					return true;

				if (inventory_item && inventory_item->Type == 1 && inventory_item->Container.Capacity > 0 && inventory_item->Container.SizeCapacity>=item->Size && item->Type!=1)
				{
					if (can_stack(inventory_item, item))
						return true;
					for (int b = 0; b < inventory_item->Container.Capacity; b++)
					{
						Zeal::EqStructures::_EQITEMINFO* bag_item = inventory_item->Container.Item[b];
						if (!bag_item)
							return true;
						if (can_stack(bag_item, item))
							return true;
					}
				}
			
			}
			return false;
		}
		Zeal::EqStructures::ActorLocation get_actor_location(int actor)
		{
			DWORD addr = *(DWORD*)0x7f99c8; //game pointer to function
			Zeal::EqStructures::ActorLocation actor_loc{};
			int* r = (int *)&actor_loc;
			__asm
			{
				push r
				push actor
				call addr
				add esp, 0x8
			}

			return actor_loc;
		}
		bool show_context_menu()
		{
			int ctx = EqGameInternal::CXWndShowContextMenu(*(int*)0x809db4, 0, *(int*)0x8092e8, *(int*)0x8092ec);
			return ctx;
		}
		EqUI::CXWndManager* get_wnd_manager()
		{
			return *(EqUI::CXWndManager**)0x809db4;
		}
		bool is_game_ui_window_hovered()
		{
			EqUI::CXWndManager* mgr = *(EqUI::CXWndManager**)0x809db4;
			if (mgr)
				return mgr->Hovered!=0;
			else
				return false;
		}
		bool game_wants_input()
		{
			int chat_input = EqGameInternal::UI_ChatInputCheck();
			bool focused_window_needs_input = false;
			if (is_new_ui()) {
				int focused_wnd = EqGameInternal::GetFocusWnd(*(int*)0x809db4, 0);
				if (focused_wnd)
					focused_window_needs_input = EqGameInternal::CXWndIsType(focused_wnd, 0, 2);
			}
			return chat_input!=0 || focused_window_needs_input;
		}

		void get_camera_location()
		{
			DWORD addr = *(DWORD*)0x7f99d4; //game pointer to function
			DWORD disp = *(int*)Zeal::EqGame::Display;
			DWORD a1 = *(int*)(disp + 0x8);
			DWORD a2 = (disp + 0x1c);
			__asm
			{
				push ecx
				mov ecx, disp
				push a2
				push a1
				call addr
				add esp, 0x8
				pop ecx
			}
		}
		Vec3 get_ent_head_pos(Zeal::EqStructures::Entity* ent)
		{
			Vec3 head_pos = ent->Position;
			head_pos.z += ent->Height;
			return head_pos;
		}
		Vec3 get_player_head_pos()
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			Vec3 head_pos = self->ActorInfo->DagHead->Position;
			return head_pos;
		}
		float encum_factor()
		{
			if (*(int*)EqGame::_ControlledPlayer == *(int*)EqGame::Self)
				return get_char_info()->encum_factor();
			else
				return 1.0f;
		}
		int* get_sound_manager()
		{
			return (int*)(*(int*)0x63dea8);
		}
		void DoPercentConvert(std::string& data)
		{
			char temp_buffer[2048];  // Same maximum size as internal DoPercentConvert.
			strncpy_s(temp_buffer, data.c_str(), sizeof(temp_buffer));
			temp_buffer[sizeof(temp_buffer) - 1] = 0;
			reinterpret_cast<void(__thiscall*)(int everquest, const char* name, int len)>(0x538110)(*(int*)0x809478, temp_buffer, 1);
			data = temp_buffer;
		}
		void log(std::string& data)
		{
			reinterpret_cast<void(__cdecl*)(const char* data)>(0x5240dc)(data.c_str());
		}
		Zeal::EqStructures::EQCHARINFO* get_char_info()
		{
			return (Zeal::EqStructures::EQCHARINFO*)(*(int*)0x7F94E8);
		}
		void do_autoattack(bool enabled)
		{
			reinterpret_cast<void(__thiscall*)(int, bool)>(0x5493b5)(0x798540, enabled);
		}
		const Zeal::EqStructures::EQCommand* get_command_struct(const std::string& command)
		{
			auto command_list = reinterpret_cast<const Zeal::EqStructures::EQCommand*>(0x00609af8);
			for (int i = 0; i < Zeal::EqStructures::EQCommand::kNumCommands; ++i)
			{
				if ((command_list[i].localized_name && command == command_list[i].localized_name) ||
					(command == command_list[i].name))
					return &command_list[i];
			}
			return nullptr;
		}
		int get_command_function(const std::string& command)
		{
			auto eq_command = get_command_struct(command);
			if (eq_command)
				return eq_command->fn;
			return 0;
		}
		std::vector<std::string> get_command_matches(const std::string& start_of_name) {
			std::vector<std::string> result;
			auto command_list = reinterpret_cast<const Zeal::EqStructures::EQCommand*>(0x00609af8);
			for (int i = 0; i < Zeal::EqStructures::EQCommand::kNumCommands; ++i)
			{
				const char* localized_name = command_list[i].localized_name;
				const char* name = command_list[i].name;
				if (localized_name && (start_of_name.empty() ||
					_strnicmp(localized_name, start_of_name.c_str(), start_of_name.length()) == 0))
					result.push_back(command_list[i].localized_name);

				if ((start_of_name.empty() || 
					_strnicmp(name, start_of_name.c_str(), start_of_name.length()) == 0)
					&& (!localized_name || _stricmp(localized_name, name)))
					result.push_back(command_list[i].name);
			}
			return result;
		}
		Zeal::EqStructures::ViewActor* get_view_actor()
		{
			Zeal::EqStructures::ViewActor* v = *(Zeal::EqStructures::ViewActor**)Zeal::EqGame::ViewActor;
			return v;
		}
		UINT get_eq_time()
		{
			return reinterpret_cast<UINT(__stdcall*)()>(0x4f35c7)();
		}
		int get_eq_main()
		{
			return *(int*)0x7f9574;
		}
		void SetMusicSelection(int number, bool enabled)
		{
			int* sound_manager = get_sound_manager();
			if (sound_manager)
				reinterpret_cast<void(__thiscall*)(int*, int, bool)>(0x4d54c1)(get_sound_manager(), number, enabled);
		}
		bool CanIHitTarget(float dist)
		{
			return reinterpret_cast<bool(__thiscall*)(Zeal::EqStructures::Entity*, Zeal::EqStructures::Entity*, float )>(0x509E09)(get_self(), get_target(), dist);

		}
		bool do_attack(uint8_t type, uint8_t p2)
		{
			return reinterpret_cast<bool(__thiscall*)(Zeal::EqStructures::Entity * player, uint8_t type, uint8_t p2, Zeal::EqStructures::Entity * target)>(0x50A0F8)(get_self(), type, p2, get_target());
		}
		Zeal::EqStructures::Entity* get_view_actor_entity()
		{
			return get_view_actor()->Entity;
		}
		const char* trim_name(const char* name)  // aka trimName in eqmac.exe
		{
			// Returns a pointer to a modified name stored in a 64-entry global circular buffer. Increments buffer index.
			// This function cleans the name (removes numbers, special characters) but retains any suffix.
			// Use for cleaning up name's corpse123 => name's corpse.
			if (name == NULL)
				return (char*)"";
			return reinterpret_cast<char* (__thiscall*)(int CEverquest_ptr, const char* spawnName)>(0x537D39)(*(int*)0x809478, name);
		}
		const char* strip_name(const char* name)  // aka stripName in eqmac.exe
		{
			// Returns a pointer to a modified name stored in a 64-entry global circular buffer. Does not increment buffer index.
			// Strips any numbers and any text after an apostrophe.  Use for name's corpse123 => name.
			if (name == NULL)
				return (const char*)"";
			return reinterpret_cast<char* (__thiscall*)(int CEverquest_ptr, const char* spawnName)>(0x537E4B)(*(int*)0x809478, name);
		}
		void send_message(UINT opcode, int* buffer, UINT size, int unknown)
		{
			reinterpret_cast<void(__cdecl*)(int* connection, UINT opcode, int* buffer, UINT size, int unknown)>(0x54e51a)((int*)0x7952fc, opcode, buffer, size, unknown);
		}
		bool is_view_actor_me()
		{	
			if (get_controlled() && get_controlled()->ActorInfo)
			{
				int my_view_actor = (int)get_controlled()->ActorInfo->ViewActor_;
				if ((int)get_view_actor()== my_view_actor)
					return true;
			}
			return false;
		}
		void print_group_leader()
		{
			const Zeal::EqStructures::GroupInfo* group_info = Zeal::EqGame::GroupInfo;
			if (!group_info->is_in_group())
				print_chat("You are not in a group.");
			else if (strcmp(group_info->LeaderName, Zeal::EqGame::get_char_info()->Name) == 0)
				print_chat("You are the group leader.");
			else
				print_chat("%s is the leader of your group.", group_info->LeaderName);
		}

		void print_raid_leaders(bool show_all_groups, bool show_open_groups)
		{
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid()) {
				print_chat("You are not in a raid.");
				return;
			}

			DWORD group_number = get_raid_group_number();
			std::string group_leader = get_raid_group_leader(group_number);
			if (group_leader.empty())
				print_group_leader();  // Not in a raid group, so fallback to report a normal group.
			else 
			{
				if (strcmp(group_leader.c_str(), Zeal::EqGame::get_char_info()->Name) == 0)
					print_chat("You are the leader of your raid group [%i].", group_number + 1);
				else
					print_chat("%s is the leader of your raid group [%i].",
						group_leader.c_str(), group_number + 1);

				const Zeal::EqStructures::GroupInfo* group_info = Zeal::EqGame::GroupInfo;
				if (group_info->is_in_group() && strcmp(group_leader.c_str(), group_info->LeaderName))
					print_chat("Mismatch: %s is the leader of your normal group.", group_info->LeaderName);
			}

			if (raid_info->IsLeader)
				print_chat("You are the raid leader.");
			else
				print_chat("%s is the leader of your raid.", raid_info->LeaderName);

			if (show_all_groups || show_open_groups) {
				for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers / 6; i++)
				{
					int group_count = get_raid_group_count(i);
					if (group_count == 0 && !show_all_groups)
						continue;

					group_leader = get_raid_group_leader(i);
					if (group_count == 0)
						print_chat("Group[%i]: Empty", i + 1);
					else if (group_count < 6)
						print_chat("Group[%i]: %s (%i slots open)", i + 1, group_leader.c_str(), 6 - group_count);
					else if (show_all_groups)
						print_chat("Group[%i]: %s (FULL)", i + 1, group_leader.c_str());
				}
				print_raid_ungrouped();
			}
		}

		void print_raid_ungrouped()
		{
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid()) {
				print_chat("You are not in a raid.");
				return;
			}

			bool zero_ungrouped = true;
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				const auto& member = raid_info->MemberList[i];
				if (member.GroupNumber == Zeal::EqStructures::RaidMember::kRaidUngrouped
					&& member.Name[0])
				{
					if (zero_ungrouped)
						print_chat("There are ungrouped raid members:");
					zero_ungrouped = false;
					print_chat("  %s (%s %s)", member.Name, member.PlayerLevel, member.Class);
				}
			}
			if (zero_ungrouped)
				print_chat("All raid members are grouped.");
		}

		void dump_raid_state()
		{
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid())
			{
				print_chat("You are not in a raid.");
				return;
			}

			print_chat("Raid member count: %i", raid_info->MemberCount);
			print_chat("Raid id: 0x%08x", raid_info->Id);
			print_chat("Raid leader: %s", raid_info->LeaderName);
			print_chat("Is raid leader: %i", raid_info->IsLeader);
			print_chat("Raid loot type: %i", raid_info->LootType);
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxLooters; ++i)
			{
				if (raid_info->LooterNames[i][0])
					print_chat("Looter[%i]: %s", i, raid_info->LooterNames[i]);
			}
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				if (raid_info->MemberList[i].Name[0])
				{
					const Zeal::EqStructures::RaidMember& member = raid_info->MemberList[i];
					print_chat("Member[%i]: %s, %s, %s, %i, %i, %i",
						i, member.Name, member.PlayerLevel, member.Class, member.ClassValue,
						member.IsGroupLeader, member.GroupNumber);
				}
			}
		}

		std::vector<Zeal::EqStructures::RaidMember*> get_raid_list()
		{
			std::vector<Zeal::EqStructures::RaidMember*> raid_member_list;

			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid()) {
				return raid_member_list;
			}

			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; i++) // sometimes gaps so need to check all
			{
				Zeal::EqStructures::RaidMember* raid_member = &raid_info->MemberList[i];
				if (raid_member->Name[0] != '\0') {
					raid_member_list.push_back(raid_member);
				}
			}

			return raid_member_list;
		}

		DWORD get_raid_group_number()
		{
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid())
				return Zeal::EqStructures::RaidMember::kRaidUngrouped;
			const char* self_name = Zeal::EqGame::get_char_info()->Name;
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				if (strcmp(self_name, raid_info->MemberList[i].Name) == 0)
					return raid_info->MemberList[i].GroupNumber;
			}
			return Zeal::EqStructures::RaidMember::kRaidUngrouped;
		}

		std::string get_raid_group_leader(DWORD group_number)
		{
			std::string group_leader("");
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid() || 
				group_number >= Zeal::EqStructures::RaidInfo::kRaidMaxMembers / 6)
				return group_leader;

			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				if (raid_info->MemberList[i].GroupNumber == group_number && raid_info->MemberList[i].IsGroupLeader)
				{
					// The server/client can get out of sync, so warn if the state is goofy.
					if (group_leader.empty())
						group_leader = std::string(raid_info->MemberList[i].Name);
					else
						print_chat("Error: Extra raid group leader: %s", raid_info->MemberList[i].Name);
				}
			}
			return group_leader;
		}

		int get_raid_group_count(DWORD group_number) 
		{
			Zeal::EqStructures::RaidInfo* raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid() ||
				group_number >= Zeal::EqStructures::RaidInfo::kRaidMaxMembers / 6)
				return 0;

			int group_count = 0;
			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				if (raid_info->MemberList[i].GroupNumber == group_number && raid_info->MemberList[i].Name[0])
					group_count++;
			}
			return min(6, group_count);
		}

		bool is_raid_pet(const Zeal::EqStructures::Entity& entity)
		{
			const int pet_owner_id = entity.PetOwnerSpawnId;
			if (!pet_owner_id || (entity.Type != Zeal::EqEnums::NPC))
				return false;

			const auto self = Zeal::EqGame::get_self();
			if (!self || self->SpawnId == pet_owner_id)
				return false;  // Don't hide the self pet.

			const auto raid_info = Zeal::EqGame::RaidInfo;
			if (!raid_info->is_in_raid())
				return false;

			for (int i = 0; i < Zeal::EqStructures::RaidInfo::kRaidMaxMembers; ++i)
			{
				const Zeal::EqStructures::RaidMember& member = raid_info->MemberList[i];
				if (member.Name[0] == 0)  // Empty string check.
					continue;
				auto raid_entity = ZealService::get_instance()->entity_manager->Get(member.Name);
				if (raid_entity && raid_entity->SpawnId == pet_owner_id)
					return true;
			}

			return false;
		}

		bool is_player_pet(const Zeal::EqStructures::Entity& entity)
		{
			const int pet_owner_id = entity.PetOwnerSpawnId;
			if (!pet_owner_id || (entity.Type != Zeal::EqEnums::NPC))
				return false;

			auto owner_entity = Zeal::EqGame::get_entity_by_id(pet_owner_id);
				
			return (owner_entity && owner_entity->Type == Zeal::EqEnums::Player);
		}

		Vec3 get_view_actor_head_pos()
		{
			//print_chat("movement: %i", get_view_actor()->Entity->ActorInfo->MovementType);
			//if (get_view_actor())
			//{
			//	Zeal::EqStructures::Entity* self = get_view_actor()->Entity;
			//	Vec3 head_pos = self->Position;
			//	Vec3 dag_pos = self->ActorInfo->DagHeadPoint->Position;
			//	head_pos.z = dag_pos.z;
			//	return head_pos;
			//}
			if (get_view_actor())
			{
				Zeal::EqStructures::Entity* self = get_view_actor()->Entity;
				Vec3 head_pos = self->Position;
				head_pos.z += (self->CameraHeightOffset - self->ModelHeightOffset)-0.5f; //standing
				//if (self->StandingState == Stance::Duck || self->StandingState == Stance::Sit)
				//	head_pos.z -= self->Height / 3;// self->CameraHeightOffset - self->ModelHeightOffset;
				//else if (self->StandingState != Stance::Stand)
				//	head_pos.z = self->Position.z;
				return head_pos;
			}
			else
			{
				return { 0,0,0 };
			}
		}
		bool is_in_character_select()
		{
			return *(int*)0x63d5d8!=0;
		}
		int get_region_from_pos(Vec3* pos)
		{
			static int last_good_region = 0;
			EqGameInternal::t3dGetRegionNumberFromWorldAndXYZ = mem::function<int __cdecl(int, Vec3*)>(*(int*)0x07f9a30);
			int rval = EqGameInternal::t3dGetRegionNumberFromWorldAndXYZ(*(int*)((*(int*)Zeal::EqGame::Display)+0x4), pos);
			if (rval == -1)
				rval = last_good_region;
			else
				last_good_region = rval;
			return rval;
		}
		bool collide_with_world(Vec3 start, Vec3 end, Vec3& result, char collision_type, bool debug)
		{
			DWORD disp = *(int*)Zeal::EqGame::Display;
			char x = EqGameInternal::s3dCollideSphereWithWorld(disp, 0, start.x, start.y, start.z, end.x, end.y, end.z, (float*)&result.x, (float*)&result.y, (float*)&result.z, collision_type);

			if (debug)
			{
				print_chat("start: %s  end: %s dist: %f result: %i", start.toString().c_str(), end.toString().c_str(), result.Dist(end), x);
			}
			return result.Dist(end)>0.1; //return true if there was a collision
		}

		bool can_move()
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
			if (!self)
				return false;
			if (!Zeal::EqGame::is_view_actor_me())
				return false;
			if (self->CharInfo && self->CharInfo->StunnedState)
				return false;
			if (self->StandingState == Zeal::EqEnums::Stance::Standing || self->StandingState == Zeal::EqEnums::Stance::Ducking)
				return true;
			return false;
		}

		float CalcBoundingRadius(Zeal::EqStructures::Entity* ent)
		{
			int myrace = ent->Race;
			float mysize = ent->Height;
			float base_size = 5.0f;
			int mygender = ent->Gender;
			float myradius = 6.0f;
			switch (myrace)
			{
			case 1: // "Human"
			case 2: // "Barbarian"
			case 3: // "Erudite"
			case 4: // "Wood Elf"
			case 5: // "High Elf"
			case 6: // "Dark Elf"
			case 7: // "Half Elf"
			case 8: // "Dwarf"
			case 9: // "Troll",
			case 10: // "Ogre"
			case 11: // "Halfling"
			case 12: // "Gnome"
				// playable races have fixed mods
				mysize = 5.0f;
				myradius = 5.0f;
				break;
			case 13: // "Aviak"
				break;
			case 14: // "Were Wolf"
				break;
			case 15: // "Brownie",
				break;
			case 16: // "Centaur"
				break;
			case 17: // "Golem"
				break;
			case 18: // "Giant / Cyclops"
				break;
			case 19: // "Trakanon",
				myradius = 10.48f;
				break;
			case 20:
				break;
			case 21: // "Evil Eye"
				break;
			case 22: // "Beetle"
				break;
			case 23: // "Kerra"
				break;
			case 24: // "Fish"
				break;
			case 25: // "Fairy"
				break;
			case 26: // "Old Froglok"
				break;
			case 27: // "Old Froglok Ghoul"
				break;
			case 28: // "Fungusman"
				break;
			case 29: // "Gargoyle"
				break;
			case 30: // "Gasbag"
				break;
			case 31: // "Gelatinous Cube"
				break;
			case 33: // "Ghoul"
				break;
			case 34: // "Giant Bat"
				mysize = 5.0f;
				break;
			case 36: // "Giant Rat"
				break;
			case 37: // "Giant Snake"
				break;
			case 38: // "Giant Spider"
				break;
			case 39: // "Gnoll"
				break;
			case 40: // "Goblin"
				break;
			case 41: // "Gorilla"
				break;
			case 42: // "Wolf"
				break;
			case 43: // "Bear"
				break;
			case 44: // "Freeport Guards"
				break;
			case 45: // "Demi Lich"
				break;
			case 46: // "Imp"
				break;
			case 47: // "Griffin"
				break;
			case 48: // "Kobold"
				break;
			case 49: // "Lava Dragon"
				mysize = 32.5f;
				break;
			case 50: // "Lion"
				break;
			case 51: // "Lizard Man"
				break;
			case 52: // "Mimic"
				break;
			case 53: // "Minotaur"
				break;
			case 54: // "Orc"
				break;
			case 55: // "Human Beggar"
				break;
			case 56: // "Pixie"
				break;
			case 57: // "Dracnid"
				break;
			case 58: // "Solusek Ro"
				break;
			case 59: // "Bloodgills"
				break;
			case 60: // "Skeleton"
				break;
			case 61: // "Shark"
				break;
			case 62: // "Tunare"
				break;
			case 63: // "Tiger"
				break;
			case 64: // "Treant"
				break;
			case 65: // "Vampire"
				break;
			case 66: // "Rallos Zek"
				break;
			case 67: // "Highpass Citizen"
				break;
			case 68: // "Tentacle"
				break;
			case 69: // "Will 'O Wisp"
				break;
			case 70: // "Zombie"
				break;
			case 71: // "Qeynos Citizen"
				break;
			case 74: // "Piranha"	
				break;
			case 75: // "Elemental"
				break;
			case 76: // "Puma"
				break;
			case 77: // "Neriak Citizen"
				break;
			case 78: // "Erudite Citizen"
				break;
			case 79: // "Bixie"
				break;
			case 80: // "Reanimated Hand"
				break;
			case 81: // "Rivervale Citizen"
				break;
			case 82: // "Scarecrow"
				break;
			case 83: // "Skunk"
				break;
			case 85: // "Spectre"
				break;
			case 86: // "Sphinx"
				break;
			case 87: // "Armadillo"
				break;
			case 88: // "Clockwork Gnome"
				break;
			case 89: // "Drake"
				break;
			case 90: // "Halas Citizen"
				break;
			case 91: // "Alligator"
				break;
			case 92: // "Grobb Citizen"
				break;
			case 93: // "Oggok Citizen"
				break;
			case 94: // "Kaladim Citizen"
				break;
			case 95: // "Cazic Thule"
				break;
			case 96: // "Cockatrice"
				break;
			case 98: // "Elf Vampire"
				break;
			case 99: // "Denizen"
				break;
			case 100: // "Dervish"
				break;
			case 101: // "Efreeti"
				break;
			case 102: // "Old Froglok Tadpole"
				break;
			case 103: // "Kedge"
				break;
			case 104: // "Leech"
				break;
			case 105: // "Swordfish"
				break;
			case 106: // "Felguard"
				break;
			case 107: // "Mammoth"
				break;
			case 108: // "Eye of Zomm"
				break;
			case 109: // "Wasp"
				break;
			case 110: // "Mermaid"
				break;
			case 111: // "Harpie"
				break;
			case 112: // "Fayguard"
				break;
			case 113: // "Drixie"
				break;
			case 116: // "Sea Horse"
				break;
			case 117: // "Ghost Dwarf"
				break;
			case 118: // "Erudite Ghost"
				break;
			case 119: // "Sabertooth Cat"
				break;
			case 120: // "Wolf Elemental",
				break;
			case 121: // "Gorgon"
				break;
			case 122: // "Dragon Skeleton"
				break;
			case 123: // "Innoruuk"
				break;
			case 124: // "Unicorn"
				break;
			case 125: // "Pegasus"
				break;
			case 126: // "Djinn"
				break;
			case 127: // "Invisible Man"
				break;
			case 128: // "Iksar"
				// playable races have fixed mods
				mysize = 5.0f;
				myradius = 5.0f;
				break;
			case 129: // "Scorpion"
				break;
			case 130: // "Vah Shir"
				// playable races have fixed mods
				mysize = 5.0f;
				myradius = 5.0f;
				break;
			case 131: // "Sarnak"
				break;
			case 133: // "Lycanthrope"
				break;
			case 134: // "Mosquito"
				break;
			case 135: // "Rhino"
				break;
			case 136: // "Xalgoz"
				break;
			case 137: // "Kunark Goblin"
				break;
			case 138: // "Yeti"
				break;
			case 139: // "Iksar Citizen"
				break;
			case 140: // "Forest Giant"
				break;
			case 144: // "Burynai"
				break;
			case 145: // "Goo"
				break;
			case 146: // "Spectral Sarnak"
				break;
			case 147: // "Spectral Iksar"
				break;
			case 148: // "Kunark Fish"
				break;
			case 149: // "Iksar Scorpion"
				break;
			case 150: // "Erollisi"
				break;
			case 151: // "Tribunal"
				break;
			case 153: // "Bristlebane"
				break;
			case 154: // "Fay Drake"
				break;
			case 155: // "Sarnak Skeleton"
				break;
			case 156: // "Ratman"
				break;
			case 157: // "Wyvern"
				break;
			case 158: // "Wurm"
				mysize = 16.0f;
				break;
			case 159: // "Devourer"
				break;
			case 160: // "Iksar Golem"
				break;
			case 161: // "Iksar Skeleton"
				break;
			case 162: // "Man Eating Plant"
				break;
			case 163: // "Raptor"
				break;
			case 164: // "Sarnak Golem"
				break;
			case 165: // "Water Dragon"
				break;
			case 166: // "Iksar Hand"
				break;
			case 167: // "Succulent"
				break;
			case 168: // "Flying Monkey"
				break;
			case 169: // "Brontotherium"
				break;
			case 170: // "Snow Dervish"
				break;
			case 171: // "Dire Wolf"
				break;
			case 172: // "Manticore"
				break;
			case 173: // "Totem"
				break;
			case 174: // "Cold Spectre"
				break;
			case 175: // "Enchanted Armor"
				break;
			case 176: // "Snow Bunny"
				break;
			case 177: // "Walrus"
				break;
			case 178: // "Rock-gem Men"
				break;
			case 181: // "Yak Man"
				break;
			case 183: // "Coldain"
				break;
			case 184: // "Velious Dragons"
				myradius = 9.48f;
				break;
			case 185: // "Hag"
				break;
			case 187: // "Siren"
				break;
			case 188: // "Frost Giant"
				break;
			case 189: // "Storm Giant"
				break;
			case 190: // "Ottermen"
				break;
			case 191: // "Walrus Man"
				break;
			case 192: // "Clockwork Dragon"
				myradius = 10.48f;
				break;
			case 193: // "Abhorent"
				break;
			case 194: // "Sea Turtle"
				break;
			case 195: // "Black and White Dragons"
				myradius = 9.48f;
				break;
			case 196: // "Ghost Dragon"
				myradius = 9.48f;
				break;
			case 198: // "Prismatic Dragon"
				myradius = 9.48f;
				break;
			case 199: // "ShikNar"
				break;
			case 200: // "Rockhopper"
				break;
			case 201: // "Underbulk"
				break;
			case 202: // "Grimling"
				break;
			case 203: // "Vacuum Worm"
				break;
			case 205: // "Kahli Shah"
				break;
			case 206: // "Owlbear"
				break;
			case 207: // "Rhino Beetle"
				break;
			case 208: // "Vampyre"
				break;
			case 209: // "Earth Elemental"
				break;
			case 210: // "Air Elemental"
				break;
			case 211: // "Water Elemental"
				break;
			case 212: // "Fire Elemental"
				break;
			case 213: // "Wetfang Minnow"
				break;
			case 214: // "Thought Horror"
				break;
			case 215: // "Tegi"
				break;
			case 216: // "Horse"
				break;
			case 217: // "Shissar"
				break;
			case 218: // "Fungal Fiend"
				break;
			case 219: // "Vampire Volatalis"
				break;
			case 220: // "StoneGrabber"
				break;
			case 221: // "Scarlet Cheetah"
				break;
			case 222: // "Zelniak"
				break;
			case 223: // "Lightcrawler"
				break;
			case 224: // "Shade"
				break;
			case 225: // "Sunflower"
				break;
			case 226: // "Sun Revenant"
				break;
			case 227: // "Shrieker"
				break;
			case 228: // "Galorian"
				break;
			case 229: // "Netherbian"
				break;
			case 230: // "Akheva"
				break;
			case 231: // "Spire Spirit"
				break;
			case 232: // "Sonic Wolf"
				break;
			case 234: // "Vah Shir Skeleton"
				break;
			case 235: // "Mutant Humanoid"
				break;
			case 236: // "Seru"
				break;
			case 237: // "Recuso"
				break;
			case 238: // "Vah Shir King"
				break;
			case 239: // "Vah Shir Guard"
				break;
			case 241: // "Lujein",
			case 242: // "Naiad",
			case 243: // "Nymph",
			case 244: // "Ent",
			case 245: // "Fly Man",
			case 246: // "Tarew Marr"
				break;
			case 247: // "Sol Ro"
				break;
			case 248: // "Clockwork Golem"
				break;
			case 249: // "Clockwork Brain",
			case 250: // "Spectral Banshee",
			case 251: // "Guard of Justice",
			case 252: // 'PoM Castle',
			case 253: // "Disease Boss"
			case 254: // "Sol Ro Guard"
			case 255: // "New Bertox",
			case 256: // "New Tribunal",
			case 257: // "Terris Thule",
			case 258: // "Vegerog",
			case 259: // "Crocodile",
			case 260: // "Bat",
			case 261: // "Slarghilug",
			case 262: // "Tranquilion"
			case 263: // "Tin Soldier"
			case 264: // "Nightmare Wraith",
			case 265: // "Malarian",
			case 266: // "Knight of Pestilence",
			case 267: // "Lepertoloth",
			case 268: // "Bubonian Boss",
			case 269: // "Bubonian Underling",
			case 270: // "Pusling",
			case 271: // "Water Mephit",
			case 272: // "Stormrider",
			case 273: // "Junk Beast"
				break;
			case 274: // "Broken Clockwork"
				break;
			case 275: // "Giant Clockwork",
			case 276: // "Clockwork Beetle",
			case 277: // "Nightmare Goblin",
			case 278: // "Karana",
			case 279: // "Blood Raven",
			case 280: // "Nightmare Gargoyle",
			case 281: // "Mouths of Insanity",
			case 282: // "Skeletal Horse",
			case 283: // "Saryn",
			case 284: // "Fennin Ro",
			case 285: // "Tormentor",
			case 286: // "Necro Priest",
			case 287: // "Nightmare",
			case 288: // "New Rallos Zek",
			case 289: // "Vallon Zek",
			case 290: // "Tallon Zek",
			case 291: // "Air Mephit",
			case 292: // "Earth Mephit",
			case 293: // "Fire Mephit",
			case 294: // "Nightmare Mephit",
			case 295: // "Zebuxoruk",
			case 296: // "Mithaniel Marr",
			case 297: // "Undead Knight",
			case 298: // "The Rathe",
			case 299: // "Xegony",
			case 300: // "Fiend",
			case 301: // "Test Object",
			case 302: // "Crab",
			case 303: // "Phoenix",
			case 304: // "PoP Dragon",
			case 305: // "PoP Bear",
			case 306: // "Storm Taarid",
			case 307: // "Storm Satuur",
			case 308: // "Storm Kuraaln",
			case 309: // "Storm Volaas",
			case 310: // "Storm Mana",
			case 311: // "Storm Fire",
			case 312: // "Storm Celestial",
			case 313: // "War Wraith",
			case 314: // "Wrulon",
			case 315: // "Kraken",
			case 316: // "Poison Frog",
			case 317: // "Queztocoatal",
			case 318: // "Valorian",
			case 319: // "War Boar",
			case 320: // "PoP Efreeti",
			case 321: // "War Boar Unarmored",
			case 322: // "Black Knight",
			case 323: // "Animated Armor",
			case 324: // "Undead Footman",
			case 325: // "Rallos Zek Minion"
			case 326: // "Arachnid"
			case 327: // "Crystal Spider",
			case 328: // "Zeb Cage",
			case 329: // "BoT Portal",
			case 330: // "Froglok",
			case 331: // "Troll Buccaneer",
			case 332: // "Troll Freebooter",
			case 333: // "Troll Sea Rover",
			case 334: // "Spectre Pirate Boss",
			case 335: // "Pirate Boss",
			case 336: // "Pirate Dark Shaman",
			case 337: // "Pirate Officer",
			case 338: // "Gnome Pirate",
			case 339: // "Dark Elf Pirate",
			case 340: // "Ogre Pirate",
			case 341: // "Human Pirate",
			case 342: // "Erudite Pirate",
			case 343: // "Poison Dart Frog",
			case 344: // "Troll Zombie",
			case 345: // "Luggald Land",
			case 346: // "Luggald Armored",
			case 347: // "Luggald Robed",
			case 348: // "Froglok Mount",
			case 349: // "Froglok Skeleton",
			case 350: // "Undead Froglok",
			case 351: // "Chosen Warrior",
			case 352: // "Chosen Wizard",
			case 353: // "Veksar",
			case 354: // "Greater Veksar",
			case 355: // "Veksar Boss",
			case 356: // "Chokadai",
			case 357: // "Undead Chokadai",
			case 358: // "Undead Veksar",
			case 359: // "Vampire Lesser",
			case 360: // "Vampire Elite",	
				break;
			default:
				myradius = 6.0;
				break;

			}
			myradius *= mysize;
			myradius /= base_size;
			return myradius;
		}

		float CalcZOffset(Zeal::EqStructures::Entity* ent)
		{
			float size = std::clamp(ent->Height, 0.f, 20.f); //rule has bestzsizemax at 20 on server code
			switch (ent->Race)
			{
			case 49: //dragon
			case 158: //wurm
			case 196: //ghost dragon
				return 20.f;
			case 63: //tiger
				return (size / 5.0f * 3.125f * 0.44999999f);
			case 42: //wolf
			case 120: //wolf elemntal
				if (ent->Gender == 2)
					return (size / 5.0f * 3.125f * 0.44999999f);
			}
			return (size / 5.0f * 3.125f);
		}

		float CalcCombatRange(Zeal::EqStructures::Entity* entity1, Zeal::EqStructures::Entity* entity2)
		{
			float size_mod = (CalcBoundingRadius(entity1) + CalcBoundingRadius(entity2)) * 0.75f;
			float z_diff = std::abs(CalcZOffset(entity1) - CalcZOffset(entity2));
			if (entity2->MovementSpeed > 0)
				size_mod += 2.0f;
			size_mod += z_diff;
			size_mod = std::clamp(size_mod, 14.f, 75.f);
			return size_mod;
		}

		std::vector<Zeal::EqStructures::Entity*> get_world_visible_actor_list(float max_dist, bool only_targetable)
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			get_camera_location();
			DWORD disp = *(int*)Zeal::EqGame::Display;
			int ent_count = 0;
			DWORD addr = *(DWORD*)0x7f9850;
			int a1 = *(int*)(disp + 0x4);
			int a2 = *(int*)(disp + 0x8);
			int a3 = (disp + 0x2CD0);
			float mdist=max_dist;
			__asm
			{
				push ecx
				mov ecx, disp
				push a3
				push 0x11
				push ecx
				fstp max_dist
				push 0
				push a2
				push a1
				call addr
				add esp, 0x18
				mov ent_count, eax
				pop ecx
			}
			int* cObject = *(int**)(disp + 0x2CDC);
			Zeal::EqStructures::Entity* current_ent;
			
			auto char_info = Zeal::EqGame::get_char_info();
			bool can_see_invis = char_info && char_info->can_i_see_invis();
			std::vector<Zeal::EqStructures::Entity*> rEnts;
			for (int i = 0; i < ent_count; i++)
			{
				if (cObject[i])
				{
					bool add_to_list = !only_targetable;
					current_ent = *(Zeal::EqStructures::Entity**)(cObject[i] + 0x60);
					if (!current_ent || current_ent == self || current_ent->TargetType > 0x40)
						continue;  // Skip self or invalid target spawn types.
					if (current_ent->Position.Dist2D(self->Position)<= mdist)
					{
						if (only_targetable)
						{
							Vec3 result;
							Vec3 ent_head = get_ent_head_pos(current_ent);
							Vec3 my_head = self->Position;
							my_head.z += self->Height;
							std::vector<std::pair<Vec3, Vec3>> collision_checks =
							{
								{my_head, ent_head}, //face to face
								{my_head, current_ent->Position}, //your face to their feet
								{self->Position, current_ent->Position}, //your feet to their feet
								{self->Position, ent_head}, //your feet to their face
							};
						
							for (int i = 0; auto& [pos1, pos2] : collision_checks)
							{
								if (!collide_with_world(pos1, pos2, result, 0x3, false)) //had no collision
								{
									add_to_list = true;
									break; //we don't really care which version of this had no world collision so break the for loop
								}
								i++;
							}
						}
						bool is_visible = (current_ent->IsHidden != 0x01) ||
							(can_see_invis && (self->IsGameMaster || !current_ent->IsGameMaster));
						if (add_to_list && is_visible)
							rEnts.push_back(current_ent);
					}
				}
			}
			return rEnts;
		}

		float get_target_attack_fade_factor(float speed_factor) {
			if (!(bool)(*(BYTE*)0x7f6ffe))  // Auto attack enabled.
				return 1.0f; // No fading.

			// Calculate a fraction of the cycle time. Phase alignment doesn't matter, so just
			// do a modulo off of the current time in millseconds.
			const float client_cycle_time_ms = 300.f;  // Base client flicker cycle rate is ~300 ms.
			const float cycle_time_ms = max(2.f, (speed_factor <= 0) ? 2.f : 
				(client_cycle_time_ms * speed_factor));
			const float fraction = static_cast<float>(
				fmod(static_cast<double>(GetTickCount64()), cycle_time_ms)) / cycle_time_ms;

			// Fade in during the first half of the cycle and out the second.
			const float fade_factor = (fraction < 0.5) ? (fraction * 2) : (1.0f - fraction) * 2;
			return fade_factor;
		}

		std::vector<std::string> splitStringByNewLine(const std::string& str) {
			std::vector<std::string> tokens;
			std::istringstream iss(str);
			std::string token;

			while (std::getline(iss, token, '\n')) {
				tokens.push_back(token);
			}

			return tokens;
		}

		void do_say(bool hide_local, const char* format, ...)
		{
			byte orig[13] = {0};
			if (hide_local)
				mem::set(0x538672, 0x90, 13, orig);

			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);

			EqGameInternal::do_say(get_self(), buffer);

			if (hide_local && orig)
			{
				mem::copy(0x538672, orig, 13);
			}

		}
		void do_say(bool hide_local, std::string data)
		{
			byte orig[13] = {0};
			if (hide_local)
				mem::set(0x538672, 0x90, 13, orig);

			EqGameInternal::do_say(get_self(), data.c_str());

			if (hide_local && orig)
			{
				mem::copy(0x538672, orig, 13);
			}
		}
		void do_gsay(std::string data)
		{
			EqGameInternal::do_gsay(get_self(), data.c_str());
		}
		void do_guildsay(std::string data)
		{
			EqGameInternal::do_guildsay(get_self(), data.c_str());
		}
		void do_auction(std::string data)
		{
			EqGameInternal::do_auction(get_self(), data.c_str());
		}
		void do_ooc(std::string data)
		{
			EqGameInternal::do_ooc(get_self(), data.c_str());
		}
		void send_raid_chat(std::string data)
		{
			EqGameInternal::send_raid_chat(Zeal::EqGame::RaidInfo, 0, data.c_str());
		}
		void print_chat(std::string data)
		{
			if (!is_in_game())
			{
				ZealService::get_instance()->print_buffer.push_back(data);
				return;
			}
			std::vector<std::string> vd = splitStringByNewLine(data);
			for (auto& d : vd)
				EqGameInternal::print_chat(*(int*)0x809478, 0, d.c_str(), 0, true);
		}
		void print_chat(const char* format, ...)
		{

			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			if (!is_in_game())
			{
				ZealService::get_instance()->print_buffer.push_back(buffer);
				return;
			}			
			EqGameInternal::print_chat(*(int*)0x809478, 0, buffer, 0, true);
		}
		void __fastcall PrintChat(int t, int unused, const char* data, short color_index, bool u) {};

		void print_chat_hook(const char* format, ...)
		{
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			if (!is_in_game())
			{
				ZealService::get_instance()->print_buffer.push_back(buffer);
				return;
			}
			ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(*(int*)0x809478, 0, buffer, 0, true);
		}
		void print_debug(const char* format, ...)
		{
			va_list argptr;
			char buffer[512];
			char buffer_with_newline[514]; // Additional space for the newline and null terminator

			va_start(argptr, format);
			vsnprintf(buffer, sizeof(buffer), format, argptr);
			va_end(argptr);

			// Append newline character to the formatted string
			snprintf(buffer_with_newline, sizeof(buffer_with_newline), "%s\n", buffer);

			OutputDebugStringA(buffer_with_newline);
		}
		void print_chat(short color, const char* format, ...)
		{
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			if (!is_in_game())
			{
				ZealService::get_instance()->print_buffer.push_back(buffer);
				return;
			}
			EqGameInternal::print_chat(*(int*)0x809478, 0, buffer, color, true);

		}

		void print_chat_wnd(Zeal::EqUI::ChatWnd* wnd, short color, const char* format, ...)
		{
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			if (!is_in_game())
			{
				ZealService::get_instance()->print_buffer.push_back(buffer);
				return;
			}
			//eal::EqUI::ChatWnd* wnd, int u, Zeal::EqUI::CXSTR msg, short channel)
			Zeal::EqUI::CXSTR cxBuff = Zeal::EqUI::CXSTR(buffer);
			reinterpret_cast<void(__thiscall*)(Zeal::EqUI::ChatWnd*, Zeal::EqUI::CXSTR msg, short channel)>(ZealService::get_instance()->hooks->hook_map["AddOutputText"]->trampoline)(wnd, cxBuff, color);
			cxBuff.FreeRep();
		}
		int get_gamestate()
		{
			if (get_eq())
				return get_eq()->game_state;
			return -1;
		}
		EqStructures::Everquest* get_eq()
		{
			return *(EqStructures::Everquest**)0x809478;
		}
		void do_inspect(Zeal::EqStructures::Entity* player)
		{
			reinterpret_cast<void(__thiscall*)(EqStructures::Everquest*, Zeal::EqStructures::Entity*)>(0x54390E)(get_eq(), player);
		}
		void pet_command(int cmd, short spawn_id)
		{
			reinterpret_cast<void(__thiscall*)(EqStructures::Everquest*, int, short)>(0x547749)(get_eq(), cmd, spawn_id);
		}
		void execute_cmd(UINT cmd, bool isdown, int unk2)
		{
			reinterpret_cast<void(__cdecl*)(UINT, bool, int)>(0x54050c)(cmd, isdown, unk2);
		}
		std::string generateTimestamp() {
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);

			std::ostringstream oss;
			oss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
			return oss.str();
		}
		bool use_item(int item_index, bool quiet)
		{
			Zeal::EqStructures::EQCHARINFO* chr = Zeal::EqGame::get_char_info();
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			if (!chr || !self)
			{
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "[Fatal Error] Failed to get charinfo for useitem!");
				return false;
			}
			Zeal::EqStructures::EQITEMINFO* item = nullptr;
			if (item_index < 0 || item_index > 29)
			{
				Zeal::EqGame::print_chat("useitem requires an item slot between 0 and 29, you tried to use %i", item_index);
				return false;
			}
			if (item_index < 21)
				item = chr->InventoryItem[item_index];
			else
				item = chr->InventoryPackItem[item_index - 22]; //-22 to make it back to 0 index

			if (!item)
			{
				if (!quiet)
					Zeal::EqGame::print_chat("There isn't an item at %i", item_index);
				return false;
			}
			if (item->Type != 0 || !item->Common.SpellId)
			{
				if (!quiet)
					Zeal::EqGame::print_chat("Item %s does not have a spell attached to it.", item->Name);
				return false;
			}
			// List of checks copied from eqemu/zone/client_packet.cpp:
			if ((item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectClick) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectExpendable) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectEquipClick) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectClick2))
			{
				if (!quiet)
					Zeal::EqGame::print_chat("Item %s does not have a click effect.", item->Name);
				return false;
			}
			if (!self->ActorInfo || self->ActorInfo->CastingSpellId != kInvalidSpellId)
			{
				Zeal::EqGame::print_chat(USERCOLOR_SPELLS, "You must stop casting to cast this spell!");
				return false;
			}
			if (self->StandingState != Stance::Stand && self->StandingState != Stance::Sit)       //on quarm spell casting has autostand while sitting
			{
				Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "You must be standing to cast a spell.");
				return false;
			}
			chr->cast(0xA, 0, (int*)&item, item_index < 21 ? item_index + 1 : item_index);
			return true;
		}
		Zeal::EqStructures::Entity* get_active_corpse()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::Active_Corpse;
		}
		Zeal::EqStructures::Entity* get_target()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::Target;
		}
		void set_target(Zeal::EqStructures::Entity* target)
		{
			auto old_target = Zeal::EqGame::get_target();
			if (!target)
				print_chat(get_string(0x3057)); //you no longer have a target
			*(Zeal::EqStructures::Entity**)Zeal::EqGame::Target = target;

			// Note: The change in target will get detected in EQ_Character::DoPassageOfTime() which
			// is called by the RenderReal_World() code. That sends a TargetMouse opcode (0x4162)
			// that will keep the server in sync and also trigger a target HP update packet response.
		}
		void do_target(const char* name)
		{
			reinterpret_cast<void(__cdecl*)(int, const char*)>(0x4FD9A7)(0, name);
		}
		Zeal::EqStructures::Entity* get_entity_list()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::EntListPtr;
		}

		long get_user_color(int index)
		{
			index -= 1;
			long _param_1 = reinterpret_cast<long(__cdecl*)(int)>(0x4AA2C1)(index);
			if (_param_1 == 0)
				return 0xFFFFFFFF;
			return (_param_1 & 0xff00 | _param_1 >> 0x10 & 0xff | (_param_1 | 0xffffff00) << 0x10);
		}

		Zeal::EqStructures::Entity* get_entity_by_id(short id)
		{
			if ((id < 0) || (id >= kEntityIdArraySize))
				return nullptr;
			return EntityIdArray[id];
		}
		Zeal::EqStructures::Entity* get_entity_by_parent_id(short parent_id)
		{
			Zeal::EqStructures::Entity* current_ent = get_entity_list();
			while (current_ent->Next)
			{
				if (current_ent->PetOwnerSpawnId == parent_id)
					return current_ent;
				current_ent = current_ent->Next;
			}
			return 0;
		}
		Zeal::EqStructures::SPELLMGR* get_spell_mgr()
		{
			return *(Zeal::EqStructures::SPELLMGR**)0x805CB0;
		}
		int get_spell_level(int spell_id)
		{
			Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
			const auto* spell_mgr = Zeal::EqGame::get_spell_mgr();
			if (spell_id < 1 || spell_id >= EQ_NUM_SPELLS || !spell_mgr || !char_info)
				return -1;

			const auto* spell = spell_mgr->Spells[spell_id];
			if (!spell)
				return -1;

			return spell->ClassLevel[char_info->Class];
		}
		const char* get_spell_name(int spell_id) 
		{
			const auto* spell_mgr = Zeal::EqGame::get_spell_mgr();
			if (spell_id < 1 || spell_id >= EQ_NUM_SPELLS || !spell_mgr)
				return nullptr;

			const auto* spell = spell_mgr->Spells[spell_id];
			if (!spell)
				return nullptr;

			return spell->Name;
		}
		Zeal::EqStructures::Entity* get_self()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::Self;
		}
		Zeal::EqStructures::Entity* get_pet()
		{
			Zeal::EqStructures::Entity* pet_entity = NULL;
			Zeal::EqStructures::Entity* self = get_self();

			if (self && self->ActorInfo)
			{
				int pet_position = self->ActorInfo->PetID;
				if (pet_position)
				{
					pet_entity = Zeal::EqGame::get_entity_by_id(pet_position);
				}
			}
			return pet_entity;
		}
		Zeal::EqStructures::Entity* get_controlled()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::_ControlledPlayer;
		}
		Zeal::EqStructures::CameraInfo* get_camera()
		{
			return *(Zeal::EqStructures::CameraInfo**)Zeal::EqGame::CameraInfo;
		}
		int* get_display()
		{
			return *(int**)Zeal::EqGame::Display;
		}
		bool is_mouse_hovering_window()
		{
			return *Zeal::EqGame::mouse_hover_window!=0;
		}
	
		//void set_camera_position(Vec3* pos)
		//{
		//	int cam_position_ptr = *(int*)((*(int*)Zeal::EqGame::Display) + 0x8);
		//	Vec3* cam_pos = (Vec3*)(cam_position_ptr) + 0xC);
		//	*cam_pos = *pos;
		//}

		void CXStr_PrintString(Zeal::EqUI::CXSTR* str, const char* format, ...)
		{
				va_list argptr;
				char buffer[512];
				va_start(argptr, format);
				//printf()
				vsnprintf(buffer, 511, format, argptr);
				va_end(argptr);

				EqGameInternal::CXStr_PrintString(str, buffer);
		}

		const char* get_aa_title_name(BYTE class_id, int aa_rank, BYTE gender_id)
		{
			const char* title_name = "";
			switch (class_id)
			{
			case Zeal::EqEnums::ClassTypes::Warrior:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Veteran";
				if (aa_rank == 3)
					title_name = "Marshall";	
				break;
			case Zeal::EqEnums::ClassTypes::Cleric:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Venerable";
				if (aa_rank == 3)
					title_name = "Exarch";
				break;
			case Zeal::EqEnums::ClassTypes::Paladin:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Sir";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Lady";
				if (aa_rank == 3 && gender_id == 0)
					title_name = "Duke";
				if (aa_rank == 3 && gender_id == 1)
					title_name = "Duchess";
				break;
			case Zeal::EqEnums::ClassTypes::Ranger:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Veteran";
				if (aa_rank == 3 && gender_id == 0)
					title_name = "Hunter";
				if (aa_rank == 3 && gender_id == 1)
					title_name = "Huntress";
				break;
			case Zeal::EqEnums::ClassTypes::Shadowknight:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Sir";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Lady";
				if (aa_rank == 3 && gender_id == 0)
					title_name = "Duke";
				if (aa_rank == 3 && gender_id == 1)
					title_name = "Duchess";
				break;
			case Zeal::EqEnums::ClassTypes::Druid:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Venerable";
				if (aa_rank == 3)
					title_name = "Elder";
				break;
			case Zeal::EqEnums::ClassTypes::Monk:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Brother";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Sister";
				if (aa_rank == 3)
					title_name = "Sensei";
				break;
			case Zeal::EqEnums::ClassTypes::Bard:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Veteran";
				if (aa_rank == 3 && gender_id == 0)
					title_name = "Impresario";
				if (aa_rank == 3 && gender_id == 1)
					title_name = "Muse";
				break;
			case Zeal::EqEnums::ClassTypes::Rogue:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Veteran";
				if (aa_rank == 3)
					title_name = "Marauder";
				break;
			case Zeal::EqEnums::ClassTypes::Shaman:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Venerable";
				if (aa_rank == 3)
					title_name = "Elder";
				break;
			case Zeal::EqEnums::ClassTypes::Necromancer:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Sage";
				if (aa_rank == 3)
					title_name = "Lich";
				break;
			case Zeal::EqEnums::ClassTypes::Wizard:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Master";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Mistress";
				if (aa_rank == 3)
					title_name = "Sage";
				break;
			case Zeal::EqEnums::ClassTypes::Magician:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Master";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Mistress";
				if (aa_rank == 3)
					title_name = "Sage";
				break;
			case Zeal::EqEnums::ClassTypes::Enchanter:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2 && gender_id == 0)
					title_name = "Master";
				if (aa_rank == 2 && gender_id == 1)
					title_name = "Mistress";
				if (aa_rank == 3)
					title_name = "Sage";
				break;
			case Zeal::EqEnums::ClassTypes::Beastlord:
				if (aa_rank == 1 && gender_id == 0)
					title_name = "Barron";
				if (aa_rank == 1 && gender_id == 1)
					title_name = "Barroness";
				if (aa_rank == 2)
					title_name = "Venerable";
				if (aa_rank == 3)
					title_name = "Elder";
				break;
			default:
				title_name = "";
				break;
			}
			return title_name;
		}

		std::string class_name(int class_id)
		{
			std::string class_string = "";
			int modified_class_id = class_id;
			if (modified_class_id > 16 && modified_class_id < 32)
				modified_class_id -= 16;
			switch (modified_class_id)
			{
			case Zeal::EqEnums::ClassTypes::Warrior:
				class_string = "Warrior";
				break;
			case Zeal::EqEnums::ClassTypes::Cleric:
				class_string = "Cleric";
				break;
			case Zeal::EqEnums::ClassTypes::Paladin:
				class_string = "Paladin";
				break;
			case Zeal::EqEnums::ClassTypes::Ranger:
				class_string = "Ranger";
				break;
			case Zeal::EqEnums::ClassTypes::Shadowknight:
				class_string = "Shadowknight";
				break;
			case Zeal::EqEnums::ClassTypes::Druid:
				class_string = "Druid";
				break;
			case Zeal::EqEnums::ClassTypes::Monk:
				class_string = "Monk";
				break;
			case Zeal::EqEnums::ClassTypes::Bard:
				class_string = "Bard";
				break;
			case Zeal::EqEnums::ClassTypes::Rogue:
				class_string = "Rogue";
				break;
			case Zeal::EqEnums::ClassTypes::Shaman:
				class_string = "Shaman";
				break;
			case Zeal::EqEnums::ClassTypes::Necromancer:
				class_string = "Necromancer";
				break;
			case Zeal::EqEnums::ClassTypes::Wizard:
				class_string = "Wizard";
				break;
			case Zeal::EqEnums::ClassTypes::Magician:
				class_string = "Magician";
				break;
			case Zeal::EqEnums::ClassTypes::Enchanter:
				class_string = "Enchanter";
				break;
			case Zeal::EqEnums::ClassTypes::Beastlord:
				class_string = "Beastlord";
				break;
			case Zeal::EqEnums::ClassTypes::Banker:
				class_string = "Banker";
				break;
			case Zeal::EqEnums::ClassTypes::Merchant:
				class_string = "Merchant";
				break;
			default:
				class_string = "Unknown";
				break;
			}
			if (class_id > 16 && class_id < 32)
				class_string += " GuildMaster";
			return class_string;
		}
		std::string class_name_short(int class_id)
		{
			std::string class_string = "";
			int modified_class_id = class_id;
			if (modified_class_id > 16 && modified_class_id < 32)
				modified_class_id -= 16;
			switch (modified_class_id)
			{
			case Zeal::EqEnums::ClassTypes::Warrior:
				class_string = "War";
				break;
			case Zeal::EqEnums::ClassTypes::Cleric:
				class_string = "Clr";
				break;
			case Zeal::EqEnums::ClassTypes::Paladin:
				class_string = "Pal";
				break;
			case Zeal::EqEnums::ClassTypes::Ranger:
				class_string = "Rng";
				break;
			case Zeal::EqEnums::ClassTypes::Shadowknight:
				class_string = "Shd";
				break;
			case Zeal::EqEnums::ClassTypes::Druid:
				class_string = "Dru";
				break;
			case Zeal::EqEnums::ClassTypes::Monk:
				class_string = "Mnk";
				break;
			case Zeal::EqEnums::ClassTypes::Bard:
				class_string = "Brd";
				break;
			case Zeal::EqEnums::ClassTypes::Rogue:
				class_string = "Rog";
				break;
			case Zeal::EqEnums::ClassTypes::Shaman:
				class_string = "Shm";
				break;
			case Zeal::EqEnums::ClassTypes::Necromancer:
				class_string = "Nec";
				break;
			case Zeal::EqEnums::ClassTypes::Wizard:
				class_string = "Wiz";
				break;
			case Zeal::EqEnums::ClassTypes::Magician:
				class_string = "Mag";
				break;
			case Zeal::EqEnums::ClassTypes::Enchanter:
				class_string = "Enc";
				break;
			case Zeal::EqEnums::ClassTypes::Beastlord:
				class_string = "Bst";
				break;
			case Zeal::EqEnums::ClassTypes::Banker:
				class_string = "Banker";
				break;
			case Zeal::EqEnums::ClassTypes::Merchant:
				class_string = "Merchant";
				break;
			default:
				class_string = "Unknown";
				break;
			}
			if (class_id > 16 && class_id < 32)
				class_string += " GuildMaster";
			return class_string;
		}
		int get_showname()
		{
			// Holds value of /showname command.
			//  1 = first names, 2 = first/last names, 3 = first/last/guild names, 4 = everything
			return *reinterpret_cast<int32_t*>(0x007d01e4);
		}
		std::string get_full_zone_name(int zone_id) {
			const int fn_GetFullZoneName = 0x00523e49;
			void* pWorld = *reinterpret_cast<void**>(0x007F9494);
			char buffer[512];  // LongName is only 0x80 in EQZONEINFO.
			buffer[0] = 0;  // Returns "Unknown Zone" if invalid index 
			reinterpret_cast<void(__thiscall*)(void* this_world, int zone_id, char* buffer)>(fn_GetFullZoneName)(pWorld, zone_id, buffer);
			return std::string(buffer);
		}
		std::string get_zone_name_from_index(int zone_id) {
			const int fn_GetZoneNameFromIndex = 0x00523f73;
			void* pWorld = *reinterpret_cast<void**>(0x007F9494);
			char buffer[512];  // ShortName is only 0x20 in EQZONEINFO.
			if (!reinterpret_cast<bool(__thiscall*)(void* this_world, int zone_id, char* buffer)>(fn_GetZoneNameFromIndex)(pWorld, zone_id, buffer))
				buffer[0] = 0;  // Return an empty buffer if not found.
			return std::string(buffer);
		}
		int get_index_from_zone_name(const std::string& name) {
			const int fn_GetIndexFromZoneName = 0x00523fa4;  // Returns 0 if not found.
			void* pWorld = *reinterpret_cast<void**>(0x007F9494);
			return reinterpret_cast<int(__thiscall*)(void* this_world, const char* buffer)>(fn_GetIndexFromZoneName)(pWorld, name.c_str());
		}
		std::string get_class_desc(int class_id) {
			const int fn_GetClassDesc = 0x0052d5f1;
			auto pEverquest = Zeal::EqGame::get_eq();
			auto desc = reinterpret_cast<const char* (__thiscall*)(Zeal::EqStructures::Everquest*, int)>(fn_GetClassDesc)(pEverquest, class_id);
			return std::string(desc);
		}
		std::string get_title_desc(int class_id, int aa_rank, int gender) {
			const int fn_GetTitleDesc = 0x0052eb69;
			auto pEverquest = Zeal::EqGame::get_eq();
			auto desc = reinterpret_cast<const char* (__thiscall*)(Zeal::EqStructures::Everquest*, int, int, int)>(fn_GetTitleDesc)(pEverquest, class_id, aa_rank, gender);
			return std::string(desc);
		}
		std::string get_player_guild_name(short guild_id) {
			// Roughly equivalent to:
			// return (guild_id == -1) ? "Unknown" : Zeal::EqGame::guild_names->Guild[guildId].Name;
			const int fn_GetPlayerGuildName = 0x0054c7e1;
			auto desc = reinterpret_cast<const char* (*)(short)>(fn_GetPlayerGuildName)(guild_id);
			return std::string(desc);
		}
		bool is_targetable(Zeal::EqStructures::Entity* ent)
		{
			if (!ent || !ent->ActorInfo || ent->ActorInfo->IsInvisible || ent->TargetType > 0x40)
				return false;

			if (ent->IsHidden == 0x01)
			{
				auto self = Zeal::EqGame::get_self();
				auto char_info = Zeal::EqGame::get_char_info();
				if (self && char_info)
				{
					bool can_see_invis = char_info && char_info->can_i_see_invis();
					return (can_see_invis && (self->IsGameMaster || !ent->IsGameMaster));
				}
				return false;
			}
			return true;
		}
		bool is_in_game()
		{
			if (get_gamestate() != -1)
				return get_gamestate() == GAMESTATE_INGAME;
			else
				return false;
		}
		bool is_new_ui()
		{
			return *(BYTE*)0x8092D8;
		}
		HWND get_game_window()
		{
			HMODULE eqw = GetModuleHandleA("eqw.dll");
			if (eqw)
				return *(HWND*)((DWORD)eqw + 0x97A4);
			else
				return 0;
		}


		namespace Spells
		{
			void OpenBook()
			{
				if (!Windows->SpellBook)
					return;
				Windows->SpellBook->OpenBook();
			}
			void StopSpellBookAction() {
				if (!Windows->SpellBook)
					return;
				Windows->SpellBook->StopSpellBookAction();
			}
			void Memorize(int book_index, int gem_index)
			{
				if (!Windows->SpellBook)
					return;
				if (!Windows->SpellBook->IsVisible)
					Zeal::EqGame::Spells::OpenBook();
				ZealService::get_instance()->callbacks->AddDelayed([book_index, gem_index]() {
					if (Windows->SpellBook->IsVisible && Zeal::EqGame::get_self()->StandingState==Stance::Sit)
						Windows->SpellBook->BeginMemorize(book_index, gem_index, false);
				}, 25);
				
			}
			void Forget(int index) 
			{
				if (!Windows->SpellGems)
					return;
				Windows->SpellGems->Forget(index);
			}
			void UpdateGems(int index)
			{
				if (!Windows->SpellGems)
					return;
				Windows->SpellGems->UpdateSpellGems(index);
			}
		}

		namespace OldUI
		{
			bool spellbook_window_open()
			{
				// ISSUE: There is currently a small edge case where chat scrollbar usage can cause the value we're checking to flicker.
				// ISSUE: Spamming chat while in spellboolk ultimately causes chat to scroll which makes the value flicker like the above issue.
				HMODULE dx8 = GetModuleHandleA("eqgfx_dx8.dll");
				// feedback/help window increase offset of pointer by 44, but they also get hit by game_wants_input(), so don't bother check them.
				if (dx8)
				{
					int offset = EQ_NUM_SPELL_GEMS * 88;
					for (size_t i = 0; i < EQ_NUM_SPELL_GEMS; ++i)
						if (Zeal::EqGame::get_char_info()->MemorizedSpell[i] == -1)
							offset -= 88;

					if (Zeal::EqGame::get_target()) { offset += 44; }
					bool view_button_clicked = *(DWORD*)((DWORD)dx8 + (0x3CD1C4 + offset)) != ULONG_MAX; // weird offset edge case (view hotkey not included)
					if (view_button_clicked) { offset += 44; }
					return *(DWORD*)((DWORD)dx8 + (0x3CD1C4 + offset)) == ULONG_MAX;
				}
				else
				{
					return false;
				}
			}
		}

		// Primitive comparison with simple numeric string support.
		static bool sort_list_wnd_compare(const std::vector<std::string>& a,
			const std::vector<std::string>& b, int sort_column)
		{
			int a_num = std::atoi(a[sort_column].c_str());
			int b_num = std::atoi(b[sort_column].c_str());
			if (a_num != 0 && b_num != 0)  // Both are non-zero numerics strings, so numeric comparison.
				return a_num < b_num;
			return a[sort_column] < b[sort_column];  // Else use default string comparison.
		}

		// Sorts the contents of a list window by the sort_column index and sort_type
		void sort_list_wnd(Zeal::EqUI::ListWnd* list_wnd, int sort_column, SortType sort_type)
		{
			if (!list_wnd)
				return;

			const int num_rows = list_wnd->ItemCount;
			if (num_rows < 2)
				return;

			// The CListWnd::SetItemText() code showed that the number of columns was accessed
			// by a pointer at +0xfc that points to an array of 28 byte structures with the column
			// value in the second integer index.
			if (list_wnd->ColInfoArray == nullptr)
				return;
			const int num_cols = list_wnd->ColInfoArray[0].ColCount;
			if (sort_column < 0 || sort_column >= num_cols)
				return;

			// Copy data into a standard 2-D structure for sorting.
			std::vector<std::vector<std::string>> data;
			Zeal::EqUI::CXSTR temp;
			for (int r = 0; r < num_rows; ++r)
			{
				// Temporary logging / sanity check that there are equal number of columns for each row.
				if (list_wnd->ColInfoArray[r].ColCount != num_cols) {
					Zeal::EqGame::print_chat("Sorting column mismatch: %d vs %d",
									num_cols, list_wnd->ColInfoArray[r].ColCount);
					return;
				}
				data.push_back(std::vector<std::string>());
				for (int c = 0; c < num_cols; ++c)
				{
					list_wnd->GetItemText(&temp, r, c);
					data[r].push_back(std::string(temp));
				}
			}
			temp.FreeRep();  // Need to release the temporary reference count.

			bool ascending = (sort_type == SortType::Ascending) ||
				(sort_type == SortType::Toggle &&
					sort_list_wnd_compare(data.back(), data.front(), sort_column));
			if (ascending)
				std::sort(data.begin(), data.end(), [sort_column](const std::vector<std::string>& a,
					const std::vector<std::string>& b)
					{ return sort_list_wnd_compare(a, b, sort_column); });
			else
				std::sort(data.rbegin(), data.rend(), [sort_column](const std::vector<std::string>& a,
					const std::vector<std::string>& b)
					{ return sort_list_wnd_compare(a, b, sort_column); });


			for (int r = 0; r < num_rows; ++r)
			{
				for (int c = 0; c < num_cols; ++c)
					list_wnd->SetItemText(data[r][c], r, c);
			}
		}

	}
}
