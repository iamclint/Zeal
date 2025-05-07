#include "ui_buff.h"
#include "Zeal.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include <string>
#include <cstdio>
#include <cstring>


TickTime EQ_CalculateTickTime(int ticks) {
	TickTime time{ 0, 0, 0 };

	if (ticks > 0) {
		time.seconds = ticks * 6;

		time.hours = time.seconds / 3600;
		time.seconds %= 3600;

		time.minutes = time.seconds / 60;
		time.seconds %= 60;
	}

	return time;
}

std::string EQ_GetShortTickTimeString(int ticks) {
	TickTime time = EQ_CalculateTickTime(ticks);
	char timeText[128] = { 0 };

	if (time.hours > 0) {
		std::snprintf(timeText, sizeof(timeText), "%dh", time.hours);
	}
	else if (time.minutes > 0) {
		std::snprintf(timeText, sizeof(timeText), "%dm", time.minutes);
	}
	else {
		std::snprintf(timeText, sizeof(timeText), "%ds", time.seconds);
	}
	return std::string(timeText);
}


int __fastcall BuffWindow_Refresh(Zeal::EqUI::BuffWindow* this_ptr, void* not_used) //this is used for the actual tooltip
{
	int result = ZealService::get_instance()->hooks->hook_map["BuffWindow_Refresh"]->original(BuffWindow_Refresh)(this_ptr, not_used);
	if (!ZealService::get_instance()->ui->buffs->BuffTimers.get())
		return result;
	Zeal::EqStructures::EQCHARINFO* charInfo = Zeal::EqGame::get_char_info();
	if (!charInfo)
		return result;

	int max_buffs = charInfo->GetMaxBuffs();

	// Detects whether we are using the Normal Buff Window or the Song Window.
	int start_buff_offset = (this_ptr == Zeal::EqGame::Windows->BuffWindowNORMAL) ? 0 : EQ_NUM_BUFFS;

	for (size_t i = 0; i < EQ_NUM_BUFFS; i++)
	{
		int buff_slot = i + start_buff_offset;
		if (buff_slot >= max_buffs) {
			break;
		}

		Zeal::EqStructures::_EQBUFFINFO* buff = charInfo->GetBuff(buff_slot);
		if (buff->BuffType == 0 || !Zeal::EqGame::Spells::IsValidSpellIndex(buff->SpellId))
			continue;

		int buffTicks = buff->Ticks;
		if (!buffTicks)
			continue;

		char buffTimeText[128];
		std::snprintf(buffTimeText, sizeof(buffTimeText) - 1, " %s", EQ_GetShortTickTimeString(buffTicks).c_str());
		Zeal::EqUI::BuffWindowButton* buffButtonWnd = this_ptr->BuffButtonWnd[i];
		if (buffButtonWnd && buffButtonWnd->ToolTipText.Data)
			buffButtonWnd->ToolTipText.Append(buffTimeText);
	}
	return result;
};

int __fastcall BuffWindow_PostDraw(Zeal::EqUI::BuffWindow* this_ptr, void* not_used)
{
	int result = ZealService::get_instance()->hooks->hook_map["BuffWindow_PostDraw"]->original(BuffWindow_PostDraw)(this_ptr, not_used);
	if (!ZealService::get_instance()->ui->buffs->BuffTimers.get())
		return result;
	Zeal::EqStructures::EQCHARINFO* charInfo = Zeal::EqGame::get_char_info();
	if (!charInfo)
		return result;

	int max_buffs = charInfo->GetMaxBuffs();

	// Detects whether we are using the Normal Buff Window or the Song Window.
	int start_buff_offset = (this_ptr == Zeal::EqGame::Windows->BuffWindowNORMAL) ? 0 : EQ_NUM_BUFFS;

	for (size_t i = 0; i < EQ_NUM_BUFFS; i++)
	{
		int buff_slot = i + start_buff_offset;
		if (buff_slot >= max_buffs) {
			break;
		}

		Zeal::EqStructures::_EQBUFFINFO* buff = charInfo->GetBuff(buff_slot);
		if (buff->BuffType == 0 || !Zeal::EqGame::Spells::IsValidSpellIndex(buff->SpellId))
			continue;

		int buffTicks = buff->Ticks;
		if (!buffTicks)
			continue;

		char buffTimeText[128];
		std::snprintf(buffTimeText, sizeof(buffTimeText) - 1, "%s", EQ_GetShortTickTimeString(buffTicks).c_str());
		Zeal::EqUI::BuffWindowButton* buffButtonWnd = this_ptr->BuffButtonWnd[i];
		if (buffButtonWnd && buffButtonWnd->ToolTipText.Data)
		{
				std::string orig = buffButtonWnd->ToolTipText.Data->Text;
				buffButtonWnd->ToolTipText.Set(buffTimeText);
				Zeal::EqUI::CXRect relativeRect = buffButtonWnd->GetScreenRect();
				buffButtonWnd->DrawTooltipAtPoint(relativeRect.Left, relativeRect.Top);
				buffButtonWnd->ToolTipText.Set(orig);
		}
	}

	return result;
}

