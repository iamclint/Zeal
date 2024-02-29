#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "hook_wrapper.h"
#include "EqFunctions.h"
//hooks
#include "commands.h"
#include "camera_mods.h"
#include "looting.h"
#include "FindPattern.h"
#include "labels.h"
#include "binds.h"
#include "raid.h"
#include "eqstr.h"
#include "chat.h"
#include "IO_ini.h"
#include "main_loop.h"
// other features
#include "cycle_target.h"
#include "outputfile.h"
#include "experience.h"
#include "buff_timers.h"

#include "Zeal.h"

extern HMODULE this_module;
