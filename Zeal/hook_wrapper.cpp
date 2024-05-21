#include "hook_wrapper.h"

namespace Zeal
{
	namespace Memory
	{
		HookWrapper Hooks;
	}
}
void hook::replace(int addr, int dest)
{
	if (*(BYTE*)addr == 0xE9 || *(BYTE*)addr == 0xE8)
	{
		destination = (DWORD)dest;
		address = (DWORD)addr;
		trampoline = mem::instruction_to_absolute_address(addr);
		DWORD old;
		VirtualProtect((LPVOID)addr, 0x5, PAGE_EXECUTE_READWRITE, &old);
		memcpy(original_bytes, (LPVOID)addr, 5);
		*(DWORD*)(addr + 1) = dest - addr - 5;
		VirtualProtect((LPVOID)addr, 0x5, old, NULL);
	}
}

void hook::detour(int addr, int dest)
{
	int orig_to_dest_offset = dest - addr - 5;
	DWORD old_protect;
	if (*(byte*)addr == 0xE9) //a jump (already hooked by something lets play nice)
	{
		orig_byte_count = 5;
		trampoline = (int)malloc(orig_byte_count);
		VirtualProtect((LPVOID)trampoline, orig_byte_count, PAGE_EXECUTE_READWRITE, &old_protect);
		int jmp_absolute = mem::instruction_to_absolute_address(addr);
		int trampoline_to_orig_offset = jmp_absolute - trampoline - 5;
		mem::write<byte>(trampoline, 0xE9);
		mem::write<int>(trampoline + 1, trampoline_to_orig_offset);
		mem::write<byte>(addr, 0xE9);
		mem::write<int>(addr + 1, orig_to_dest_offset);
		//there is no need to write a jump back to the original since we are just going to jump to their hook
	}
	else
	{
		// Build a trampoline
		trampoline = (int)malloc(orig_byte_count + 5); // A jump is 5 bytes
		VirtualProtect((LPVOID)trampoline, orig_byte_count+5, PAGE_EXECUTE_READWRITE, &old_protect);
		mem::copy(trampoline, original_bytes, orig_byte_count);
		// Calculate the relative offsets
		int trampoline_to_orig_offset = addr - trampoline - orig_byte_count;

		// Write the relative jump instruction at the end of the trampoline
		mem::write<byte>(trampoline + orig_byte_count, 0xE9);
		mem::write<int>(trampoline + orig_byte_count + 1, trampoline_to_orig_offset);

		// Write the relative jump instruction at the original address
		mem::write<byte>(addr, 0xE9);
		mem::write<int>(addr + 1, orig_to_dest_offset);

		// If there are more than 5 bytes of original instructions, fill the gap with NOPs
		if (orig_byte_count > 5)
			mem::set(addr + 5, 0x90, orig_byte_count - 5);
	}
	
}

void hook::replace_call(int addr, int dest)
{
	orig_byte_count = 5;
	replace(addr, dest);
	//mem::copy((int)&original_bytes, (BYTE*)dest, 5);
}
void hook::replace_vtable(int addr, int index, int dest)
{
	mem::copy((int)&original_bytes, (BYTE*)dest, 4);
	int orig_addr = ((int*)addr)[index];
	DWORD old_protect;
	orig_byte_count = 4;
	trampoline = (int)malloc(5);
	VirtualProtect((LPVOID)trampoline, 5, PAGE_EXECUTE_READWRITE, &old_protect);
	mem::write<byte>(trampoline, 0xE9);
	mem::write<int>(trampoline + 1, orig_addr);
	mem::write<int>(addr, dest);
}
void hook::replace_vtable(int addr, int dest)
{
	DWORD old_protect;
	trampoline = (int)malloc(5);
	VirtualProtect((LPVOID)trampoline, 5, PAGE_EXECUTE_READWRITE, &old_protect);
	int trampoline_to_orig_offset = *(int*)addr - trampoline - 5;
	mem::write<byte>(trampoline, 0xE9);
	mem::write<int>(trampoline + 1, trampoline_to_orig_offset);
	mem::write<int>(addr, dest);
}
void hook::rehook()
{

	if (hook_type == hook_type_detour)
		detour(address, destination);
	else
		replace_call(address, destination);
}

void hook::remove()
{
	mem::copy(address, original_bytes, orig_byte_count);
	free((void*)trampoline);
}
