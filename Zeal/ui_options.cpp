#include "ui_options.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>

void ui_options::InitUI()
{
	/*add callbacks when the buttons are pressed in the options window*/
	ui->AddCheckboxCallback("Zeal_HideCorpse", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->looting_hook->set_hide_looted(wnd->Checked); });
	ui->AddCheckboxCallback("Zeal_Cam", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->camera_mods->set_smoothing(wnd->Checked); });
	ui->AddCheckboxCallback("Zeal_BlueCon", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_bluecon(wnd->Checked); });
	ui->AddCheckboxCallback("Zeal_Timestamp", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_timestamp(wnd->Checked); });
	ui->AddCheckboxCallback("Zeal_Input", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->chat_hook->set_input(wnd->Checked); });
	ui->AddCheckboxCallback("Zeal_Escape", [](Zeal::EqUI::BasicWnd* wnd) { ZealService::get_instance()->ini->setValue<bool>("Zeal", "Escape", wnd->Checked); });
	
	ui->AddCheckboxCallback("Zeal_ShowHelm", [](Zeal::EqUI::BasicWnd* wnd) { Zeal::EqGame::print_chat("Show helm toggle"); });
	ui->AddSliderCallback("Zeal_PanDelaySlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->set_pan_delay(value*4); 
		ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	});
	ui->AddSliderCallback("Zeal_FirstPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	});
	ui->AddSliderCallback("Zeal_FirstPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	});
	ui->AddSliderCallback("Zeal_ThirdPersonSlider_X", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	});
	ui->AddSliderCallback("Zeal_ThirdPersonSlider_Y", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd = value / 50.f;
		ZealService::get_instance()->camera_mods->update_sensitivity();
		ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	});
	ui->AddSliderCallback("Zeal_FoVSlider", [this](Zeal::EqUI::SliderWnd* wnd, int value) {
		float val = 45.0f + (static_cast<float>(value) / 100.0f) * 45.0f;
		ZealService::get_instance()->camera_mods->set_fov(val);

		// Update the label with the remapped value
		ui->SetLabelValue("Zeal_FoVValueLabel", "%f", val);
	});
	ui->AddLabel("Zeal_PanDelayValueLabel");
	ui->AddLabel("Zeal_FirstPersonLabel_X");
	ui->AddLabel("Zeal_FirstPersonLabel_Y");
	ui->AddLabel("Zeal_ThirdPersonLabel_X");
	ui->AddLabel("Zeal_ThirdPersonLabel_Y");
	ui->AddLabel("Zeal_FoVValueLabel");

	//AddComboCallback("Zeal_HideCorpseCombobox", [this](Zeal::EqUI::BasicWnd* wnd, int value) { Zeal::EqGame::print_chat("Combo set to %i", value); });
	/*set the current states*/
	UpdateOptions();
}


void ui_options::CleanUI()
{

}


void ui_options::UpdateOptions()
{
	ui->SetComboValue("Zeal_HideCorpseCombobox", 2);
	ui->SetSliderValue("Zeal_PanDelaySlider", ZealService::get_instance()->camera_mods->pan_delay > 0.f ? ZealService::get_instance()->camera_mods->pan_delay / 4 : 0.f);
	ui->SetSliderValue("Zeal_ThirdPersonSlider_Y", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd * 50 : 0.f);
	ui->SetSliderValue("Zeal_ThirdPersonSlider_X", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd * 50 : 0.f);
	ui->SetSliderValue("Zeal_FirstPersonSlider_Y", ZealService::get_instance()->camera_mods->user_sensitivity_y > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_y * 50 : 0.f);
	ui->SetSliderValue("Zeal_FirstPersonSlider_X", ZealService::get_instance()->camera_mods->user_sensitivity_x > 0.f ? ZealService::get_instance()->camera_mods->user_sensitivity_x * 50 : 0.f);
	ui->SetSliderValue("Zeal_FoVSlider", static_cast<int>((ZealService::get_instance()->camera_mods->fov - 45.0f) / 45.0f * 100.0f));
	ui->SetLabelValue("Zeal_FoVValueLabel", "%f", ZealService::get_instance()->camera_mods->fov);
	ui->SetLabelValue("Zeal_FirstPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x);
	ui->SetLabelValue("Zeal_FirstPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y);
	ui->SetLabelValue("Zeal_ThirdPersonLabel_X", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_x_3rd);
	ui->SetLabelValue("Zeal_ThirdPersonLabel_Y", "%.2f", ZealService::get_instance()->camera_mods->user_sensitivity_y_3rd);
	ui->SetLabelValue("Zeal_PanDelayValueLabel", "%d ms", ZealService::get_instance()->camera_mods->pan_delay);
	ui->SetChecked("Zeal_HideCorpse", ZealService::get_instance()->looting_hook->hide_looted);
	ui->SetChecked("Zeal_Cam", ZealService::get_instance()->camera_mods->enabled);
	ui->SetChecked("Zeal_BlueCon", ZealService::get_instance()->chat_hook->bluecon);
	ui->SetChecked("Zeal_Timestamp", ZealService::get_instance()->chat_hook->timestamps);
	ui->SetChecked("Zeal_Input", ZealService::get_instance()->chat_hook->zealinput);
	ui->SetChecked("Zeal_Escape", ZealService::get_instance()->ini->getValue<bool>("Zeal", "Escape"));
}



ui_options::ui_options(ZealService* zeal, IO_ini* ini, ui_manager* mgr)
{
	ui = mgr;
	zeal->callbacks->add_generic([this]() { CleanUI(); }, callback_type::CleanUI);
	zeal->callbacks->add_generic([this]() { InitUI(); }, callback_type::InitUI);
	//if (Zeal::EqGame::is_in_game()) InitUI();
}
ui_options::~ui_options()
{
}

