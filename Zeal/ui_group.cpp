#include "ui_group.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include <algorithm>
#include <cctype>


void ui_group::InitUI()
{

}
void ui_group::swap(UINT index1, UINT index2)
{
	auto* group_info = Zeal::EqGame::GroupInfo;
	if (index1 < 5 && index2 < 5)
	{
		std::pair<std::string, Zeal::EqStructures::Entity*> Ent1 = std::make_pair(group_info->Names[index1], group_info->EntityList[index1]);
		std::pair<std::string, Zeal::EqStructures::Entity*> Ent2 = std::make_pair(group_info->Names[index2], group_info->EntityList[index2]);

		//move the strings
		mem::copy((DWORD)(group_info->Names[index1]), (DWORD)Ent2.first.c_str(), Ent2.first.length()+1); //+1 don't forget string terminator
		mem::copy((DWORD)(group_info->Names[index2]), (DWORD)Ent1.first.c_str(), Ent1.first.length()+1);
		group_info->EntityList[index1] = Ent2.second;
		group_info->EntityList[index2] = Ent1.second;
	}
	else
	{
		Zeal::EqGame::print_chat("Error moving group members: %i to %i", index1, index2);
	}
}
void ui_group::sort()
{
	auto* group_info = Zeal::EqGame::GroupInfo;
	std::vector<std::pair<std::string, Zeal::EqStructures::Entity*>> group_members_sorted;
	for (int i = 0; i < 5; i++)
	{
		std::pair<std::string, Zeal::EqStructures::Entity*> ent_pair = std::make_pair(group_info->Names[i], group_info->EntityList[i]);
		group_members_sorted.push_back({ group_info->Names[i], group_info->EntityList[i]});
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
	mem::set((uint32_t)group_info->EntityList, 0, sizeof(group_info->EntityList));//erase the pointer array
	mem::set((uint32_t)group_info->Names, 0, sizeof(group_info->Names));//erase the pointer array
	for (int i = 0; i < group_members_sorted.size(); ++i) {
		mem::copy((DWORD)(group_info->Names[i]), (DWORD)group_members_sorted[i].first.c_str(), group_members_sorted[i].first.length());
		group_info->EntityList[i] = group_members_sorted[i].second;
	}
}
ui_group::~ui_group()
{

}
ui_group::ui_group(ZealService* zeal, UIManager* mgr)
{
	ui = mgr;
	//zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
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
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
