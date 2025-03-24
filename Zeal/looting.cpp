#include "looting.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include "string_util.h"
#include "EqPackets.h"
#include <fstream>
//void __fastcall finalize_loot(int uk, int lootwnd_ptr)
//{
//	Zeal::EqStructures::Entity* corpse =  Zeal::EqGame::get_active_corpse();
//	if (ZealService::get_instance()->gui->Options->hidecorpse_looted && corpse)
//		corpse->ActorInfo->IsInvisible = 1;
//	ZealService::get_instance()->hooks->hook_map["FinalizeLoot"]->original(finalize_loot)(uk, lootwnd_ptr);
//}

static constexpr int kMaxLinkCount = 10;
static constexpr int kMaxItemCount = 30;

void looting::link_all(const char* channel) const
{

	if (!Zeal::EqGame::Windows->Loot || !Zeal::EqGame::Windows->Loot->IsVisible)
	{
		Zeal::EqGame::print_chat("Looting window not visible");
		return;
	}

	std::vector<std::pair<int, std::string>> items;
	for (int i = 0; i < kMaxItemCount; i++)
	{
		const auto item = Zeal::EqGame::Windows->Loot->Item[i];
		if (item)
			items.push_back({ item->ID, item->Name });
	}

	if (items.empty())
		return;

	std::sort(items.begin(), items.end(),
		[](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
		return a.second < b.second; });

	const char* delimiter = setting_alt_delimiter.get() ? " | " : ", ";

	if (channel)  // Support outputting multiple lines if there's an explicit channel.
	{
		int link_count = 0;
		std::string link_text;
		for (int i = 0; i < items.size(); ++i)
		{
			if (link_count)
				link_text += delimiter;
			link_count++;
			// The AddItemTag performs an sprintf("%c%d%06d%s%c", 0x12, 0, item_id, item_name, 0x12).
			const char kMarker = 0x12;
			link_text += std::format("{0:c}0{1:06d}{2}{3:c}", kMarker, items[i].first, items[i].second, kMarker);
			if (link_count == kMaxLinkCount || i == items.size() - 1)
			{
				std::string select = channel;
				if (select.starts_with("rs"))
					Zeal::EqGame::send_raid_chat(link_text.c_str());
				else if (select.starts_with("gs"))
					Zeal::EqGame::do_gsay(link_text.c_str());
				else if (select.starts_with("gu"))
					Zeal::EqGame::do_guildsay(link_text.c_str());
				else if (select.starts_with("ooc"))
					Zeal::EqGame::do_ooc(link_text.c_str());
				else if (select.starts_with("auc"))
					Zeal::EqGame::do_auction(link_text.c_str());
				else
					Zeal::EqGame::do_say(false, link_text.c_str());
				link_text = "";
				link_count = 0;
			}
		}
		return;
	}

	// No specified channel: Paste the links into the active chat window.
	Zeal::EqUI::ChatWnd* wnd = Zeal::EqGame::Windows->ChatManager->GetActiveChatWindow();
	if (!wnd) {
		Zeal::EqGame::print_chat("No active chat window found");
		return;
	}

	Zeal::EqUI::EditWnd* input_wnd = (Zeal::EqUI::EditWnd*)wnd->edit;
	for (int i = 0; i < items.size(); ++i)
	{
		if (i > 0)
			input_wnd->ReplaceSelection(delimiter, false);
		if (input_wnd->item_link_count < kMaxLinkCount)
			input_wnd->AddItemTag(items[i].first, items[i].second.c_str());
		else  // Append the name as text (not a link). Skipped the ID so log parsing is the same.
			input_wnd->ReplaceSelection(items[i].second.c_str(), false);
	}
	input_wnd->SetFocus();
}

void looting::set_hide_looted(bool val)
{
	hide_looted = val;
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "HideLooted", hide_looted);
	ZealService::get_instance()->ui->options->UpdateOptions();
	if (hide_looted)
		Zeal::EqGame::print_chat("Corpses will be hidden after looting.");
	else
		Zeal::EqGame::print_chat("Corpses will no longer be hidden after looting.");
}

