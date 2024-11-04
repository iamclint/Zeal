#include <windows.h>
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"
namespace Zeal
{
	namespace EqGame
	{
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
		bool can_equip_item(Zeal::EqStructures::EQITEMINFO* item)
		{
			Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
			if (!item || !c)
				return false;
			
			using FunctionType2 = bool(__thiscall*)(Zeal::EqStructures::EQCHARINFO* char_info, Zeal::EqStructures::EQITEMINFO* iItem);
			using FunctionType  = bool(__cdecl*)(Zeal::EqStructures::EQCHARINFO* char_info, UINT equipable_slots, UINT slot, Zeal::EqStructures::EQITEMINFO * iItem);
			FunctionType check_loc = reinterpret_cast<FunctionType>(0x4F0DB4);
			FunctionType2 can_use_item = reinterpret_cast<FunctionType2>(0x4BB8E8);
			if (!can_use_item(c, item))
				return false;
			for (int i = 0; i < EQ_NUM_INVENTORY_SLOTS; i++)
			{
				if (check_loc(c, item->EquipableSlots, i+1, item) && !c->InventoryItem[i])
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
			reinterpret_cast<void(__thiscall*)(int everquest, const char* name, int len)>(0x538110)(*(int*)0x809478, data.data(), 1);
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
		char* strip_name(char* name)
		{
			return reinterpret_cast<char*(__thiscall*)(int everquest, char* name)>(0x537e4b)(*(int*)0x809478, name);
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

		std::vector<Zeal::EqStructures::RaidMember*> get_raid_list()
		{
			short raid_size = *(short*)0x794F9C;
			std::vector<Zeal::EqStructures::RaidMember*> raid_member_list;

			if (raid_size <= 0) {
				return raid_member_list;
			}

			for (int i = 0; i < 72; i++) // 12 groups x 6 members per group = 72 slots, sometimes gaps so need to check all
			{
				Zeal::EqStructures::RaidMember* raid_member = (Zeal::EqStructures::RaidMember*)(RaidMemberList + (0x0000D0 * i));
				if (raid_member->Name[0] != '\0') {
					raid_member_list.push_back(raid_member);
				}
			}

			return raid_member_list;
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
			
			std::vector<Zeal::EqStructures::Entity*> rEnts;
			for (int i = 0; i < ent_count; i++)
			{
				if (*cObject)
				{
					bool add_to_list = !only_targetable;
					current_ent = *(Zeal::EqStructures::Entity**)(*cObject + 0x60);
					if (current_ent && current_ent->Position.Dist2D(Zeal::EqGame::get_self()->Position)<= mdist)
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
						if (current_ent != EqGame::get_self() && !current_ent->IsHidden && add_to_list)
							rEnts.push_back(current_ent);
					}
				}
				cObject += 1;
			}
			return rEnts;
		}

		bool can_target(Zeal::EqStructures::Entity* ent)
		{
			bool rval=1;
			DWORD addr = 0x4afa90;
			DWORD self = (DWORD)EqGame::get_self();
			DWORD x = (DWORD)EqGame::get_display();
			__asm
			{
				push ent
				push self
				mov ecx, x
				call addr
				mov rval, al
			}
			return rval;
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
				mem::set(0x4f828b, 0x90, 13, orig);

			EqGameInternal::do_say(get_self(), data.c_str());

			if (hide_local && orig)
			{
				mem::copy(0x4f828b, orig, 13);
			}
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
		bool use_item(int item_index)
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
				Zeal::EqGame::print_chat("There isn't an item at %i", item_index);
				return false;
			}
			if (item->Type != 0 || !item->Common.SpellId)
			{
				Zeal::EqGame::print_chat("Item %s does not have a spell attached to it.", item->Name);
				return false;
			}
			// List of checks copied from eqemu/zone/client_packet.cpp:
			if ((item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectClick) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectExpendable) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectEquipClick) &&
				(item->Common.EffectType != Zeal::EqEnums::ItemEffect::ItemEffectClick2))
			{
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
			if (!target)
				print_chat(get_string(0x3057)); //you no longer have a target
			*(Zeal::EqStructures::Entity**)Zeal::EqGame::Target = target;
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
			if (id == get_controlled()->SpawnId)
				return get_controlled();
			Zeal::EqStructures::Entity* current_ent = get_entity_list();
			while (current_ent->Next)
			{
				if (current_ent->SpawnId == id)
					return current_ent;
				current_ent = current_ent->Next;
			}
			return 0;
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
		Zeal::EqStructures::Entity* get_self()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::Self;
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
				class_string = "Sk";
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
				class_string = "Sha";
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

		bool is_targetable(Zeal::EqStructures::Entity* ent)
		{
			
			if (!ent->IsHidden && !ent->ActorInfo->IsInvisible)
				return true;
			return false;
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
				Windows->SpellBook->OpenBook();
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
				Windows->SpellGems->Forget(index);
			}
			void UpdateGems(int index)
			{
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
	}
}
