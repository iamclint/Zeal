#include "floating_damage.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include <cstdint>
#include <random>
int getRandomIntBetween(int min, int max) {
	// Create a random device and a random engine
	std::random_device rd;
	static std::mt19937 gen(rd());

	// Define the distribution range
	std::uniform_int_distribution<> dis(min, max);

	// Generate and return the random integer
	return dis(gen);
}
void DamageData::tick()
{
	if (GetTickCount64() - start_time > 2500)
	{
		needs_removed = true;
		return;
	}
	if (GetTickCount64() - last_tick >= 25)
	{
		y_offset -= 2;
		opacity -= 0.02f;
		last_tick = GetTickCount64();
	}
}
DamageData::DamageData(int dmg, bool _is_my_damage_dealt, bool _is_spell, int _heal)
{
	is_spell = _is_spell;
	is_my_damage = _is_my_damage_dealt;
	if (dmg>0)
		str_dmg = std::to_string(dmg);
	else if (_heal>0)
		str_dmg = "+" + std::to_string(_heal);
	damage = dmg;
	opacity = 1.0f;
	y_offset = getRandomIntBetween(-20, 20);
	x_offset = getRandomIntBetween(-20, 20);
	needs_removed = false;
	last_tick = GetTickCount64();
	start_time = GetTickCount64();
	heal = _heal;
}

long FloatRGBAtoLong(float r, float g, float b, float a) {
	// Clamp values between 0 and 1
	if (r < 0.0f) r = 0.0f;
	else if (r > 1.0f) r = 1.0f;
	if (g < 0.0f) g = 0.0f;
	else if (g > 1.0f) g = 1.0f;
	if (b < 0.0f) b = 0.0f;
	else if (b > 1.0f) b = 1.0f;
	if (a < 0.0f) a = 0.0f;
	else if (a > 1.0f) a = 1.0f;

	// Convert float values to 8-bit unsigned integers (0-255 range)
	std::uint8_t red = static_cast<std::uint8_t>(r * 255);
	std::uint8_t green = static_cast<std::uint8_t>(g * 255);
	std::uint8_t blue = static_cast<std::uint8_t>(b * 255);
	std::uint8_t alpha = static_cast<std::uint8_t>(a * 255);

	// Pack RGBA values into a long integer (32-bit)
	return (static_cast<std::uint32_t>(alpha) << 24) | (static_cast<std::uint32_t>(red) << 16) |
		(static_cast<std::uint32_t>(green) << 8) | static_cast<std::uint32_t>(blue);
}

long ModifyAlpha(long rgba, float newAlpha) {
	// Clamp the alpha value between 0.0 and 1.0
	if (newAlpha < 0.0f) newAlpha = 0.0f;
	else if (newAlpha > 1.0f) newAlpha = 1.0f;

	// Convert the new alpha float value to an 8-bit integer (0-255)
	std::uint8_t alpha = static_cast<std::uint8_t>(newAlpha * 255);

	// Mask out the old alpha value and set the new alpha value
	rgba = (rgba & 0x00FFFFFF) | (alpha << 24);

	return rgba;
}