void looting::unhide_last_hidden()
{
	Zeal::EqStructures::Entity* corpse = nullptr;
	if (last_hidden_entity && last_hidden_spawnid) {
		if (Zeal::EqGame::get_entity_by_id(last_hidden_spawnid) == last_hidden_entity)
			corpse = last_hidden_entity;
	}

	if (corpse && corpse->Type == Zeal::EqEnums::NPCCorpse && corpse->ActorInfo &&
		corpse->ActorInfo->IsInvisible)
		corpse->ActorInfo->IsInvisible = 0;  // Note: Let client update the ViewActor_->Flags field.
	else
		Zeal::EqGame::print_chat("No valid last hidden corpse to unhide");
	set_last_hidden_corpse(nullptr);

}

void looting::set_last_hidden_corpse(Zeal::EqStructures::Entity* corpse)
{
	last_hidden_entity = corpse;
	last_hidden_spawnid = corpse ? corpse->SpawnId : 0;
}


// Hides a corpse if it is a valid NPC corpse. Returns true if a valid corpse.
static bool hide_npc_corpse(Zeal::EqStructures::Entity* corpse)
{
	if (corpse && corpse->Type == Zeal::EqEnums::NPCCorpse && 
		corpse->ActorInfo && corpse->ActorInfo->ViewActor_)
	{
		corpse->ActorInfo->IsInvisible = 1; // Flag set by /hidecorpse all.
		corpse->ActorInfo->ViewActor_->Flags |= 0x40000000;  // t3dSetActorInvisible(true).
		return true;
	}
	return false;
}

// Hides all valid NPC corpses in the entity list.
static void hide_npc_corpses()
{
	Zeal::EqGame::print_chat("Hiding all existing npc corpses.");
	Zeal::EqStructures::Entity* current_ent = Zeal::EqGame::get_entity_list();
	while (current_ent)
	{
		hide_npc_corpse(current_ent);
		current_ent = current_ent->Next;
	}
}

void __fastcall CLootWndDeactivate(int uk, int unused, int lootwnd_ptr)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->looting_hook->handle_hide_looted();
	zeal->hooks->hook_map["CLootWndDeactivate"]->original(CLootWndDeactivate)(uk, unused, lootwnd_ptr);
}

void __fastcall release_loot(int uk, int lootwnd_ptr)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->looting_hook->handle_hide_looted();
	zeal->hooks->hook_map["ReleaseLoot"]->original(release_loot)(uk, lootwnd_ptr);
}

void looting::handle_hide_looted()
{
	if (!hide_looted)
		return;

	Zeal::EqStructures::Entity* corpse = Zeal::EqGame::get_active_corpse();
	if (hide_npc_corpse(corpse))
		set_last_hidden_corpse(corpse);
}

looting::~looting()
{
}

void looting::init_ui()
{
	load_protected_items();
}

//struct formatted_msg
//{
//	UINT16 unk;
//	UINT16 string_id;
//	UINT16 type;
//	char message[0];
//};
//
//
//struct pkt_loot_item
//{
//	WORD corpse_id;
//	WORD player_id;
//	WORD slot_id;
//	UINT8 unknown[2];
//	UINT32 auto_loot;
//};
//pkt_loot_item li;
//li.player_id = Zeal::EqGame::get_self()->SpawnId;
//li.corpse_id = Zeal::EqGame::get_active_corpse()->SpawnId;
//li.auto_loot = 1;
//li.slot_id = Zeal::EqGame::Windows->Loot->ItemSlotIndex[i];
//Zeal::EqGame::send_message(0x40a0, (int*)&li, sizeof(li), 1);

