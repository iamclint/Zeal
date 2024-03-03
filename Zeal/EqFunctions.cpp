#include <windows.h>
#include "EqFunctions.h"
#include "EqAddresses.h"
namespace Zeal
{
	namespace EqGame
	{
		float heading_to_yaw(float heading)
		{
			float y = 512 - heading;
			y *= 0.703125f;
			if (y < 0)
				y += 360;
			return y;
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
		bool is_game_ui_window_hovered()
		{
			if (EqGame::WndManager)
				return EqGame::WndManager->ptr_hovered_wnd!=0;
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
			Vec3 head_pos = self->Position;
			head_pos.z += (self->CameraHeightOffset - self->ModelHeightOffset); //standing
			if (self->StandingState == Stance::Duck || self->StandingState == Stance::Sit)
				head_pos.z -= self->Height/3;// self->CameraHeightOffset - self->ModelHeightOffset;
			else if (self->StandingState!=Stance::Stand)
				head_pos.z = self->Position.z;
			return head_pos;
		}
		float encum_factor()
		{
			if (*(int*)EqGame::_ControlledPlayer == *(int*)EqGame::Self)
				return EqGameInternal::encum_factor(*Zeal::EqGame::ptr_LocalPC, 0);
			else
				return 1.0f;
		}
		Zeal::EqStructures::ViewActor* get_view_actor()
		{
			Zeal::EqStructures::ViewActor* v = *(Zeal::EqStructures::ViewActor**)Zeal::EqGame::ViewActor;
			return v;
		}

		Zeal::EqStructures::Entity* get_view_actor_entity()
		{
			return get_view_actor()->Entity;
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




		Vec3 get_view_actor_head_pos()
		{
			if (get_view_actor())
			{
				Zeal::EqStructures::Entity* self = get_view_actor()->Entity;
				Vec3 head_pos = self->Position;
				head_pos.z += (self->CameraHeightOffset - self->ModelHeightOffset); //standing
				if (self->StandingState == Stance::Duck || self->StandingState == Stance::Sit)
					head_pos.z -= self->Height / 3;// self->CameraHeightOffset - self->ModelHeightOffset;
				else if (self->StandingState != Stance::Stand)
					head_pos.z = self->Position.z;
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
						if (current_ent != EqGame::get_self() && !current_ent->IsHidden && current_ent->HpCurrent > 0 && add_to_list)
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
			std::vector<std::string> vd = splitStringByNewLine(data);
			for (auto& d : vd)
				EqGameInternal::print_chat(*(int*)0x809478, 0, d.c_str(), 0, false);
		}
		void print_chat(const char* format, ...)
		{
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			EqGameInternal::print_chat(*(int*)0x809478, 0, buffer, 0, false);

		}
		void print_chat_zeal(const char* data, short color, bool un)
		{
			std::string msg = "[ZEAL] " + std::string(data);

			EqGameInternal::print_chat(*(int*)0x809478, 0, msg.c_str(), color, un);
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
			*(Zeal::EqStructures::Entity**)Zeal::EqGame::Target = target;
		}
		Zeal::EqStructures::Entity* get_entity_list()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::EntListPtr;
		}
		Zeal::EqStructures::Entity* get_entity_by_id(short id)
		{
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

		void CXStr_PrintString(Zeal::EqStructures::CXSTR* str, const char* format, ...)
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
			return *Zeal::EqGame::in_game;
		}
		void change_stance(Stance new_stance)
		{
			if (Self && Self->StandingState != (BYTE)new_stance)
				EqGameInternal::change_stance(get_self(), 0, new_stance); //EQPlayer::ChangePosition
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
			int GetCastSpellWnd()
			{
				return *(int*)0x63d648;
			}
			int GetSpellBookWnd()
			{
				return *(int*)0x63d64C;
			}
			void OpenBook()
			{
				EqGameInternal::Spells::OpenBook(GetSpellBookWnd(), 0);
			}
			void Memorize(int book_index, int gem_index)
			{
				EqGameInternal::Spells::BeginMemorize(GetSpellBookWnd(), 0, book_index, gem_index, false);
			}
			void Forget(int index) 
			{
				EqGameInternal::Spells::ForgetMemorizedSpell(GetCastSpellWnd(), 0, index);
			}
			void UpdateGems(int index)
			{
				EqGameInternal::Spells::UpdateSpellGems(GetCastSpellWnd(), 0, index);
			}
		}

	}
}