// Support for spell recast timers as tool tips.
int __fastcall CastSpellWnd_PostDraw(Zeal::EqUI::CastSpellWnd* this_ptr, void* not_used)
{
	int result = ZealService::get_instance()->hooks->hook_map["CastSpellWnd_PostDraw"]
		->original(CastSpellWnd_PostDraw)(this_ptr, not_used);

	if (!ZealService::get_instance()->ui->buffs->RecastTimers.get() ||
		Zeal::EqGame::get_wnd_manager()->AltKeyState)  // Skip if alt tooltip key is pressed.
		return result;

	auto self = Zeal::EqGame::get_self();
	auto actor_info = self ? self->ActorInfo : nullptr;
	auto char_info = Zeal::EqGame::get_char_info();
	int* this_display = *(int**)Zeal::EqGame::Display;
	if (!self || !actor_info || !char_info || !this_display)
		return result;
	int game_time = this_display[200 / 4];  // The client uses this display offset as a timestamp.

	for (size_t i = 0; i < EQ_NUM_SPELL_GEMS; i++)
	{
		if (!Zeal::EqGame::Spells::IsValidSpellIndex(char_info->MemorizedSpell[i]) ||
			actor_info->CastingSpellId == char_info->MemorizedSpell[i])
			continue;
		if (actor_info->RecastTimeout[i] <= game_time)
			continue;

		int time_left = actor_info->RecastTimeout[i] - game_time;
		int secs_left = (time_left + 500) / 1000;
		if (secs_left <= 0)
			continue;

		auto time_text = (secs_left > 11) ? EQ_GetShortTickTimeString((secs_left + 5) / 6) :
			std::format("{}s", secs_left);
		auto gem_wnd = this_ptr->SpellSlots[i];
		if (!gem_wnd || !gem_wnd->ToolTipText.Data)
			continue;

		std::string orig = gem_wnd->ToolTipText.Data->Text;
		gem_wnd->ToolTipText.Set(time_text);
		Zeal::EqUI::CXRect relativeRect = gem_wnd->GetScreenRect();
		int x = ZealService::get_instance()->ui->buffs->RecastTimersLeftAlign.get() ?
			relativeRect.Left : relativeRect.Right;
		gem_wnd->DrawTooltipAtPoint(x + 2, relativeRect.Top + 2);  // Match alt tip.
		gem_wnd->ToolTipText.Set(orig);
	}

	return result;
}

ui_buff::ui_buff(ZealService* zeal, UIManager* mgr)
{
	zeal->hooks->Add("BuffWindow_PostDraw", 0x4095FE, BuffWindow_PostDraw, hook_type_detour);
	zeal->hooks->Add("BuffWindow_Refresh", 0x409334, BuffWindow_Refresh, hook_type_detour);
	zeal->hooks->Add("CastSpellWnd_PostDraw", 0x0040a2a4, CastSpellWnd_PostDraw, hook_type_detour);
	ui = mgr;
	//zeal->callbacks->AddGeneric([this]() { CleanUI(); }, callback_type::CleanUI);
	//zeal->callbacks->AddGeneric([this]() { InitUI(); }, callback_type::InitUI);
	//zeal->callbacks->AddGeneric([this]() { Deactivate(); }, callback_type::DeactivateUI);
}
ui_buff::~ui_buff()
{


}