void looting::looted_item()
{
	if (Zeal::EqGame::get_char_info()->CursorItem) 
	{
		loot_all = false;
		loot_next_item_time = 0;
		return;
	}
	if (loot_all && Zeal::EqGame::Windows && Zeal::EqGame::Windows->Loot && Zeal::EqGame::Windows->Loot->IsVisible)
	{
		std::string corpse_name = Zeal::EqGame::strip_name(Zeal::EqGame::get_active_corpse()->Name);
		std::string self_name = Zeal::EqGame::get_self()->Name;
		bool is_me = corpse_name == self_name; //my own corpse
		byte nodrop_confirm_bypass[2] = { 0x74, 0x22 };
		int item_count = 0;
		for (int i = 0; i < kMaxItemCount; i++)
			if (Zeal::EqGame::Windows->Loot->Item[i])
				item_count++;

		if (is_me && item_count == 1)
		{
			Zeal::EqGame::print_chat(USERCOLOR_LOOT, "Loot all but 1 item on your own corpse for safety, you may click the item to loot it yourself.");
			loot_all = false;
			return;
		}
		bool skipped_loot_last = false;  // Only skip the first item (make sure it can count down to 1).
		for (int i = 0; i < kMaxItemCount; i++)
		{
			bool loot = false;
			if (Zeal::EqGame::Windows->Loot->Item[i])
			{
				if (is_me && item_count>1)
					loot = true;
				else if (Zeal::EqGame::Windows->Loot->Item[i]->NoDrop != 0)
					loot = true;

				if (loot && !Zeal::EqGame::can_inventory_item(Zeal::EqGame::Windows->Loot->Item[i]))
				{
					Zeal::EqGame::print_chat("Cannot loot %s - not enough inventory space", Zeal::EqGame::Windows->Loot->Item[i]->Name);
					loot_all = false;
					loot = false;
				}

				if (loot && is_me && !skipped_loot_last && setting_loot_last_item.get() > 0 &&
					Zeal::EqGame::Windows->Loot->Item[i]->ID == setting_loot_last_item.get())
				{
					loot = false;
					skipped_loot_last = true;  // Prevent more skips.
				}

				if (loot)
				{
					Zeal::EqGame::Windows->Loot->RequestLootSlot(i, true);
					return;
				}
			}
		}
		loot_all = false;
	}
	else
		loot_all = false;
}

bool looting::parse_loot_last(const std::vector<std::string>& args)
{
	const char kMarker = 0x12;  // Link marker.
	int new_value = 0;
	if (args.size() >= 2 && args[1].size() >= 8 && args[1].front() == kMarker) {
		std::string link = args[1];  // Need to re-assemble possibly split link names.
		for (int i = 2; i < args.size(); ++i)
			link += " " + args[i];
		std::string item_id = link.substr(2, 6);
		std::string item_name = link.substr(8, link.length() - 1 - 8);
		if (link.back() == kMarker && Zeal::String::tryParse(item_id, &new_value) && new_value > 0)
			setting_loot_last_item.set(new_value);
		else
			Zeal::EqGame::print_chat("Error: Invalid link item id: %s", item_id.c_str());
	}
	else if (args.size() == 2 && Zeal::String::tryParse(args[1], &new_value)) {
		new_value = max(0, new_value);
		setting_loot_last_item.set(new_value);
	}
	else
		Zeal::EqGame::print_chat("Usage: /lootlast <item#> (0 to disable) or /lootlast <itemlink>");

	if (setting_loot_last_item.get() == 0)
		Zeal::EqGame::print_chat("Loot last item disabled");
	else
		Zeal::EqGame::print_chat("Loot last item set to %d", setting_loot_last_item.get());

	return true;
}

static void __fastcall DropHeldItemOnGround(void* ceverquest_this, int unused_edx, int print_message)
{
	auto zeal = ZealService::get_instance();
	if (zeal->looting_hook->is_cursor_protected(Zeal::EqGame::get_char_info()))
		return;  // Item or money were blocked from dropping.
	zeal->hooks->hook_map["DropHeldItemOnGround"]->original(DropHeldItemOnGround)(ceverquest_this,
		unused_edx, print_message);
}

