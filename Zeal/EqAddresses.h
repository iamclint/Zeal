#pragma once
#include "EqStructures.h"
namespace Zeal
{
	namespace EqGame
	{
		static EqStructures::Entity* Self = (Zeal::EqStructures::Entity*)0x7F94CC;
		static EqStructures::Entity* Target = (Zeal::EqStructures::Entity*)0x7F94EC;
		static EqStructures::Entity* Active_Corpse = (Zeal::EqStructures::Entity*)0x7f9500;
		static EqStructures::Entity* EntListPtr = (Zeal::EqStructures::Entity*)0x7f9498;
		static EqStructures::CameraInfo* CameraInfo = (Zeal::EqStructures::CameraInfo*)0x63B928;
		static EqStructures::Entity* GroupList = (Zeal::EqStructures::Entity*)0x7913F8;
		static EqStructures::ViewActor* ViewActor = (EqStructures::ViewActor*)0x63D6C0;
		static EqStructures::KeyboardModifiers* KeyMods = (EqStructures::KeyboardModifiers*)0x799738;
		static DWORD* ptr_LocalPC = (DWORD*)0x7F94E8;
		static int* ptr_ContainerMgr = (int*)0x63d6b8;
		static int* ptr_COptionsWnd = (int*)0x63d634;
		static int* ptr_PrimaryKeyMap = (int*)0x7CD84C;
		static int* ptr_AlternateKeyMap = (int*)0x7CDC4C;
		static BYTE* strafe_direction = (BYTE*)0x7985EB;
		static float* strafe_speed = (float*)0x799780;
		static EqStructures::Entity* _ControlledPlayer = (EqStructures::Entity*)0x7f94e0;
		static int* Display = (int*)0x7F9510;
		static EqStructures::Cam* camera = (EqStructures::Cam*)0x799688;// 0x7996C0;
		static bool* is_right_mouse_down = (bool*)0x7985ea;
		static int* mouse_hover_window = (int*)0x809DD8; //unsure
		static int* camera_view = (int*)0x63BE68;
		static int max_pitch = 0x5e86d0;
		static EqStructures::KeyboardInput* KeyInput = (EqStructures::KeyboardInput*)0x7ce058;
		static char* in_game = (char*)0x798550;
		
		//Vec3* camera_position = *(Vec3**)0x9c08128;
	}
}
//int pWorld = 0x7F9494;