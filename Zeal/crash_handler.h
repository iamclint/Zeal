#pragma once
#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <fstream>
#include <string>
class CrashHandler
{
public:
	CrashHandler();
	~CrashHandler();
private:
	PVOID exception_handler;
};