// Not even sure if this is possible with client ui but just in case.
static void __fastcall DropHeldMoneyOnGround(void* ceverquest_this, int unused_edx, int print_message)
{
	auto zeal = ZealService::get_instance();
	if (zeal->looting_hook->is_cursor_protected(Zeal::EqGame::get_char_info()))
		return;  // Item or money were blocked from dropping.
	zeal->hooks->hook_map["DropHeldMoneyOnGround"]->original(DropHeldMoneyOnGround)(ceverquest_this,
		unused_edx, print_message);
}

static void __fastcall DestroyHeldItemOrMoney(Zeal::EqStructures::EQCHARINFO* char_info, int unused_edx)
{
	ZealService* zeal = ZealService::get_instance();
	if (zeal->looting_hook->is_cursor_protected(char_info))
		return;  // Item or money were blocked from destruction.
	zeal->hooks->hook_map["DestroyHeldItemOrMoney"]->original(DestroyHeldItemOrMoney)(char_info, unused_edx);
}

static void __fastcall ClickedTradeButton(Zeal::EqUI::TradeWnd* wnd, int unused_edx)
{
	ZealService* zeal = ZealService::get_instance();
	if (zeal->looting_hook->is_trade_protected(wnd))
		return;  // Trading was blocked (click will be ignored).
	zeal->hooks->hook_map["ClickedTradeButton"]->original(ClickedTradeButton)(wnd, unused_edx);
}

static void __fastcall RequestSellItem(Zeal::EqUI::MerchantWnd* this_wnd, int unused_edx, int param)
{
	ZealService* zeal = ZealService::get_instance();
	Zeal::EqStructures::EQITEMINFO* item_info =
		(this_wnd && this_wnd->ItemInfo && *this_wnd->ItemInfo) ? *this_wnd->ItemInfo : nullptr;
	if (item_info && zeal->looting_hook->is_item_protected_from_selling(item_info))
		return;  // Item was protected.
	zeal->hooks->hook_map["RequestSellItem"]->original(RequestSellItem)(this_wnd, unused_edx, param);
}

// Added for reference. This appears to be primarily used by old UI and is untested, so commented out.
//static void __fastcall SellItem(void* this_eq_main, int unused_edx, Zeal::EqStructures::EQITEMINFO** item_info, int param)
//{
//	ZealService* zeal = ZealService::get_instance();
//	if (item_info && *item_info && zeal->looting_hook->is_item_protected_from_selling(*item_info))
//		return;  // Item was protected.
//	zeal->hooks->hook_map["SellItem"]->original(SellItem)(this_eq_main, unused_edx, item_info, param);
//}

bool looting::is_item_protected_from_selling(const Zeal::EqStructures::EQITEMINFO* item_info) const
{
	if (!setting_protect_enable.get() || !item_info)
		return false;

	// First check the item versus the protected list.
	int item_id = item_info->ID;
	if (std::find_if(protected_items.begin(), protected_items.end(),
		[item_id](ProtectedItem item) {return item.id == item_id; }) != protected_items.end()) {
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT,
			"Zeal /protect blocked action: Use /protect off or disable protection with /protect item %d", item_id);
		return true;
	}

	// Then check if it is a non-empty container (client already does this, but extra layer).
	if (item_info->Type == 1) { // Container
		for (int i = 0; i < EQ_NUM_CONTAINER_SLOTS; ++i)
			if (item_info->Container.Item[i]) {
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal /protect blocked non-empty container: Use /protect off");
				return true;
			}
	}

	return false;
}

