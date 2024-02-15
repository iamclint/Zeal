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

		bool s3dCollideSphereWithWorld(EqStructures::ActorLocation& acp)
		{
			DWORD disp = *(int*)Zeal::EqGame::Display;
			DWORD addr = 0x4b3c45;
			float c_x = *(float*)(disp + 0x1c);
			float c_y = *(float*)(disp + 0x20);
			float c_z = *(float*)(disp + 0x24);

			char x = EqGameInternal::s3dCollideSphereWithWorld(disp, 0, c_x, c_y, c_z + 5.0, acp.Position.x, acp.Position.y, acp.Position.z + 5, nullptr, nullptr, nullptr, 0x3);
			return x==0;
		}

		std::vector<Zeal::EqStructures::Entity*> get_world_visible_actor_list(float max_dist, bool only_targetable)
		{
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
					bool add_to_list = true;
					current_ent = *(Zeal::EqStructures::Entity**)(*cObject + 0x60);
					if (current_ent && current_ent->Position.Dist2D(Zeal::EqGame::get_self()->Position)<= mdist)
					{
						if (only_targetable)
						{
							Zeal::EqStructures::ActorLocation aloc = get_actor_location((int)*cObject);
							bool x = s3dCollideSphereWithWorld(aloc);
							if (!x)
							{
								aloc.Position.z += 20;
								x = s3dCollideSphereWithWorld(aloc);
								if (!x)
									add_to_list = false;
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
		void print_chat(std::string data)
		{
			std::string msg = "[ZEAL] " + std::string(data);

			EqGameInternal::print_chat(*(int*)0x809478, 0, msg.c_str(), 5, false);
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
		int set_camera_position(Zeal::EqStructures::CameraSetting pos)
		{
			static mem::function<int __fastcall(int* t, int unk, int disp_4, int disp_8, Zeal::EqStructures::CameraSetting* pos)> SetCameraLocation = *(int*)Zeal::EqGame::EqGameInternal::fn_s_t3dSetCameraLocation;
			int display = *(int*)Zeal::EqGame::Display;
			if (pos.Heading==0)
				return SetCameraLocation(get_display(), 0, *(int*)(display + 4), *(int*)(display + 8), *(Zeal::EqStructures::CameraSetting**)(display + 0x1C));
			else
				return SetCameraLocation(get_display(), 0, *(int*)(display + 4), *(int*)(display + 8), &pos);
		}

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

		std::string ClassName(int class_id)
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
		std::string ClassNameShort(int class_id)
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
			if (Self->StandingState != (BYTE)new_stance)
			{
				//print_chat("Current Stance 0x%x   New Stance 0x%x", Self->StandingState, new_stance);
				EqGameInternal::change_stance(get_self(), 0, new_stance);
			}
			else
			{
				//print_chat("Cannot change to new stance as its already your stance");
			}
		}
	}
}