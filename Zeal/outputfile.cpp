#include "outputfile.h"
#include "Zeal.h"
#include "string_util.h"
#include <fstream>

using Zeal::EqEnums::EquipSlot::EquipSlot;

static std::string IDToEquipSlot(int equipSlot)
{
  switch (equipSlot) {
    case EquipSlot::LeftEar:
    case EquipSlot::RightEar:    return "Ear";
    case EquipSlot::Head:        return "Head";
    case EquipSlot::Face:        return "Face";
    case EquipSlot::Neck:        return "Neck";
    case EquipSlot::Shoulder:    return "Shoulders";
    case EquipSlot::Arms:        return "Arms";
    case EquipSlot::Back:        return "Back";
    case EquipSlot::LeftWrist:
    case EquipSlot::RightWrist:  return "Wrist";
    case EquipSlot::Range:       return "Range";
    case EquipSlot::Hands:       return "Hands";
    case EquipSlot::Primary:     return "Primary";
    case EquipSlot::Secondary:   return "Secondary";
    case EquipSlot::LeftFinger:
    case EquipSlot::RightFinger: return "Fingers";
    case EquipSlot::Chest:       return "Chest";
    case EquipSlot::Legs:        return "Legs";
    case EquipSlot::Feet:        return "Feet";
    case EquipSlot::Waist:       return "Waist";
    case EquipSlot::Ammo:        return "Ammo";
    default:{}break;
  }
  return "Unknown";
}

static bool ItemIsContainer(Zeal::EqStructures::EQITEMINFO* item)
{
  return (item->Type == 1 && item->Container.Capacity > 0);
}

static bool ItemIsStackable(Zeal::EqStructures::EQITEMINFO* item)
{
  return ((item->Common.IsStackable) && (item->Common.SpellId == 0));
}

void OutputFile::export_inventory(const std::vector<std::string>& args)
{
  Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();

  std::ostringstream oss;
  std::string t = "\t"; // output spacer
  oss << "Location" << t << "Name" << t << "ID" << t << "Count" << t << "Slots" << std::endl;

  // Processing Equipment
  for (size_t i = 0; i < EQ_NUM_INVENTORY_SLOTS; ++i) {
    Zeal::EqStructures::EQITEMINFO* item = self->CharInfo->InventoryItem[i];
    // EQITEMINFO->EquipSlot value only updates when a load happens. Don't use it for this.
    if (item) {
      oss << IDToEquipSlot(i) << t << item->Name << t << item->ID << t << 1 << t << 0 << std::endl;
    }
    else {
      oss << IDToEquipSlot(i) << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
    }
  }

  { // Processing Inventory Slots
    for (size_t i = 0; i < EQ_NUM_INVENTORY_PACK_SLOTS; ++i) {
      Zeal::EqStructures::EQITEMINFO* item = self->CharInfo->InventoryPackItem[i];
      if (item) {
        if (ItemIsContainer(item)) {
          int capacity = static_cast<int>(item->Container.Capacity);
          oss << "General" << i + 1 << t << item->Name << t << item->ID << t << 1 << t << capacity << std::endl;
          for (int j = 0; j < capacity; ++j) {
            Zeal::EqStructures::EQITEMINFO* bag_item = item->Container.Item[j];
            if (bag_item) {
              int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
              oss << "General" << i + 1 << "-Slot" << j + 1 << t << bag_item->Name << t << bag_item->ID << t << stack_count << t << 0 << std::endl;
            }
            else {
              oss << "General" << i + 1 << "-Slot" << j + 1 << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
            }
          }
        }
        else {
          int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
          oss << "General" << i + 1 << t << item->Name << t << item->ID << t << stack_count << t << 0 << std::endl;
        }
      }
      else {
        oss << "General" << i + 1 << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
      }
    }
    ULONGLONG coin = 0;
    coin += static_cast<ULONGLONG>(self->CharInfo->Platinum) * 1000;
    coin += static_cast<ULONGLONG>(self->CharInfo->Gold) * 100;
    coin += static_cast<ULONGLONG>(self->CharInfo->Silver) * 10;
    coin += self->CharInfo->Copper;
    oss << "General-Coin" << t << "Currency" << t << 0 << t << coin << t << 0 << std::endl;
  }

  { // Process Cursor Item
    Zeal::EqStructures::EQITEMINFO* item = self->CharInfo->CursorItem;
    if (item) {
      if (ItemIsContainer(item)) {
        int capacity = static_cast<int>(item->Container.Capacity);
        oss << "Held" << t << item->Name << t << item->ID << t << 1 << t << capacity << std::endl;
        for (int i = 0; i < capacity; ++i) {
          Zeal::EqStructures::EQITEMINFO* bag_item = item->Container.Item[i];
          if (bag_item) {
            int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
            oss << "Held" << "-Slot" << i + 1 << t << bag_item->Name << t << bag_item->ID << t << stack_count << t << 0 << std::endl;
          }
          else {
            oss << "Held" << "-Slot" << i + 1 << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
          }
        }
      }
      else {
        int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
        oss << "Held" << t << item->Name << t << item->ID << t << stack_count << t << 0 << std::endl;
      }
    }
    else {
      ULONGLONG coin = 0;
      coin += static_cast<ULONGLONG>(self->CharInfo->CursorPlatinum) * 1000;
      coin += static_cast<ULONGLONG>(self->CharInfo->CursorGold) * 100;
      coin += static_cast<ULONGLONG>(self->CharInfo->CursorSilver) * 10;
      coin += self->CharInfo->CursorCopper;

      if (coin != 0)
        oss << "Held" << t << "Currency" << t << 0 << t << coin << t << 0 << std::endl;
      else
        oss << "Held" << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
    }
  }

  { // Process Bank Items
    int num_bank_slots = Zeal::EqGame::get_num_personal_bank_slots();
    for (int i = 0; i < Zeal::EqGame::get_num_total_bank_slots(); ++i) {
      Zeal::EqStructures::EQITEMINFO* item = self->CharInfo->InventoryBankItem[i];
      const char* label = (i < num_bank_slots) ? "Bank" : "SharedBank";
      int slot = (i < num_bank_slots) ? (i + 1) : (i + 1 - num_bank_slots);
      if (item) {
        if (ItemIsContainer(item)) {
          int capacity = static_cast<int>(item->Container.Capacity);
          oss << label << slot << t << item->Name << t << item->ID << t << 1 << t << capacity << std::endl;
          for (int j = 0; j < capacity; ++j) {
            Zeal::EqStructures::EQITEMINFO* bag_item = item->Container.Item[j];
            if (bag_item) {
              int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
              oss << label << slot << "-Slot" << j + 1 << t << bag_item->Name << t << bag_item->ID << t << stack_count << t << 0 << std::endl;
            }
            else {
              oss << label << slot << "-Slot" << j + 1 << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
            }
          }
        }
        else {
          int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
          oss << label << slot << t << item->Name << t << item->ID << t << stack_count << t << 0 << std::endl;
        }
      }
      else {
        oss << label << slot << t << "Empty" << t << 0 << t << 0 << t << 0 << std::endl;
      }
    }
    ULONGLONG coin = 0;
    coin += static_cast<ULONGLONG>(self->CharInfo->BankPlatinum) * 1000;
    coin += static_cast<ULONGLONG>(self->CharInfo->BankGold) * 100;
    coin += static_cast<ULONGLONG>(self->CharInfo->BankSilver) * 10;
    coin += self->CharInfo->BankCopper;
    oss << "Bank-Coin" << t << "Currency" << t << 0 << t << coin << t << 0 << std::endl;
  }

  std::string optional_name = "";  // Blank optional_name results in "<char_name>-Inventory.txt".
  if (args.size() > 2) {
    optional_name = args[2];
  }
  write_to_file(oss.str(), "Inventory", optional_name);
}

