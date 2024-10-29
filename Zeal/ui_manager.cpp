#include "ui_manager.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
Zeal::EqUI::EQWND* ui_manager::CreateSidlScreenWnd(const std::string& name)
{
	Zeal::EqUI::EQWND* wnd = (Zeal::EqUI::EQWND*)HeapAlloc(*(HANDLE*)0x80B420, 0, sizeof(Zeal::EqUI::EQWND));
	mem::set((int)wnd, 0, sizeof(Zeal::EqUI::EQWND));
	reinterpret_cast<int* (__thiscall*)(Zeal::EqUI::BasicWnd*, Zeal::EqUI::BasicWnd*, Zeal::EqUI::CXSTR name)>(0x56e2b0)(wnd, 0, Zeal::EqUI::CXSTR(name));
	//reinterpret_cast<int* (__thiscall*)(Zeal::EqUI::EQWND*, Zeal::EqUI::EQWND*, Zeal::EqUI::CXSTR name, int, int)>(0x56e1e0)(wnd, 0, Zeal::EqUI::CXSTR(name), -1, 0);
	wnd->SetupCustomVTable();
	wnd->CreateChildren();
	return wnd;
}
static int __fastcall ButtonClick_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	int rval = ZealService::get_instance()->hooks->hook_map["ButtonClick"]->original(ButtonClick_hook)(pWnd, unused, pt, flag);
	auto cb = ui->GetButtonCallback(pWnd);
	if (cb)
	{
		ui->clicked_button = pWnd;
		cb(pWnd);
	}
	return rval;
}
static int __fastcall CheckboxClick_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, Zeal::EqUI::CXPoint pt, unsigned int flag)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	int rval = ZealService::get_instance()->hooks->hook_map["CheckboxClick"]->original(CheckboxClick_hook)(pWnd, unused, pt, flag);

	auto cb = ui->GetCheckboxCallback(pWnd);
	if (cb)
		cb(pWnd);

	return rval;
}
static void __fastcall SetSliderValue_hook(Zeal::EqUI::SliderWnd* pWnd, int unused, int value)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(pWnd, unused, value);

	if (value < 0)
		value = 0;
	if (value > pWnd->max_val)
		value = pWnd->max_val;

	auto cb = ui->GetSliderCallback(pWnd);
	if (cb)
		cb(pWnd, value);
}
static void __fastcall SetComboValue_hook(Zeal::EqUI::BasicWnd* pWnd, int unused, int value)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(pWnd, unused, value);

	auto cb = ui->GetComboCallback(pWnd);
	auto cb_parent = ui->GetComboCallback(pWnd->ParentWnd);
	if (cb)
		cb(pWnd, value);
	else if (cb_parent)
		cb_parent(pWnd->ParentWnd, value);
}

Zeal::EqUI::BasicWnd* ui_manager::AddButtonCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = wnd->GetChildItem(name);
		if (btn)
		{
			button_callbacks[btn] = callback;
			button_names[name] = btn;
			return btn;
		}
	}
	return nullptr;
}
Zeal::EqUI::BasicWnd* ui_manager::AddCheckboxCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = wnd->GetChildItem(name);
		if (btn)
		{
			checkbox_callbacks[btn] = callback;
			checkbox_names[name] = btn;
			return btn;
		}
	}
	return nullptr;
}

Zeal::EqUI::BasicWnd* ui_manager::AddSliderCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::SliderWnd*, int)> callback, int max_val)
{
	if (wnd)
	{
		Zeal::EqUI::SliderWnd* btn = (Zeal::EqUI::SliderWnd*)wnd->GetChildItem(name);
		if (btn)
		{
			slider_callbacks[btn] = callback;
			slider_names[name] = btn;
			btn->max_val = max_val;
			return btn;
		}
	}
	return nullptr;
}

Zeal::EqUI::BasicWnd* ui_manager::AddComboCallback(Zeal::EqUI::BasicWnd* wnd, std::string name, std::function<void(Zeal::EqUI::BasicWnd*, int)> callback)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = (Zeal::EqUI::BasicWnd*)wnd->GetChildItem(name);
		if (btn)
		{
			combo_callbacks[btn] = callback;
			combo_names[name] = btn;
			return btn;
		}
	}
	return nullptr;
}

