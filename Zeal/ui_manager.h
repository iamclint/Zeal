#pragma once
#include "ui_options.h"
#include "ui_bank.h"
#include "ui_loot.h"
#include "ui_guild.h"
#include "ui_raid.h"
#include "ui_hotbutton.h"
#include "ui_group.h"
#include "ui_inputdialog.h"
#include "ui_buff.h"
#include "ui_zoneselect.h"
#include "ui_inspect.h"
#include "hook_wrapper.h"
#include "memory.h"
#include "EqUI.h"
class UIManager
{
public:
	static constexpr const char* ui_path = "uifiles\\zeal\\";
	std::string GetUIIni();

	Zeal::EqUI::SliderWnd* GetSlider(std::string name);
	Zeal::EqUI::BasicWnd* GetCheckbox(std::string name);
	Zeal::EqUI::BasicWnd* GetButton(std::string name);
	Zeal::EqUI::BasicWnd* GetCombo(std::string name);
	std::function<void(Zeal::EqUI::SliderWnd*, int)> GetSliderCallback(Zeal::EqUI::SliderWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*, int)> GetComboCallback(Zeal::EqUI::BasicWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*)> GetButtonCallback(Zeal::EqUI::BasicWnd* wnd);
	std::function<void(Zeal::EqUI::BasicWnd*)> GetCheckboxCallback(Zeal::EqUI::BasicWnd* wnd);

	Zeal::EqUI::BasicWnd* clicked_button = nullptr;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::unordered_map<std::string, Zeal::EqUI::BasicWnd*>> WindowChildren;
	void SetLabelValue(std::string name, const char* format, ...);
	void SetSliderValue(std::string name, int value);
	void SetSliderValue(std::string name, float value);
	void SetComboValue(std::string name, int value);
	void SetChecked(std::string name, bool checked);
	Zeal::EqUI::BasicWnd* AddButtonCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback, bool log_errors=true);
	Zeal::EqUI::BasicWnd* AddCheckboxCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback, bool log_errors = true);
	Zeal::EqUI::BasicWnd* AddSliderCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback, int max_val=100, bool log_errors = true);
	Zeal::EqUI::BasicWnd* AddComboCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback, bool log_errors = true);
	void AddLabel(Zeal::EqUI::BasicWnd* wnd, std::string name, bool log_errors = true);
	void AddListItems(Zeal::EqUI::ComboWnd* wnd, const std::vector<std::string> data);
	void AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::vector<std::string>> data);
	void AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::string> data);
	Zeal::EqUI::EQWND* CreateSidlScreenWnd(const std::string& name);
	void DestroySidlScreenWnd(Zeal::EqUI::EQWND* sidl_wnd);
	bool WriteTemporaryUI(const std::string& file_path, std::string ui_path);
	void RemoveTemporaryUI(const std::string& file_path);
	void AddXmlInclude(const std::string& name);
	UIManager(class ZealService* zeal);
	bool AlreadyLoadedXml(std::string name);
	std::shared_ptr<ui_options> options = nullptr;
	std::shared_ptr<ui_bank> bank = nullptr;
	std::shared_ptr<ui_loot> loot = nullptr;
	std::shared_ptr<ui_guild> guild = nullptr;
	std::shared_ptr<ui_raid> raid = nullptr;
	std::shared_ptr<ui_hotbutton> hotbutton = nullptr;
	std::shared_ptr<ui_group> group = nullptr;
	std::shared_ptr<ui_inputdialog> inputDialog = nullptr;
	std::shared_ptr<ui_buff> buffs = nullptr;
	std::shared_ptr<ui_zoneselect> zoneselect = nullptr;
	std::shared_ptr<ui_inspect> inspect = nullptr;
	std::vector<std::string> included_files;
	void CreateTmpXML();

private:
	bool handle_uilock(const std::vector<std::string>& args);

	std::vector<std::string> XMLIncludes;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> checkbox_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> button_names;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> checkbox_callbacks;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*)>> button_callbacks;
	std::unordered_map<Zeal::EqUI::SliderWnd*, std::function<void(Zeal::EqUI::SliderWnd*, int)>> slider_callbacks;
	std::unordered_map<Zeal::EqUI::BasicWnd*, std::function<void(Zeal::EqUI::BasicWnd*, int)>> combo_callbacks;
	std::unordered_map<std::string, Zeal::EqUI::SliderWnd*> slider_names;
	
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> combo_names;
	std::unordered_map<std::string, Zeal::EqUI::BasicWnd*> label_names;
	
	void CleanUI();
};

