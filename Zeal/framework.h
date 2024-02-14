#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "hook_wrapper.h"
#include "EqFunctions.h"
//hooks
#include "cycle_target.h"
#include "mouse_zoom.h"
#include "looting.h"
#include "FindPattern.h"
#include "labels.h"
#include "binds.h"
#include "commands.h"
#include "main_loop.h"
#include "experience.h"
#include "Zeal.h"
extern HMODULE this_module;