bool looting::is_cursor_protected(const Zeal::EqStructures::EQCHARINFO* char_info) const
{
	if (!setting_protect_enable.get() || !char_info)
		return false;

	// If no item on cursor, check for money.
	if (!char_info->CursorItem) {
		int value = char_info->CursorPlatinum * 1000 + char_info->CursorGold * 100
			+ char_info->CursorSilver * 10 + char_info->CursorCopper;
		bool block = value >= setting_protect_value.get() * 1000;
		if (block)
			Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal /protect blocked action: Use /protect off or change /protect value");
		return block;
	}

	// Next check the item versus the protected list.
	const auto item_info = char_info->CursorItem;
	int item_id = item_info->ID;
	if (std::find_if(protected_items.begin(), protected_items.end(),
		[item_id](ProtectedItem item) {return item.id == item_id;}) != protected_items.end()) {
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT,
			"Zeal /protect blocked action: Use /protect off or disable protection with /protect item %d", item_id);
		return true;
	}

	// Then check if it is a non-empty container (protect against Silchas maneuver).
	if (item_info->Type == 1) { // Container
		for (int i = 0; i < EQ_NUM_CONTAINER_SLOTS; ++i)
			if (item_info->Container.Item[i]) {
				Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal /protect blocked non-empty container: Use /protect off");
				return true;
			}
	}

	// And then check for value.
	int value = item_info->Cost;  // In copper.
	if (item_info->Type == 0 && item_info->Common.IsStackable && item_info->Common.StackCount)
		value *= item_info->Common.StackCount;
	if (value >= setting_protect_value.get() * 1000) {
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal /protect blocked action: Use /protect off or change /protect value");
		return true;
	}
	return false;
}

bool looting::is_trade_protected(Zeal::EqUI::TradeWnd* wnd) const
{
	if (!setting_protect_enable.get() || !wnd)
		return false;

	// Trades with other players are not protected.
	auto trade_target = *reinterpret_cast<Zeal::EqStructures::Entity**>(0x007f94c8);
	if (!trade_target || trade_target->Type == Zeal::EqEnums::Player)
		return false;

	// Trades to bankers are blocked by default (to prevent accidents).
	if (trade_target->Class == Zeal::EqEnums::ClassTypes::Banker) {
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT,
			"Zeal blocked trading with banker: use /protect off");
		return true;
	}

	// Then just check all items versus the protected list (also checks for non-empty bags).
	const int kNpcTradeSize = 4;
	for (int i = 0; i < kNpcTradeSize; ++i)  {
		const auto item_info = wnd->GiveItems[i];
		if (item_info && is_item_protected_from_selling(item_info))
			return true;  // The call above emits the blocked message.
	}
	return false;
}

static std::string get_protected_items_filename()
{
	if (!Zeal::EqGame::get_char_info())
		return "";
	return std::format("./{0}_protected.ini", Zeal::EqGame::get_char_info()->Name);
}

void looting::load_protected_items()
{
	std::string filename = get_protected_items_filename();
	if (filename.empty() || !std::filesystem::exists(filename))
		return;  // Game not initialized yet or no protected file.

	std::ifstream input_file(filename);
	if (!input_file.is_open()) {
		Zeal::EqGame::print_chat("Error opening protect file: %s", filename.c_str());
		return;
	}

	protected_items.clear();
	std::string line;
	while (std::getline(input_file, line))
	{
		auto fields = Zeal::String::split_text(line, ",");
		if (fields.size() == 2) {
			int item_id = std::stoi(fields.front());
			if (item_id > 0) {
				protected_items.push_back({ item_id, fields.back() });
				continue;  // Keep going.
			}
		}
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "Zeal protect error parsing line: %s", line.c_str());
		return;

	}
}

void looting::update_protected_item(int item_id, const std::string& name, bool add_only)
{
	auto it = std::find_if(protected_items.begin(), protected_items.end(),
		[item_id](ProtectedItem item) {return item.id == item_id; });
	if (it == protected_items.end()) {
		Zeal::EqGame::print_chat("protect: Adding item %d (%s) to the list", item_id, name.c_str());
		protected_items.push_back({ item_id, name });
	}
	else if (add_only) {
		Zeal::EqGame::print_chat("protect: Item %d (%s) already protected", item_id, name.c_str());
		return;
	}
	else {
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "protect: Removing item %d (%s) from the list",
			item_id, name.c_str());
		protected_items.erase(it);
	}
	std::sort(protected_items.begin(), protected_items.end(),
		[](ProtectedItem a, ProtectedItem b) {return a.id < b.id; });

	std::ofstream output_file(get_protected_items_filename());
	if (output_file.is_open())
		for (const auto& item : protected_items) 
			output_file << item.id << "," << item.name << "\n";
	else
		Zeal::EqGame::print_chat("Error: Failed to save protected item list");
}

