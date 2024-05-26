#include "floatingdamage.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
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
DamageData::DamageData(int dmg, bool _is_my_damage_dealt, bool _is_spell)
{
	is_spell = _is_spell;
	is_my_damage = _is_my_damage_dealt;
	str_dmg = std::to_string(dmg);
	opacity = 1.0f;
	y_offset = getRandomIntBetween(-20, 20);
	x_offset = getRandomIntBetween(-20, 20);
	needs_removed = false;
	last_tick = GetTickCount64();
	start_time = GetTickCount64();
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

void FloatingDamage::callback_render()
{
	if (!Zeal::EqGame::is_in_game() || !enabled)
		return;
	if (Zeal::EqGame::get_wnd_manager())
	{
		Zeal::EqUI::CTextureFont* fnt = Zeal::EqGame::get_wnd_manager()->GetFont(5);
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
						(ZealService::get_instance()->dx->WorldToScreen({ target->Position.x,target->Position.y,target->Position.z }, screen_pos));
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

void FloatingDamage::add_damage(int* dmg_ptr)
{
	if (!enabled)
		return;
	Zeal::Packets::Damage_Struct* dmg = (Zeal::Packets::Damage_Struct*)dmg_ptr;
	if (dmg && (int)dmg->damage > 0)
	{
		Zeal::EqStructures::Entity* ent = Zeal::EqGame::get_entity_by_id(dmg->target);
		Zeal::EqStructures::Entity* src = Zeal::EqGame::get_entity_by_id(dmg->source);
		if (ent)
		{
			bool is_me = false;
			if (src && src == Zeal::EqGame::get_controlled())
				is_me = true;
			damage_numbers[ent].push_back(DamageData(dmg->damage, is_me, dmg->spellid > 0));
		}
	}
}

int __fastcall DrawWindows(int t, int u)
{
	ZealService::get_instance()->floating_damage->callback_render();
	return ZealService::get_instance()->hooks->hook_map["DrawWindows"]->original(DrawWindows)(t, u);
}

void __fastcall ReportSuccessfulHit(int t, int u, Zeal::Packets::Damage_Struct* dmg, char output_text, int heal)
{
	ZealService::get_instance()->floating_damage->add_damage((int*)dmg);
	ZealService::get_instance()->hooks->hook_map["ReportSuccessfulHit"]->original(ReportSuccessfulHit)(t, u, dmg, output_text, heal);
}

void FloatingDamage::set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "FloatingDamage", _enabled);
	enabled = _enabled;
}

FloatingDamage::FloatingDamage(ZealService* zeal, IO_ini* ini)
{
	if (!ini->exists("Zeal", "FloatingDamage"))
		ini->setValue<bool>("Zeal", "FloatingDamage", true);
	enabled = ini->getValue<bool>("Zeal", "FloatingDamage");

	zeal->hooks->Add("ReportSuccessfulHit", 0x5297D2, ReportSuccessfulHit, hook_type_detour);
	zeal->hooks->Add("DrawWindows", 0x59E000, DrawWindows, hook_type_detour); //render in this hook so damage is displayed behind ui
	//zeal->callbacks->add_generic([this]() { callback_render();  }, callback_type::Render);
}

FloatingDamage::~FloatingDamage()
{
}