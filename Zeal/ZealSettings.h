#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <string>
#include <functional>
#include <Windows.h>
#include <memory>
#include "callbacks.h"
//#include "Zeal.h"
class IO_ini;
class ZealService;
template <typename T>
class ZealSetting
{
public:
	void set(T val, bool store = true) { 
		if (store && section.length() && key.length())
		{
			std::string ini_name = ".\\eqclient.ini";
			if (per_character)
				ini_name = ZealService::get_instance()->ui->GetUIIni();
			if (ini_name.length())
			{
				IO_ini ini(ini_name);
				ini.setValue<T>(section, key, val);
			}

		}
		value = val; 
		if (set_callback)
			set_callback(value);
	}
	void toggle(bool store = true)
	{
		set(!value, store);
	}
	T get() { return value; }
	/*
	*   save_per_character if true saves in player names ini rather than eqclient
	*	read and write ini as well as a callback with new value
	*/
	ZealSetting(T default_value, const std::string& ini_section, const std::string& ini_key, bool save_per_character, std::function<void(T& value)> onset_callback)
	{
		set_callback = onset_callback; 
		value = default_value; 
		section = ini_section;
		key = ini_key;
		per_character = save_per_character;
		if (save_per_character)
			ZealService::get_instance()->callbacks->AddGeneric([this]() { init(); }, callback_type::InitUI);
		else
			init();
	}
	//read and write ini
	ZealSetting(T default_value, const std::string& ini_section, const std::string& ini_key, bool save_per_character)
	{
		value = default_value; 
		section = ini_section;
		key = ini_key;
		per_character = save_per_character;
		if (save_per_character)
			ZealService::get_instance()->callbacks->AddGeneric([this]() { init(); }, callback_type::InitUI);
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
	bool per_character;
	void init()
	{
		if (section.length() && key.length())
		{
			std::string ini_name = ".\\eqclient.ini";
			if (per_character)
				ini_name = ZealService::get_instance()->ui->GetUIIni();
			if (ini_name.length())
			{
				IO_ini ini(ini_name);
				if (ini.exists(section, key))
					value = ini.getValue<T>(section, key);
			}
		}
	}
};