bool looting::parse_protect(const std::vector<std::string>& args)
{
	const char kMarker = 0x12;  // Link marker.
	int new_value = 0;
	if (args.size() == 2 && args[1] == "on")
		setting_protect_enable.set(true);
	else if (args.size() == 2 && args[1] == "off")
		setting_protect_enable.set(false);
	else if (args.size() == 2 && args[1] == "list") {
		Zeal::EqGame::print_chat("Protected items: %d", protected_items.size());
		for (const auto& item : protected_items)
			Zeal::EqGame::print_chat("%d: %s", item.id, item.name.c_str());
	}
	else if (args.size() == 2 && args[1] == "cursor") {
		auto info = Zeal::EqGame::get_char_info();
		if (info && info->CursorItem)
			update_protected_item(info->CursorItem->ID, info->CursorItem->Name);
		else
			Zeal::EqGame::print_chat("No valid item in cursor to toggle protect");
	}
	else if (args.size() == 2 && args[1] == "worn") {
		auto info = Zeal::EqGame::get_char_info();
		if (info) {
			for (int i = 0; i < EQ_NUM_INVENTORY_SLOTS; i++)
				if (info->InventoryItem[i])
					update_protected_item(info->InventoryItem[i]->ID, info->InventoryItem[i]->Name, true);
		}
	}
	else if (args.size() == 3 && args[1] == "value" && Zeal::String::tryParse(args[2], &new_value)
		&& new_value >= 0)
		setting_protect_value.set(new_value);
	else if (args.size() == 3 && args[1] == "item" && Zeal::String::tryParse(args[2], &new_value)
		&& new_value > 0)
		update_protected_item(new_value, "IdOnly");
	else if (args.size() >= 2 && args[1].size() >= 8 && args[1].front() == kMarker) {
		std::string link = args[1];  // Need to re-assemble possibly split link names.
		for (int i = 2; i < args.size(); ++i)
			link += " " + args[i];
		std::string item_id = link.substr(2, 6);
		std::string item_name = link.substr(8, link.length() - 1 - 8);
		if (link.back() == kMarker && Zeal::String::tryParse(item_id, &new_value) && new_value > 0)
			update_protected_item(new_value, item_name);
		else
			Zeal::EqGame::print_chat("Error: Invalid link item id: %s", item_id.c_str());
	}
	else {
		Zeal::EqGame::print_chat("Usage: /protect on, off: enables or disables protection");
		Zeal::EqGame::print_chat("Usage: /protect list: prints the list of protected item_ids");
		Zeal::EqGame::print_chat("Usage: /protect value #: blocks destruction of items with a value > # pp");
		Zeal::EqGame::print_chat("Usage: /protect item #: toggles protection of item_id == #");
		Zeal::EqGame::print_chat("Usage: /protect <item_link>: toggles item protection from a link");
		Zeal::EqGame::print_chat("Usage: /protect cursor: toggles protection of item in cursor");
		Zeal::EqGame::print_chat("Usage: /protect worn: enables protection of all equipped items");
	}

	if (setting_protect_enable.get())
		Zeal::EqGame::print_chat("protect: ON, Value: %d pp, ItemCount: %d",
			setting_protect_value.get(), protected_items.size());
	else
		Zeal::EqGame::print_chat(USERCOLOR_SHOUT, "protect: OFF");

	return true;

}


