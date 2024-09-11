#include "ui_group.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include <cctype>
using NameArrayType = char[0x40];
using PtrArrayType = Zeal::EqStructures::Entity*;

PtrArrayType* ptrArray = reinterpret_cast<PtrArrayType*>(0x7913F8);
NameArrayType* nameArray = reinterpret_cast<NameArrayType*>(0x7912B5);

void ui_group::InitUI()
{

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
	zeal->commands_hook->Add("/sortgroup", {"/sg"}, "",
		[this](std::vector<std::string>& args) 
		{
			sort();
			return true;
		});
	if (Zeal::EqGame::is_in_game()) InitUI();
}
