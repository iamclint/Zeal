#include "memory.h"
#include <TlHelp32.h>
#include <regex>
#include <Psapi.h>
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

namespace mem
{
	uint64_t FindPattern(uint64_t rangeStart, uint64_t rangeEnd, const char* pattern)
	{
		const char* pat = pattern;
		uint64_t firstMatch = 0;
		for (uint64_t pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch)
				{
					firstMatch = pCur;
				}
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = pattern;
				if (firstMatch != 0)
					pCur = firstMatch + 1;
				firstMatch = 0;
			}
		}
		if (firstMatch != 0)
			return firstMatch;
		else
			return NULL;
	}

	uint64_t find_pattern(HMODULE module, const char* pattern)
	{
		MODULEINFO mInf;
		GetModuleInformation(GetCurrentProcess(), module, &mInf, sizeof(mInf));
		return FindPattern((uint64_t)mInf.lpBaseOfDll, (uint64_t)((uint64_t)mInf.lpBaseOfDll + mInf.SizeOfImage), pattern);
	}


	HMODULE find_module(std::string regex_str)
	{
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(NULL));
		if (hModuleSnap == INVALID_HANDLE_VALUE)
			return nullptr;
		me32.dwSize = sizeof(MODULEENTRY32);
		if (!Module32First(hModuleSnap, &me32))
		{
			CloseHandle(hModuleSnap);
			return nullptr;
		}
		while (Module32Next(hModuleSnap, &me32))
		{
			std::wstring mn = me32.szModule;
			std::string name;
			std::transform(mn.begin(), mn.end(), std::back_inserter(name), [](wchar_t c) {
				return (char)c;
				});

			std::regex re(regex_str, std::regex_constants::icase);
			std::smatch match;
			if (std::regex_search(name, match, re))
				return me32.hModule;
		}
		CloseHandle(hModuleSnap);

		return nullptr;
	}
	std::unordered_map<PVOID, mem_protect> protections;
	int instruction_to_absolute_address(int instruction_address) //assumes 32 bit
	{
		int end_of_instruction = instruction_address + 0x5;
		unprotect_memory((PVOID)instruction_address, 5);
		BYTE instruction = *(BYTE*)instruction_address;
		int r = 0;
		if (instruction == 0xE8 || instruction == 0xE9)
			r = (*(int*)(instruction_address + 0x1)) + end_of_instruction;
		else if (instruction == 0xFF)
		{
			r = (*(int*)(instruction_address + 0x2));
			r = *(int*)r;
		}
		reset_memory_protection((PVOID)instruction_address);
		return r;
	}
	void unprotect_memory(PVOID target, size_t size)
	{
		protections[target].size = size;
		VirtualProtect((PVOID*)target, size, PAGE_EXECUTE_READWRITE, &protections[target].orig);

	}
	void reset_memory_protection(PVOID target)
	{
		if (protections[target].size)
			VirtualProtect((PVOID*)target, protections[target].size, protections[target].orig, nullptr);
	}
	void set(int target, int val, int size, BYTE* buffer)
	{
		DWORD oldprotect;
		VirtualProtect((PVOID*)target, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		if (buffer)
			memcpy(buffer, (void*)target, size);
		memset((void*)target, val, size);
		VirtualProtect((PVOID*)target, size, oldprotect, &oldprotect);
	}
	void copy(int target, int source, int size, BYTE* buffer)
	{
		DWORD oldprotect;
		VirtualProtect((PVOID*)target, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		if (buffer)
			memcpy((void*)buffer, (const void*)target, size);
		memcpy((void*)target, (const void*)source, size);
		VirtualProtect((PVOID*)target, size, oldprotect, &oldprotect);
	}
	void copy(int target, BYTE* source, int size, BYTE* buffer)
	{
		DWORD oldprotect;
		VirtualProtect((PVOID*)target, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		if (buffer)
			memcpy((void*)buffer, (const void*)target, size);
		memcpy((void*)target, (const void*)source, size);
		VirtualProtect((PVOID*)target, size, oldprotect, &oldprotect);
	}
	void get(int target, int size, BYTE* buffer)
	{
		if (!buffer)
			return;
		DWORD oldprotect;
		VirtualProtect((PVOID*)target, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy((void*)buffer, (const void*)target, size);
		VirtualProtect((PVOID*)target, size, oldprotect, &oldprotect);
	}
}