looting::looting(ZealService* zeal)
{
	hide_looted = zeal->ini->getValue<bool>("Zeal", "HideLooted"); //just remembers the state
	zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);
	zeal->callbacks->AddEntity([this](Zeal::EqStructures::Entity* ent) {
		if (ent == last_hidden_entity)
			set_last_hidden_corpse(nullptr);
		}, callback_type::EntityDespawn);
	zeal->callbacks->AddGeneric([this]() {
		if (loot_next_item_time == 0)
			return;
		if (!Zeal::EqGame::Windows || !Zeal::EqGame::Windows->Loot || !Zeal::EqGame::Windows->Loot->IsVisible)
		{
			loot_next_item_time = 0;
			loot_all = false;
			return;
		}
		if (GetTickCount64() >= loot_next_item_time && loot_all)
		{
			looted_item();
			loot_next_item_time = 0;
		}
	}, callback_type::MainLoop);
	zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
			if (opcode == 0x4031)
			{
				loot_next_item_time = GetTickCount64() + 250;
			}
		return false; 
	});
	zeal->commands_hook->Add("/hidecorpse", { "/hc", "/hideco", "/hidec" }, "Adds looted argument to hidecorpse.",
		[this](std::vector<std::string>& args) {
			if (args.size() > 1 && Zeal::String::compare_insensitive(args[1], "looted"))
			{
				set_hide_looted(!hide_looted);
				return true;
			}
			else if (args.size() == 2 && args[1] == "showlast")
			{
				unhide_last_hidden();
				return true;
			}
			else if (args.size() == 2 && args[1] == "npc")
			{
				hide_npc_corpses();
				return true;
			}
			else if (args.size() == 1)
			{
				// Print out new usage instructions. Return false so default usage prints.
				Zeal::EqGame::print_chat("/hidecorpse zeal options: looted, showlast, npc");
			}
			return false;
		});
	zeal->commands_hook->Add("/lootall", {}, "Loot all items",
		[this](std::vector<std::string>& args) {
			if (!Zeal::EqGame::Windows->Loot || !Zeal::EqGame::Windows->Loot->IsVisible)
				Zeal::EqGame::print_chat("Looting window not visible");
			else
			{
				loot_all = true;
				looted_item();
			}
			return true;
		});
	zeal->commands_hook->Add("/linkall", {},
		"Link all items (opt param: say, gs, rs, gu, ooc, auc)",
		[this](std::vector<std::string>& args) {
			const char* channel = (args.size() == 2) ? args[1].c_str() : nullptr;
			link_all(channel);
			return true;
		});
	zeal->commands_hook->Add("/lootlast", {}, "Set an item to loot last when self looting.",
		[this](std::vector<std::string>& args) { return parse_loot_last(args); });

	zeal->hooks->Add("ReleaseLoot", 0x426576, release_loot, hook_type_detour);
	zeal->hooks->Add("CLootWndDeactivate", 0x426559, CLootWndDeactivate, hook_type_detour);

	zeal->commands_hook->Add("/protect", {}, "Controls secondary protection of item destruction",
		[this](const std::vector<std::string>& args) { return parse_protect(args); });
	zeal->hooks->Add("DestroyHeldItemOrMoney", 0x004d0d88, DestroyHeldItemOrMoney, hook_type_detour);
	zeal->hooks->Add("DropHeldItemOnGround", 0x00530d7e, DropHeldItemOnGround, hook_type_detour);
	zeal->hooks->Add("DropHeldMoneyOnGround", 0x005313b3, DropHeldMoneyOnGround, hook_type_detour);
	zeal->hooks->Add("ClickedTradeButton", 0x0043964e, ClickedTradeButton, hook_type_detour);  // CTradeWnd::ClickedTradeButton
	zeal->hooks->Add("RequestSellItem", 0x00427c83, RequestSellItem, hook_type_detour);  // CMerchantWnd::RequestSellItem
	// Old UI path: zeal->hooks->Add("SellItem", 0x0047e0af, SellItem, hook_type_detour);  // EQ_Main::SellItem 
}
