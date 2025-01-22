#pragma once
#include "hook_wrapper.h"
#include "memory.h"
#include "ZealSettings.h"

class OutputFile
{
public:
  OutputFile(class ZealService* zeal);
  ~OutputFile() {};
  void export_inventory(const std::vector<std::string>& args = {});
  void export_spellbook(const std::vector<std::string>& args = {});

  ZealSetting<bool> setting_export_on_camp = { false, "Zeal", "ExportOnCamp", false };

private:
  void export_raidlist(std::vector<std::string>& args);
  void write_to_file(std::string data, std::string filename, std::string optional_name);
};
