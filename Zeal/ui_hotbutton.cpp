#include "ui_hotbutton.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "string_util.h"
#include "Zeal.h"

void hotbutton_state::tick() { if (wnd) wnd->Checked = active(); }
bool hotbutton_state::active() { return GetTickCount64() < start_time + duration; }
void hotbutton_state::set(int _duration) { duration = _duration; start_time = GetTickCount64(); }
hotbutton_state::hotbutton_state(Zeal::EqUI::BasicWnd* btn) { wnd = btn; }
void __fastcall DoHotButton(Zeal::EqUI::EQWND* wnd, int unused, int p1, int p2)
{
	ZealService::get_instance()->ui->hotbutton->last_button = p1;
	ZealService::get_instance()->ui->hotbutton->last_page=Zeal::EqGame::Windows->HotButton->GetPage();
	ZealService::get_instance()->hooks->hook_map["DoHotButton"]->original(DoHotButton)(wnd, unused, p1, p2);
}

void __fastcall SetCheck(Zeal::EqUI::EQWND* wnd, int unused, int checked)
{
	if (ZealService::get_instance()->ui->hotbutton->is_btn_active(wnd))
		return;
	ZealService::get_instance()->hooks->hook_map["SetCheck"]->original(SetCheck)(wnd, unused, checked);
}

bool ui_hotbutton::is_btn_active(Zeal::EqUI::BasicWnd* btn)
{
	int current_page = Zeal::EqGame::Windows->HotButton->GetPage();
	if (states.count(current_page))
	{
		for (auto& [button_index, state] : states[current_page])
		{
			if (state.wnd == btn)
				return state.active();
		}
	}
	return false;
}

void ui_hotbutton::InitUI()
{
	buttons.clear();
	states.clear();
	for (int i = 0; i < 10; i++)
	{
		Zeal::EqUI::BasicWnd* btn = Zeal::EqGame::Windows->HotButton->GetChildItem("HB_Button" + std::to_string(i+1));
		if (btn)
		{
			buttons[i] = btn;
			//Zeal::EqGame::print_chat("Button checked: 0x%x", &btn->Checked);
		}
	}

	for (int page = 0; page < 10; page++)
	{
		for (int slot = 0; slot < 10; slot++)
		{
			states[page][slot] = hotbutton_state(buttons[slot]);
		}
	}
}

void ui_hotbutton::Render()
{
	if (Zeal::EqGame::is_in_game())
	{
		if (!states.size()) InitUI();
		int current_page = Zeal::EqGame::Windows->HotButton->GetPage();
		if (states.count(current_page))
		{
			for (auto& [button_index, state] : states[current_page])
			{
				if (state.active())
					state.tick();
			}
		}
	}
}

ui_hotbutton::ui_hotbutton(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->AddGeneric([this]() { Render();  }, callback_type::Render);
	zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	zeal->hooks->Add("DoHotButton", 0x4209bd, DoHotButton, hook_type_detour);
	zeal->hooks->Add("SetCheck", 0x595790, SetCheck, hook_type_detour);
	zeal->commands_hook->Add("/timer", { }, "Sets a timer for the last pressed hotbutton to keep it visually pressed in duration is in deciseconds (10=1 second).",
		[this](std::vector<std::string>& args) {

			if (args.size() > 1)
			{
				int dur = 10;
				Zeal::String::tryParse(args[1], &dur);
				states[last_page][last_button].set(dur*100);
			}
			else
			{
				Zeal::EqGame::print_chat("an argument is required for duration");
			}
			return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
		});
}
ui_hotbutton::~ui_hotbutton()
{

}
