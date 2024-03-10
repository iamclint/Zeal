#pragma once
#include "memory.h"
#include <unordered_map>
#include "InstructionLength.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "psapi.lib")
#define czVOID(c) (void)c
enum hook_type_
{
	hook_type_replace_call,
	hook_type_jmp,
	hook_type_detour,
	hook_type_vtable
};
class hook
{
private:
	void replace_vtable(int addr, int index, int dest);
	void replace_vtable(int addr, int dest);
	void replace_call(int addr, int dest);
	void replace(int addr, int dest);
	void detour(int addr, int dest);
	BYTE* original_bytes;
public: //methods
	void remove();
	~hook()
	{
		remove();
	}
	hook() : orig_byte_count{0}, address {}, original_bytes{}, destination{}, trampoline{}, hook_type{ hook_type_detour } { };
	template<typename X, typename T>
	hook(X addr, T dest, hook_type_ hooktype = hook_type_detour, int byte_count = 5)
	{
		orig_byte_count = byte_count;
		original_bytes = new BYTE(byte_count);
		address = (int)addr;
		destination = (int)dest;
		hook_type = hooktype;
		trampoline = 0;
		mem::copy((int)original_bytes, (BYTE*)addr, byte_count);

		switch (hook_type)
		{
			case hook_type_detour:
			{
				detour((int)addr, (int)dest);
				break;
			}
			case hook_type_replace_call:
			{
				replace_call((int)addr, (int)dest);
				break;
			}
			case hook_type_jmp:
			{
				replace_call((int)addr, (int)dest);
				break;
			}
			case hook_type_vtable:
			{
				replace_vtable((int)addr, (int)dest);
				break;
			}
		}
	}
	template<typename T>
	T original(T fnType) {
		czVOID(fnType);
		return (T)trampoline;
	}
	void rehook();
public: //variables
	int destination;
	int address;
	int trampoline;
	int orig_byte_count;
	hook_type_ hook_type;
};
class HookWrapper
{
public:
	std::unordered_map<std::string, hook*> hook_map;
	template<typename X, typename T>
	hook* Add(std::string name, X addr, T fnc, hook_type_ type, int byte_count = -1) //could have used zdys or capstone but keeping this a single compile with minimal libs and its not that hard to figure out the bytes
	{
		if (type != hook_type_vtable)
		{
			if (byte_count == -1)
			{
				byte_count = Zeal::InstructionLength((unsigned char*)addr);
				while (byte_count < 5) //you need 5 bytes for a jmp
					byte_count += Zeal::InstructionLength((unsigned char*)(addr + byte_count));
			}
		}
		else
			byte_count = 4;
		hook* x = new hook(addr, fnc, type, byte_count);
		hook_map[name] = x;
		x->hook_type = type;


		return x;
	}
	~HookWrapper()
	{
		for (auto& hook : hook_map)
			hook.second->remove();
	}
};