void FloatingDamage::callback_deferred()
{
	if (!Zeal::EqGame::is_in_game() || !enabled)
		return;
	if (Zeal::EqGame::get_wnd_manager())
	{
		Zeal::EqUI::CTextureFont* fnt = Zeal::EqGame::get_wnd_manager()->GetFont(font_size);
		if (fnt)
		{
			std::vector<Zeal::EqStructures::Entity*> visible_ents = Zeal::EqGame::get_world_visible_actor_list(250, false);
			Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
			for (auto& [target, dmg_vec] : damage_numbers)
			{
				for (auto& dmg : dmg_vec)
				{
					dmg.tick();
					if (std::find(visible_ents.begin(), visible_ents.end(), target) != visible_ents.end() || target == Zeal::EqGame::get_self())
					{
						Vec2 screen_pos = { 0,0 };
						if (ZealService::get_instance()->dx->WorldToScreen({ target->Position.x,target->Position.y,target->Position.z }, screen_pos))
						{
							long color = FloatRGBAtoLong(1.0f, 1.0f, 1.0f, dmg.opacity);
							if (dmg.is_my_damage) //if the damage is dealt by me
							{
								if (dmg.is_spell) 
									color = Zeal::EqGame::get_user_color(28);
								else
									color = Zeal::EqGame::get_user_color(10);
							}
							else if (target == Zeal::EqGame::get_controlled()) //if the target is me
							{
								if (dmg.is_spell)
									color = Zeal::EqGame::get_user_color(28);
								else
									color = Zeal::EqGame::get_user_color(11);
							}
							else if (target->Type==Zeal::EqEnums::EntityTypes::Player)
							{
								if (dmg.is_spell)
									color = Zeal::EqGame::get_user_color(28);
								else
									color = Zeal::EqGame::get_user_color(17); //player being hit
							}
							else
							{
								if (dmg.is_spell)
									color = Zeal::EqGame::get_user_color(28);
								else
									color = Zeal::EqGame::get_user_color(24); //npc being hit
							}
							if (dmg.heal > 0)
								color = 0xFFFF00FF;
							fnt->DrawWrappedText(dmg.str_dmg.c_str(), Zeal::EqUI::CXRect(screen_pos.x + dmg.y_offset, screen_pos.y + dmg.x_offset, screen_pos.x + 150, screen_pos.y + 150), Zeal::EqUI::CXRect(0, 0, screen_size.x*2, screen_size.y*2), ModifyAlpha(color, dmg.opacity), 1, 0);
						}
					}
				}
				dmg_vec.erase(std::remove_if(dmg_vec.begin(), dmg_vec.end(), [](const DamageData& d) { return d.needs_removed; }), dmg_vec.end());
			}
		}
		for (auto it = damage_numbers.begin(); it != damage_numbers.end();) {
			if (it->second.empty()) {
				it = damage_numbers.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void FloatingDamage::add_damage(Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, short dmg, int heal, short spell_id)
{
	if (!enabled)
		return;
	if ((int)dmg > 0 || heal>0)
	{
		if (target)
		{
			bool is_me = false;
			if (source && source->SpawnId == Zeal::EqGame::get_controlled()->SpawnId)
				is_me = true;

			damage_numbers[target].push_back(DamageData(dmg, is_me, spell_id > 0, heal));
		}
	}
}

void FloatingDamage::set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "FloatingDamage", _enabled);
	enabled = _enabled;
}

FloatingDamage::FloatingDamage(ZealService* zeal, IO_ini* ini)
{
	//mem::write<BYTE>(0x4A594B, 0x14);
	if (!ini->exists("Zeal", "FloatingDamage"))
		ini->setValue<bool>("Zeal", "FloatingDamage", true);
	enabled = ini->getValue<bool>("Zeal", "FloatingDamage");
	zeal->callbacks->AddGeneric([this]() { callback_deferred(); }, callback_type::AddDeferred);
	zeal->callbacks->AddReportSuccessfulHit([this](Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, WORD type, short spell_id, short damage, int heal, char output_text) { add_damage(source, target, damage, heal, spell_id); });
	zeal->commands_hook->Add("/fcd", {}, "Toggles floating combat text or adjusts the font size with argument",
		[this, ini](std::vector<std::string>& args) {
			int new_size = 5;
			if (args.size() == 2)
			{
				if (Zeal::String::tryParse(args[1], &new_size))
				{
					font_size = new_size;
					Zeal::EqGame::print_chat("Floating combat font size is now %i", font_size);
				}
			}
			else
			{
				set_enabled(!enabled);
				Zeal::EqGame::print_chat("Floating combat text is %s", enabled ? "Enabled" : "Disabled");
			}
			return true;
		});

	
	
	//zeal->callbacks->add_generic([this]() { callback_render();  }, callback_type::Render);
}

FloatingDamage::~FloatingDamage()
{
}