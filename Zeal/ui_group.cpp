#include "ui_group.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <algorithm>
#include <cctype>
using NameArrayType = char[0x40];
using PtrArrayType = Zeal::EqStructures::Entity*;

PtrArrayType* ptrArray = reinterpret_cast<PtrArrayType*>(0x7913F8);
NameArrayType* nameArray = reinterpret_cast<NameArrayType*>(0x7912B5);

void ui_group::InitUI()
{

}
void ui_group::swap(UINT index1, UINT index2)
{
	if (index1 < 5 && index2 < 5)
	{
		std::pair<std::string, Zeal::EqStructures::Entity*> Ent1 = std::make_pair(nameArray[index1], ptrArray[index1]);
		std::pair<std::string, Zeal::EqStructures::Entity*> Ent2 = std::make_pair(nameArray[index2], ptrArray[index2]);

		//move the strings
		mem::copy((DWORD)(nameArray[index1]), (DWORD)Ent2.first.c_str(), Ent2.first.length()+1); //+1 don't forget string terminator
		mem::copy((DWORD)(nameArray[index2]), (DWORD)Ent1.first.c_str(), Ent1.first.length()+1);
		ptrArray[index1] = Ent2.second;
		ptrArray[index2] = Ent1.second;
	}
	else
	{
		Zeal::EqGame::print_chat("Error moving group members: %i to %i", index1, index2);
	}
}
void ui_group::sort()
{
	std::vector<std::pair<std::string, Zeal::EqStructures::Entity*>> group_members_sorted;
	for (int i = 0; i < 5; i++)
	{
		std::pair<std::string, Zeal::EqStructures::Entity*> ent_pair = std::make_pair(nameArray[i], ptrArray[i]);
		group_members_sorted.push_back({ nameArray[i], ptrArray[i]});
	}
	std::sort(group_members_sorted.begin(), group_members_sorted.end(),
		[](const std::pair<std::string, Zeal::EqStructures::Entity*>& a, const std::pair<std::string, Zeal::EqStructures::Entity*>& b) {
			// If 'a' is empty and 'b' is not, 'b' should come first (return true)
			if (a.first.empty() && !b.first.empty()) {
				return false;
			}
			// If 'b' is empty and 'a' is not, 'a' should come first (return true)
			if (!a.first.empty() && b.first.empty()) {
				return true;
			}

			std::string lower_a = a.first;
			std::string lower_b = b.first;

			std::transform(lower_a.begin(), lower_a.end(), lower_a.begin(), ::tolower);
			std::transform(lower_b.begin(), lower_b.end(), lower_b.begin(), ::tolower);

			return lower_a < lower_b; 
		});
	mem::set(0x7913F8, 0, 20);//erase the pointer array 4 bytes * 5 members = 20 bytes
	mem::set(0x7912B5, 0, 320);//erase the entire name array 64 characters * 5 members = 320 bytes
	for (int i = 0; i < group_members_sorted.size(); ++i) {
		mem::copy((DWORD)(nameArray[i]), (DWORD)group_members_sorted[i].first.c_str(), group_members_sorted[i].first.length());
		ptrArray[i] = group_members_sorted[i].second;
	}
}
ui_group::~ui_group()
{

}
ui_group::ui_group(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->commands_hook->Add("/sortgroup", {"/sg"}, "Sort your group members example usages: /sg or /sg 1 2 where /sg alpha sorts the group and /sg 1 2 switches players 1 and 2 in your group window ",
		[this](std::vector<std::string>& args) 
		{
			int index1 = 0;
			int index2 = 0;
			if (args.size() > 2 && Zeal::String::tryParse(args[1], &index1) && Zeal::String::tryParse(args[2], &index2))
				swap(index1-1, index2-1); //makes this easier for end users so /sortgroup 1 2 will swap players 1 and 2 in your group
			else
				sort();
			return true;
		});
	if (Zeal::EqGame::is_in_game()) InitUI();
}