void ui_manager::AddLabel(Zeal::EqUI::BasicWnd* wnd, std::string name)
{
	if (wnd)
	{
		Zeal::EqUI::BasicWnd* btn = wnd->GetChildItem(name);
		if (btn)
		{
			label_names[name] = btn;
		}
	}
}

void ui_manager::SetSliderValue(std::string name, int value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, value);
	}
}
void ui_manager::SetSliderValue(std::string name, float value)
{
	if (slider_names.count(name) > 0)
	{
		ZealService::get_instance()->hooks->hook_map["SetSliderValue"]->original(SetSliderValue_hook)(slider_names[name], 0, static_cast<int>(value));
	}
}

void ui_manager::AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::string> data)
{

	for (int row = 0; auto & current_row : data)
	{
		int x = wnd->AddString("");
		wnd->SetItemText(current_row, row, 0);
		wnd->SetItemData(row);
		row++;
	}
}
void ui_manager::AddListItems(Zeal::EqUI::ComboWnd* wnd, const std::vector<std::string> data)
{

	for (auto & current_row : data)
	{
		wnd->InsertChoice(current_row);
	}
}
void ui_manager::AddListItems(Zeal::EqUI::ListWnd* wnd, const std::vector<std::vector<std::string>>data)
{

	for (int row = 0; auto& current_row : data)
	{
		
		int x = wnd->AddString("");
		for (int col = 0; auto& current_col : current_row)
		{
			wnd->SetItemText(current_col, row, col);
			col++;
		}
		wnd->SetItemData(row);
		row++;
	}
}
void ui_manager::SetChecked(std::string name, bool checked)
{
	if (checkbox_names.count(name) > 0)
		checkbox_names[name]->Checked = checked;
}

void ui_manager::SetLabelValue(std::string name, const char* format, ...)
{
	va_list argptr;
	char buffer[512];
	va_start(argptr, format);
	vsnprintf(buffer, 511, format, argptr);
	va_end(argptr);
	if (label_names.count(name) > 0)
	{
		Zeal::EqGame::EqGameInternal::CXStr_PrintString(&label_names[name]->Text, buffer);
	}
}

void ui_manager::SetComboValue(std::string name, int value)
{
	if (combo_names.count(name) > 0)
	{
		//	ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(combo_names[name]->FirstChildWnd, 0, value); //this is crashing since firstchildwnd is null, may have to maintain the combo windows ourselves?
		ZealService::get_instance()->hooks->hook_map["SetComboValue"]->original(SetComboValue_hook)(combo_names[name]->CmbListWnd, 0, value);
	}
}


Zeal::EqUI::SliderWnd* ui_manager::GetSlider(std::string name)
{
	if (slider_names.count(name))
		return slider_names[name];
	return nullptr;
}
Zeal::EqUI::BasicWnd* ui_manager::GetCheckbox(std::string name)
{
	if (checkbox_names.count(name))
		return checkbox_names[name];
	return nullptr;
}
Zeal::EqUI::BasicWnd* ui_manager::GetButton(std::string name)
{
	if (button_names.count(name))
		return button_names[name];
	return nullptr;
}
Zeal::EqUI::BasicWnd* ui_manager::GetCombo(std::string name)
{
	if (combo_names.count(name))
		return combo_names[name];
	return nullptr;
}

std::function<void(Zeal::EqUI::SliderWnd*, int)> ui_manager::GetSliderCallback(Zeal::EqUI::SliderWnd* wnd)
{
	if (slider_callbacks.count(wnd))
		return slider_callbacks[wnd];
	return nullptr;
}
std::function<void(Zeal::EqUI::BasicWnd*, int)> ui_manager::GetComboCallback(Zeal::EqUI::BasicWnd* wnd)
{
	if (combo_callbacks.count(wnd))
		return combo_callbacks[wnd];
	return nullptr;
}
std::function<void(Zeal::EqUI::BasicWnd*)> ui_manager::GetButtonCallback(Zeal::EqUI::BasicWnd* wnd)
{
	if (button_callbacks.count(wnd))
		return button_callbacks[wnd];
	return nullptr;
}
std::function<void(Zeal::EqUI::BasicWnd*)> ui_manager::GetCheckboxCallback(Zeal::EqUI::BasicWnd* wnd)
{
	if (checkbox_callbacks.count(wnd))
		return checkbox_callbacks[wnd];
	return nullptr;
}