void OutputFile::export_spellbook(const std::vector<std::string>& args)
{
  Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();

  std::stringstream oss;
  oss << "Index\tSpellId\tLevel\tName" << std::endl;
  for (size_t i = 0; i < EQ_NUM_SPELL_BOOK_SPELLS; ++i) {
    WORD SpellId = self->CharInfo->SpellBook[i];
    int Level = Zeal::EqGame::get_spell_level(SpellId);
    const char* Name = Zeal::EqGame::get_spell_name(SpellId);
    Name = Name ? Name : "Unknown";
    if (SpellId && SpellId != USHRT_MAX) {
      oss << i << "\t" << SpellId << "\t" << Level << "\t" << Name << std::endl;
    }
  }

  std::string optional_name = "";
  if (args.size() > 2) {
    optional_name = args[2];
  }
  write_to_file(oss.str(), "Spellbook", optional_name);
}

void OutputFile::export_raidlist(std::vector<std::string>& args)
{
    std::vector<Zeal::EqStructures::RaidMember*> raid_member_list = Zeal::EqGame::get_raid_list();
    if (raid_member_list.size() > 0) {
        std::stringstream oss;

        std::string points = "1";
        if (args.size() > 2) { points = args[2]; }
        std::string timestamp = Zeal::EqGame::generateTimestamp();

        oss << "Player\tLevel\tClass\tTimestamp\tPoints" << std::endl;

        for (auto& raid_member : raid_member_list)
        {
            oss << raid_member->Name << "\t" << raid_member->PlayerLevel << "\t" << raid_member->Class << "\t" << timestamp << "\t" << points << std::endl;
        }
        std::string fname = "RaidTick-" + timestamp;
        write_to_file(oss.str(), "", fname);
        Zeal::EqGame::print_chat("Raid tick saved to: %s", fname);
    }
    else {
        Zeal::EqGame::print_chat("Currently not in a raid.");
    }
}

void OutputFile::write_to_file(std::string data, std::string file_arg, std::string optional_name)
{
  std::string filename = optional_name;
  if (filename.empty()) {
    filename = Zeal::EqGame::get_self()->CharInfo->Name;
    filename += "-" + file_arg;
  }
  filename += ".txt";

  std::ofstream file;
  file.open(filename);
  file << data;
  file.close();
}

OutputFile::OutputFile(ZealService* zeal)
{
  zeal->commands_hook->Add("/outputfile", { "/output", "/out" }, "Outputs your inventory,spellbook, or raidlist to file.",
    [this](std::vector<std::string>& args) {
      if (args.size() == 1 || args.size() > 3)
      {
        Zeal::EqGame::print_chat("usage: /outputfile [inventory | spellbook | raidlist] [optional filename]");
        return true;
      }
      if (args.size() > 1) {
        if (Zeal::String::compare_insensitive(args[1], "inventory"))
        {
          Zeal::EqGame::print_chat("Outputting inventory...");
          export_inventory(args);
        }
        else if (Zeal::String::compare_insensitive(args[1], "spellbook"))
        {
          Zeal::EqGame::print_chat("Outputting spellbook...");
          export_spellbook(args);
        }
        else if (Zeal::String::compare_insensitive(args[1], "raidlist"))
          export_raidlist(args);
      }
      return true;
    }
  );
}
