#pragma once
#include "hook_wrapper.h"
#include "memory.h"

class OutputFile
{
public:
  OutputFile(class ZealService* zeal);
  ~OutputFile() {};
private:
  void export_inventory(std::vector<std::string>& args);
  void export_spellbook(std::vector<std::string>& args);
  void write_to_file(std::string data, std::string filename, std::string optional_name);
};