void ui_manager::CleanUI()
{
	Zeal::EqGame::print_debug("Clean UI UIMANAGER");
	combo_names.clear();
	combo_callbacks.clear();
	checkbox_names.clear();
	checkbox_callbacks.clear();
	slider_names.clear();
	slider_callbacks.clear();
	label_names.clear();
}


void ui_manager::AddXmlInclude(const std::string& name)
{
	XMLIncludes.push_back(name);
}
void ui_manager::WriteTemporaryUI(const std::string& file_path, std::string ui_path)
{
	if (!file_path.empty())
	{
		std::ifstream infile(file_path);
		std::stringstream buffer;
		std::string line;
		bool compositeFound = false;
		std::string modifiedContent;

		if (infile)
		{
			// Read file line by line
			while (std::getline(infile, line))
			{
				// Search for the closing </composite> tag (case insensitive)
				std::string loweredLine = line;
				std::transform(loweredLine.begin(), loweredLine.end(), loweredLine.begin(), ::tolower);

				if (!compositeFound && loweredLine.find("</composite>") != std::string::npos)
				{
					compositeFound = true;

					for (auto& file : XMLIncludes)
					// Add the new lines before the closing tag
					modifiedContent += "        <Include>" +  file + "</Include>\n";
				}

				// Add the current line to the buffer
				modifiedContent += line + "\n";
			}
			infile.close();

			std::filesystem::path new_file_path = ui_path + "EQUI_Zeal.xml";
			std::filesystem::create_directories(new_file_path.parent_path());
			std::ofstream outfile(new_file_path);
			if (outfile)
			{
				outfile << modifiedContent;
				outfile.close();
			}
		}
	}
}

void ui_manager::RemoveTemporaryUI(const std::string& file_path)
{
	std::filesystem::path new_file_path = file_path + "EQUI_Zeal.xml";
	if (std::filesystem::exists(new_file_path))
	{
		std::filesystem::remove(new_file_path);
	}
}
void __fastcall LoadSidlHk(void* t, int unused, Zeal::EqUI::CXSTR path1, Zeal::EqUI::CXSTR path2, Zeal::EqUI::CXSTR filename)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	ui->included_files.clear();
	std::string str_filename = filename;
	std::string zeal_file = ui_manager::ui_path + str_filename;

	std::string ui_file = static_cast<std::string>(path1) + str_filename;
	std::string default_file = static_cast<std::string>(path2) + str_filename;
	std::string file_path = "";
	if (str_filename == "EQUI.xml")
	{
		if (std::filesystem::exists(ui_file))
			file_path = ui_file;
		else  if (std::filesystem::exists(default_file))
			file_path = default_file;

		ui->WriteTemporaryUI(file_path, path1);
		filename = Zeal::EqUI::CXSTR("EQUI_Zeal.xml");
	}
	ZealService::get_instance()->hooks->hook_map["LoadSidl"]->original(LoadSidlHk)(t, unused, path1, path2, filename);

	if (str_filename == "EQUI.xml" && !file_path.empty())
		ui->RemoveTemporaryUI(path1);
}
bool ui_manager::AlreadyLoadedXml(std::string name)
{
	std::string lName = name;
	std::transform(lName.begin(), lName.end(), lName.begin(), ::tolower);
	bool exists = std::find(included_files.begin(), included_files.end(), lName) != included_files.end();
	if (!exists)
		included_files.push_back(lName);
	else
	{
		CreateTmpXML();
		Zeal::EqGame::print_chat("Duplicate XML: %s", lName.c_str());
		//MessageBoxA(NULL, name.c_str(), "Duplicate xml", 0);
	}
	return exists;
}
void ui_manager::CreateTmpXML()
{
	std::filesystem::path new_file_path = ui_manager::ui_path + std::string("EQUI_TMP.xml");
	if (std::filesystem::exists(new_file_path))
		std::filesystem::remove(new_file_path);
	std::filesystem::create_directories(new_file_path.parent_path());
	std::ofstream outfile(new_file_path);
	if (outfile)
	{
		outfile << "<?xml version=\"1.0\" encoding=\"us-ascii\"?>" << std::endl;
		outfile << "<XML ID=\"EQInterfaceDefinitionLanguage\">" << std::endl;
		outfile << "<Schema xmlns=\"EverQuestData\" xmlns:dt=\"EverQuestDataTypes\" />" << std::endl;
		outfile << "</XML>" << std::endl;
		outfile.close();
	}
}
int __fastcall XMLRead(void* t, int unused, Zeal::EqUI::CXSTR path1, Zeal::EqUI::CXSTR path2, Zeal::EqUI::CXSTR filename)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	std::string str_filename = filename;
	std::string file = ui_manager::ui_path + str_filename;
	if (std::filesystem::exists(file))
		path1 = Zeal::EqUI::CXSTR(ui_manager::ui_path);
	else
		path1 = Zeal::EqUI::CXSTR((char*)0x63D3C0);

	if (ui->AlreadyLoadedXml(filename))
	{
		path1 = Zeal::EqUI::CXSTR(ui_manager::ui_path);
		filename = "EQUI_TMP.xml";
	}

	return ZealService::get_instance()->hooks->hook_map["XMLRead"]->original(XMLRead)(t, unused, path1, path2, filename);
}
int __fastcall XMLReadNoValidate(void* t, int unused, Zeal::EqUI::CXSTR path1, Zeal::EqUI::CXSTR path2, Zeal::EqUI::CXSTR filename)
{
	ui_manager* ui = ZealService::get_instance()->ui.get();
	std::string str_filename = filename;
	std::string file = ui_manager::ui_path + str_filename;
	if (std::filesystem::exists(file))
		path1 = Zeal::EqUI::CXSTR(ui_manager::ui_path);
	else
		path1 = Zeal::EqUI::CXSTR((char*)0x63D3C0);

	if (ui->AlreadyLoadedXml(filename))
	{
		path1 = Zeal::EqUI::CXSTR(ui_manager::ui_path);
		filename = "EQUI_TMP.xml";
	}

	return ZealService::get_instance()->hooks->hook_map["XMLReadNoValidate"]->original(XMLReadNoValidate)(t, unused, path1, path2, filename);
}


