#include "FindPattern.h"
namespace Zeal
{
	namespace Memory
	{

		BOOLEAN MaskCompare(PVOID buffer, LPCSTR pattern, LPCSTR mask) {
			for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
				if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
					return FALSE;
				}
			}

			return TRUE;
		}

		DWORD FindPattern(LPCSTR pattern, LPCSTR mask, HMODULE target_module) {
			MODULEINFO info = { 0 };
			if (target_module == 0)
				target_module = GetModuleHandle(0);
			GetModuleInformation(GetCurrentProcess(), target_module, &info, sizeof(info));

			info.SizeOfImage -= static_cast<DWORD>(strlen(mask));

			for (auto i = 0UL; i < info.SizeOfImage; ++i) {
				auto addr = reinterpret_cast<PBYTE>(info.lpBaseOfDll) + i;
				if (MaskCompare(addr, pattern, mask)) {
					return (DWORD)addr;
				}
			}
		}
	}
}