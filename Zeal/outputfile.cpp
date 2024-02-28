#include "outputfile.h"
#include "Zeal.h"
#include <algorithm>
#include <fstream>
#include "StringUtil.h"
enum EquipSlot {
  LeftEar,
  Head,
  Face,
  RightEar,
  Neck,
  Shoulder,
  Arms,
  Back,
  LeftWrist,
  RightWrist,
  Range,
  Hands,
  Primary,
  Secondary,
  LeftFinger,
  RightFinger,
  Chest,
  Legs,
  Feet,
  Waist,
  Ammo
};

static std::string IDToEquipSlot(int equipSlot) {
  switch (equipSlot) {
    case LeftEar:
    case RightEar:    return "Ear";
    case Head:        return "Head";
    case Face:        return "Face";
    case Neck:        return "Neck";
    case Shoulder:    return "Shoulders";
    case Arms:        return "Arms";
    case Back:        return "Back";
    case LeftWrist:
    case RightWrist:  return "Wrist";
    case Range:       return "Range";
    case Hands:       return "Hands";
    case Primary:     return "Primary";
    case Secondary:   return "Secondary";
    case LeftFinger:
    case RightFinger: return "Fingers";
    case Chest:       return "Chest";
    case Legs:        return "Legs";
    case Feet:        return "Feet";
    case Waist:       return "Waist";
    case Ammo:        return "Ammo";
    default:{}break;
  }
  return "Unknown";
}

static bool ItemIsContainer(Zeal::EqStructures::EQITEMINFO* item) {
  return item->OpenType == 0x1;
}

static bool ItemIsStackable(Zeal::EqStructures::EQITEMINFO* item) {
  return ((item->Common.IsStackable) && (item->Common.SpellId == 0));
}

void OutputFile::export_inventory(std::vector<std::string>& args) {
  std::string t = "\t"; // output spacer

  std::stringstream ss;
  ss << "Location" << t << "Name" << t << "ID" << t << "Count" << t << "Slots" << std::endl;

  // Processing Equipment
  for (size_t i = 0; i < EQ_NUM_INVENTORY_SLOTS; ++i) {
    auto item = Zeal::EqGame::get_self()->CharInfo->InventoryItem[i];
    // EQITEMINFO->EquipSlot value only updates when a load happens. Don't use it for this.
    if (item != nullptr) {
      ss << IDToEquipSlot(i) << t << item->Name << t << item->Id << t << 1 << t << 0 << std::endl;
    }
    else {
      ss << IDToEquipSlot(i) << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
    }
  }

  // Processing Inventory Slots
  for (size_t i = 0; i < EQ_NUM_INVENTORY_PACK_SLOTS; ++i) {
    auto item = Zeal::EqGame::get_self()->CharInfo->InventoryPackItem[i];
    if (item != nullptr) {
      if (ItemIsContainer(item)) {
        int capacity = static_cast<int>(item->Container.Capacity);
        ss << "General" << i + 1 << t << item->Name << t << item->Id << t << 1 << t << capacity << std::endl;
        for (int j = 0; j < capacity; ++j) {
          auto bag_item = item->Container.Item[j];
          if (bag_item != nullptr) {
            int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
            ss << "General" << i + 1 << "-Slot" << j + 1 << t << bag_item->Name << t << bag_item->Id << t << stack_count << t << 0 << std::endl;
          }
          else {
            ss << "General" << i + 1 << "-Slot" << j + 1 << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
          }
        }
      }
      else {
        int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
        ss << "General" << i + 1 << t << item->Name << t << item->Id << t << stack_count << t << 0 << std::endl;
      }
    }
    else {
      ss << "General" << i + 1 << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
    }
  }

  // Process Cursor Item
  {
    auto item = Zeal::EqGame::get_self()->CharInfo->CursorItem;
    if (item != nullptr) {
      if (ItemIsContainer(item)) {
        int capacity = static_cast<int>(item->Container.Capacity);
        ss << "Held" << t << item->Name << t << item->Id << t << 1 << t << capacity << std::endl;
        for (int i = 0; i < capacity; ++i) {
          auto bag_item = item->Container.Item[i];
          if (bag_item != nullptr) {
            int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
            ss << "Held" << "-Slot" << i + 1 << t << bag_item->Name << t << bag_item->Id << t << stack_count << t << 0 << std::endl;
          }
          else {
            ss << "Held" << "-Slot" << i + 1 << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
          }
        }
      }
      else {
        int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
        ss << "Held" << t << item->Name << t << item->Id << t << stack_count << t << 0 << std::endl;
      }
    }
    else {
      ss << "Held" << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
    }
  }

  // Process Bank Items
  for (size_t i = 0; i < EQ_NUM_INVENTORY_BANK_SLOTS; ++i) {
    auto item = Zeal::EqGame::get_self()->CharInfo->InventoryBankItem[i];
    if (item != nullptr) {
      if (ItemIsContainer(item)) {
        int capacity = static_cast<int>(item->Container.Capacity);
        ss << "Bank" << i + 1 << t << item->Name << t << item->Id << t << 1 << t << capacity << std::endl;
        for (int j = 0; j < capacity; ++j) {
          auto bag_item = item->Container.Item[j];
          if (bag_item != nullptr) {
            int stack_count = ItemIsStackable(bag_item) ? static_cast<int>(bag_item->Common.StackCount) : 1;
            ss << "Bank" << i + 1 << "-Slot" << j + 1 << t << bag_item->Name << t << bag_item->Id << t << stack_count << t << 0 << std::endl;
          }
          else {
            ss << "Bank" << i + 1 << "-Slot" << j + 1 << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
          }
        }
      }
      else {
        int stack_count = ItemIsStackable(item) ? static_cast<int>(item->Common.StackCount) : 1;
        ss << "Bank" << i + 1 << t << item->Name << t << item->Id << t << stack_count << t << 0 << std::endl;
      }
    }
    else {
      ss << "Bank" << i + 1 << t << "Empty" << t << "0" << t << "0" << t << "0" << std::endl;
    }
  }

  //Zeal::EqGame::print_chat(ss.str()); // debug purposes

  std::string optional_name = "";
  if (args.size() > 2) {
    optional_name = args[2];
  }
  write_to_file(ss.str(), "Inventory", optional_name);
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
  zeal->commands_hook->add("/outputfile", {},
    [this](std::vector<std::string>& args) {
      if (args.size() == 1 || args.size() > 3)
      {
        Zeal::EqGame::print_chat("usage: /outputfile [inventory] [optional filename]");
        return true;
      }
      if (args.size() > 1 && StringUtil::caseInsensitive(args[1], "inventory"))
      {
        export_inventory(args);
        return true;
      }
      return true;
    });
}

OutputFile::~OutputFile()
{
}
