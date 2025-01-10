#pragma once
#include <string>
#include <functional>
#include <Windows.h>
#include "callbacks.h"
#include "io_ini.h"
#include "EqFunctions.h"

class ZealService;
template <typename T>
class ZealSetting
{
public:
	static constexpr char kIniFilename[] = ".\\zeal.ini";

	void set(T val, bool store = true) { 
		if (store && section.length() && key.length())
		{
			IO_ini ini(kIniFilename);
			ini.setValue<T>(get_section_name(), key, val);
		}
		value = val; 
		if (set_callback)
			set_callback(value);
	}
	void toggle(bool store = true)
	{
		set(!value, store);
	}
	T get() const { return value; }
	std::string get_section_name() const {
		if (!per_character)
			return section;

		Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
		std::string suffix = (c) ? std::string(c->Name) : "Unknown";
		return section + "_" + suffix;
	}
	/*
	*   save_per_character if true saves in player names ini rather than eqclient
	*	read and write ini as well as a callback with new value
	*/
	ZealSetting(T default_value, const std::string& ini_section, const std::string& ini_key, bool save_per_character, std::function<void(T& value)> onset_callback, bool callback_on_init=false)
	{
		set_callback = onset_callback; 
		value = default_value; 
		section = ini_section;
		key = ini_key;
		per_character = save_per_character;
		if (save_per_character)
		{
			needs_callback = callback_on_init;
			ZealService::get_instance()->callbacks->AddGeneric([this]() { init(); if (needs_callback && set_callback) set_callback(value); needs_callback = false;  }, callback_type::InitUI);
		}
		else
		{
			init();
			if (callback_on_init && set_callback)
				set_callback(value);
		}
		
			
	}
	//read and write ini
	ZealSetting(T default_value, const std::string& ini_section, const std::string& ini_key, bool save_per_character)
	{
		value = default_value; 
		section = ini_section;
		key = ini_key;
		per_character = save_per_character;
		if (save_per_character)
		{
			ZealService::get_instance()->callbacks->AddGeneric([this]() { init(); }, callback_type::InitUI);
		}
		else
			init();
	}
	//no ini settings;
	ZealSetting(T default_value)
	{
		value = default_value; 
		section = "";
		key = "";
	}
	ZealSetting()
	{

	}
private:
	std::function<void(T& value)> set_callback;
	T value;
	std::string section;
	std::string key;
	bool per_character = false;
	bool needs_callback = false;
	void init()
	{
		if (section.length() && key.length())
		{
			IO_ini ini(kIniFilename);
			std::string section_name = get_section_name();
			if (ini.exists(section_name, key))
				value = ini.getValue<T>(section_name, key);
		}
	}
};