std::string ui_manager::GetUIIni()
{
	Zeal::EqStructures::EQCHARINFO* c = Zeal::EqGame::get_char_info();
	if (c)
	{
		std::string char_name = c->Name;
		return ".\\UI_" + char_name + "_pq.proj.ini";
	}
	return "";
}

ui_manager::ui_manager(ZealService* zeal, IO_ini* ini)
{
	zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	//zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);

	bank = std::make_shared<ui_bank>(zeal, ini, this);
	options = std::make_shared<ui_options>(zeal, ini, this);
	loot = std::make_shared<ui_loot>(zeal, ini, this);
	//guild = std::make_shared<ui_guild>(zeal, ini, this);
	raid = std::make_shared<ui_raid>(zeal, ini, this);
	hotbutton = std::make_shared<ui_hotbutton>(zeal, ini, this);
	group = std::make_shared<ui_group>(zeal, ini, this);


	mem::write<BYTE>(0x40f07a, 0); //disable rotation by default
	mem::write<BYTE>(0x40f07d, 0xEB); //make rotate not checked by default

	//zeal->hooks->Add("InitCharSelectSettings", 0x53c234, InitCharSelectSettings, hook_type_replace_call);
	zeal->hooks->Add("ButtonClick", 0x5951E0, ButtonClick_hook, hook_type_detour);
	zeal->hooks->Add("CheckboxClick", 0x5c3480, CheckboxClick_hook, hook_type_detour);
	zeal->hooks->Add("SetSliderValue", 0x5a6c70, SetSliderValue_hook, hook_type_detour);
	zeal->hooks->Add("SetComboValue", 0x579af0, SetComboValue_hook, hook_type_detour);
	zeal->hooks->Add("LoadSidl", 0x5992c0, LoadSidlHk, hook_type_detour);
	zeal->hooks->Add("XMLRead", 0x58D640, XMLRead, hook_type_detour);
	zeal->hooks->Add("XMLReadNoValidate", 0x58DA10, XMLReadNoValidate, hook_type_detour);
	

	zeal->commands_hook->Add("/sortskill", {}, "",
		[this](std::vector<std::string>& args) {
			if (Zeal::EqGame::Windows && Zeal::EqGame::Windows->Skills && Zeal::EqGame::Windows->Skills->IsVisible)
			{
				Zeal::EqUI::ListWnd* wnd = (Zeal::EqUI::ListWnd*)Zeal::EqGame::Windows->Skills->GetChildItem("SkillList");
				if (wnd)
				{
					wnd->Sort(2);
				}
			}
			return true;
		});
}

