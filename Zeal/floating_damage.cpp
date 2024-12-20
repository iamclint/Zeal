#include "floating_damage.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include <cstdint>
#include <random>

#if 0  // Not currently used.

template <typename VertexType>
LPDIRECT3DVERTEXBUFFER8 CreateVertexBuffer(LPDIRECT3DDEVICE8 d3dDevice, VertexType* vertices, int vertexCount, DWORD fvf)
{
	LPDIRECT3DVERTEXBUFFER8 vertexBuffer = nullptr;

	// Create the vertex buffer
	HRESULT result = d3dDevice->CreateVertexBuffer(
		vertexCount * sizeof(VertexType),   // Size of the buffer
		D3DUSAGE_WRITEONLY,                 // Usage flags
		fvf,                                // Flexible Vertex Format (FVF)
		D3DPOOL_DEFAULT,                    // Memory pool to use
		&vertexBuffer                       // Output vertex buffer
	);

	if (FAILED(result)) {
		// Handle error
		return nullptr;
	}

	// Lock the vertex buffer and copy the vertex data into it
	void* pVertices;
	vertexBuffer->Lock(0, vertexCount * sizeof(VertexType), (BYTE**)&pVertices, 0);
	memcpy(pVertices, vertices, vertexCount * sizeof(VertexType));
	vertexBuffer->Unlock();

	return vertexBuffer;
}
#endif

int getRandomIntBetween(int min, int max) {
	// Create a random device and a random engine
	std::random_device rd;
	static std::mt19937 gen(rd());

	// Define the distribution range
	std::uniform_int_distribution<> dis(min, max);

	// Generate and return the random integer
	return dis(gen);
}


void DamageData::tick(int active_number_count)
{
	if (GetTickCount64() - start_time > 2500)
	{
		needs_removed = true;
		return;
	}
	if (GetTickCount64() - last_tick >= 25)
	{
		float speed_offset = (float)active_number_count / 20.f;
		float opacity_offset = (float)active_number_count / 200.f;
		y_offset -= 2 + speed_offset;
		opacity -= 0.01f + opacity_offset;
		if (opacity < 0)
			needs_removed = true;
		last_tick = GetTickCount64();
	}
}
DamageData::DamageData(int dmg, bool _is_my_damage_dealt, bool _is_spell, int _heal, Zeal::EqStructures::SPELL* sp)
{
	is_spell = _is_spell;
	is_my_damage = _is_my_damage_dealt;
	if (dmg>0)
		str_dmg = std::to_string(dmg);
	else if (_heal>0)
		str_dmg = "+" + std::to_string(_heal);
	damage = dmg;
	opacity = 1.0f;
	y_offset = static_cast<float>(getRandomIntBetween(-20, 20));
	x_offset = static_cast<float>(getRandomIntBetween(-40, 40));
	needs_removed = false;
	last_tick = GetTickCount64();
	start_time = GetTickCount64();
	heal = _heal;
	spell = sp;
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

int  FloatingDamage::get_active_damage_count(Zeal::EqStructures::Entity* ent)
{
	if (!damage_numbers.count(ent))
		return 0;
	return damage_numbers[ent].size();
}

void FloatingDamage::callback_render()
{
	if (!enabled.get() || !Zeal::EqGame::is_in_game())
		return;

	load_bitmap_font();
	if (bitmap_font)
		render_text();

	render_spells();
}

void FloatingDamage::render_spells()
{
	if (!spell_icons.get())
		return;
	std::vector<Zeal::EqStructures::Entity*> visible_ents = Zeal::EqGame::get_world_visible_actor_list(250, false);
	Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
	for (auto& [target, dmg_vec] : damage_numbers)
	{
		for (auto& dmg : dmg_vec)
		{
			if (dmg.spell)
			{
				if (std::find(visible_ents.begin(), visible_ents.end(), target) != visible_ents.end() || target == Zeal::EqGame::get_self())
				{
					Vec2 screen_pos = { 0,0 };
					if (ZealService::get_instance()->dx->WorldToScreen({ target->Position.x,target->Position.y,target->Position.z }, screen_pos))
					{
						draw_icon(dmg.spell->Icon, screen_pos.x + dmg.y_offset-5, screen_pos.y + dmg.x_offset - 28, dmg.opacity);
					}
				}
			}
		}
	}
}

void FloatingDamage::callback_deferred()
{
	if (enabled.get() && Zeal::EqGame::is_in_game() && bitmap_font == nullptr)
		render_text();  // Bitmap fonts were disabled, so use the client CTextureFont.
}

// Simple helper calc that scales shadows with the font size.
static int calc_shadow_offset(const std::unique_ptr<BitmapFont>& bitmap_font)
{
	if (!bitmap_font)
		return 0;

	int offset = static_cast<int>(bitmap_font->get_line_spacing() * 0.05f + 0.5f);
	return max(1, offset);
}

void FloatingDamage::render_text()
{
	if (bitmap_font || Zeal::EqGame::get_wnd_manager())
	{
		Zeal::EqUI::CTextureFont* fnt = bitmap_font ? nullptr : Zeal::EqGame::get_wnd_manager()->GetFont(font_size);
		if (bitmap_font || fnt)
		{
			int shadow_offset = calc_shadow_offset(bitmap_font);
			std::vector<Zeal::EqStructures::Entity*> visible_ents = Zeal::EqGame::get_world_visible_actor_list(250, false);
			Vec2 screen_size = ZealService::get_instance()->dx->GetScreenRect();
			for (auto& [target, dmg_vec] : damage_numbers)
			{
				for (auto& dmg : dmg_vec)
				{
					dmg.tick(get_active_damage_count(target));
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
							if (bitmap_font)
							{  // Draw with a black drop shadow.
								bitmap_font->queue_string(dmg.str_dmg.c_str(), Vec2(screen_pos.y + dmg.x_offset + shadow_offset,
									screen_pos.x + dmg.y_offset + shadow_offset), false, ModifyAlpha(0, dmg.opacity));
								bitmap_font->queue_string(dmg.str_dmg.c_str(), Vec2(screen_pos.y + dmg.x_offset,
									screen_pos.x + dmg.y_offset), false, ModifyAlpha(color, dmg.opacity));
							}
							else 
								fnt->DrawWrappedText(dmg.str_dmg.c_str(),
									Zeal::EqUI::CXRect((int)(screen_pos.x + dmg.y_offset),(int)(screen_pos.y + dmg.x_offset), (int)(screen_pos.x + 150),
										(int)(screen_pos.y + 150)), Zeal::EqUI::CXRect(0, 0, (int)(screen_size.x * 2), (int)(screen_size.y * 2)), ModifyAlpha(color, dmg.opacity), 1, 0);
						}
					}
				}
				dmg_vec.erase(std::remove_if(dmg_vec.begin(), dmg_vec.end(), [](const DamageData& d) { return d.needs_removed; }), dmg_vec.end());
			}
			if (bitmap_font)
				bitmap_font->flush_queue_to_screen();
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
	if (!enabled.get())
		return;
	if ((int)dmg > 0 || heal>0)
	{
		if (target)
		{
			bool is_me = false;
			if (source && source->SpawnId == Zeal::EqGame::get_controlled()->SpawnId)
				is_me = true;
			Zeal::EqStructures::SPELL* sp_data = nullptr;
			if (spell_id > 0)
			{
				sp_data = Zeal::EqGame::get_spell_mgr()->Spells[spell_id];
//				Zeal::EqGame::print_chat("%s", sp_data->CastOnAnother);
			}
			if (spell_id > 0 && !spells.get())
				return;
			damage_numbers[target].push_back(DamageData(dmg, is_me, spell_id > 0, heal, sp_data));
		}
	}
}


void FloatingDamage::draw_icon(int texture_index, float y, float x, float opacity)
{
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	DWORD sheet_index = texture_index / 100;
	if (sheet_index > textures.size())
		return;
	IDirect3DTexture8* texture = textures.at(sheet_index);
	texture_index = texture_index % 99;
	const int image_size = 24;    // Width and height of each image in pixels
	const int texture_grid_size = 10;  // 10x10 grid

	// Calculate row and column based on index
	int row = texture_index / texture_grid_size;
	int column = texture_index % texture_grid_size;

	// Calculate UV coordinates
	float u_start = column * (image_size / 256.0f); // 240 is total width of texture
	float v_start = row * (image_size / 256.0f);    // 240 is total height of texture
	float u_end = u_start + (image_size / 256.0f);
	float v_end = v_start + (image_size / 256.0f);

	// Define the vertices for a textured quad
	struct Vertex
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};
		
	DWORD color = D3DCOLOR_ARGB((int)(255.f*opacity), 255, 255, 255);

	Vertex vertices[] =
	{
		{ x, y, 0.0f, 1.0f,color, u_start, v_start },
		{ x + image_size, y, 0.0f, 1.0f,color, u_end, v_start },
		{ x, y + image_size, 0.0f, 1.0f,color, u_start, v_end },
		{ x + image_size, y + image_size, 0.0f, 1.0f,color, u_end, v_end }
	};
	ZealService::get_instance()->target_ring->setup_render_states();
	device->SetTexture(0, texture);
	device->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));
	device->SetTexture(0, NULL);  // Release reference to texture.
	ZealService::get_instance()->target_ring->reset_render_states();
}

IDirect3DTexture8* FloatingDamage::load_texture(std::string path)
{
	IDirect3DTexture8* texture=nullptr;
	// Get the Direct3D device
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (!device)
	{
		Zeal::EqGame::print_chat("Error: Failed to get Direct3D device.");
		return nullptr;
	}

	// Create texture from file
	HRESULT result = D3DXCreateTextureFromFileA(device, path.c_str(), &texture);
	if (FAILED(result))
	{
		Zeal::EqGame::print_chat("Error: Failed to load texture file: " + path);
		return nullptr;
	}
	return texture;
}

bool FloatingDamage::add_texture(std::string path)
{
	if (std::filesystem::exists(path))
	{
		//Zeal::EqGame::print_chat("Added texture: %s", path.c_str());
		textures.push_back(load_texture(path));
		return true;
	}
	return false;
}

void FloatingDamage::init_ui()
{
	clean_ui();  // Just in case releases all resources and clears textures.
	std::string current_ui = (char*)0x63D3C0;
	std::string path = current_ui;
	std::string default_path = "./UIFILES/default/";
	for (int i = 1; i <= 3; i++)
	{
		std::stringstream filepath;
		filepath << path << "gemicons0" << i << ".tga";
		if (add_texture(filepath.str()))
			continue;
		filepath.str("");
		filepath << default_path << "gemicons0" << i << ".tga";
		if (!add_texture(filepath.str()))
			Zeal::EqGame::print_chat("Texture not found: %s", filepath.str().c_str());
	}
	auto ini = ZealService::get_instance()->ini.get();  // Short-life pointer.
	if (ini)
	{
		if (!ini->exists("Zeal", "FloatingDamageFont"))
			ini->setValue<std::string>("Zeal", "FloatingDamageFont", std::string(""));
		bitmap_font_filename = ini->getValue<std::string>("Zeal", "FloatingDamageFont");
	}
}

void FloatingDamage::clean_ui()
{
	for (auto& texture : textures)
		if (texture)
			texture->Release();
	textures.clear();
	bitmap_font.reset();
}

bool FloatingDamage::set_font(std::string font_name) {
	if (bitmap_font && bitmap_font_filename == font_name)
		return true;  // Already loaded.

	bitmap_font.reset();  // Release all resources and reset to nullptr.
	if (font_name == kUseClientFontString)
		font_name = "";  // Disables the bitmap font.
	bitmap_font_filename = font_name;  // Attempts to load at next render pass.

	if (ZealService::get_instance() && ZealService::get_instance()->ini)
		ZealService::get_instance()->ini->setValue<std::string>("Zeal", "FloatingDamageFont", bitmap_font_filename);

	return true;  // Always succeed for now.
}

std::vector<std::string> FloatingDamage::get_available_fonts() const {
	auto fonts = BitmapFont::get_available_fonts();
	if (!fonts.empty() && fonts[0] == BitmapFont::kDefaultFontName)  // Default is too small.
		fonts.erase(fonts.begin());
	fonts.insert(fonts.begin(), kUseClientFontString);
	return fonts;
}


// Loads the bitmap font for real-time text rendering to screen.
void FloatingDamage::load_bitmap_font() {
	if (bitmap_font || bitmap_font_filename.empty())
		return;

	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (device != nullptr)
		bitmap_font = BitmapFont::create_bitmap_font(*device, bitmap_font_filename);
	if (!bitmap_font) {
		Zeal::EqGame::print_chat("Failed to load font: %s", bitmap_font_filename.c_str());
		bitmap_font_filename = "";  // Disable future attempts and use CTexture font.
	}
}

FloatingDamage::FloatingDamage(ZealService* zeal, IO_ini* ini)
{
	//mem::write<BYTE>(0x4A594B, 0x14);
	if (!ini->exists("Zeal", "FloatingDamage"))
		ini->setValue<bool>("Zeal", "FloatingDamage", true);
	if (!ini->exists("Zeal", "FloatingDamageSpells"))
		ini->setValue<bool>("Zeal", "FloatingDamageSpells", true);
	if (!ini->exists("Zeal", "FloatingDamageIcons"))
		ini->setValue<bool>("Zeal", "FloatingDamageIcons", true);
	enabled = ini->getValue<bool>("Zeal", "FloatingDamage");
	spells = ini->getValue<bool>("Zeal", "FloatingDamageSpells");
	spell_icons = ini->getValue<bool>("Zeal", "FloatingDamageIcons");
	zeal->callbacks->AddGeneric([this]() { callback_deferred(); }, callback_type::AddDeferred);
	zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
	zeal->callbacks->AddReportSuccessfulHit([this](Zeal::EqStructures::Entity* source, Zeal::EqStructures::Entity* target, WORD type, short spell_id, short damage, int heal, char output_text) { add_damage(source, target, damage, heal, spell_id); });
	zeal->callbacks->AddGeneric([this]() { init_ui(); }, callback_type::InitUI);
	zeal->callbacks->AddGeneric([this]() { clean_ui(); }, callback_type::CleanUI);
	zeal->callbacks->AddGeneric([this]() { clean_ui(); }, callback_type::DXReset);  // Just release all resources.

	zeal->commands_hook->Add("/fcd", {}, "Toggles floating combat text or adjusts the fonts with arguments",
		[this, ini](std::vector<std::string>& args) {
			int new_size = 5;
			if (args.size() == 3 && args[1] == "font")
			{
				set_font(args[2]); 
				Zeal::EqGame::print_chat("Floating combat font set to %s", bitmap_font_filename.c_str());
			}
			else if (args.size() == 2 && args[1] == "font") {
				auto fonts = BitmapFont::get_available_fonts();
				Zeal::EqGame::print_chat("Usage: `/fcd font <fontname>` selects the zeal font <fontname>");
				Zeal::EqGame::print_chat("Available fonts:");
				for (const auto& font : fonts)
					Zeal::EqGame::print_chat("  %s", font.c_str());
			}
			else if (args.size() == 2 && Zeal::String::tryParse(args[1], &new_size))
			{
				font_size = new_size;
				Zeal::EqGame::print_chat("Floating combat font size is now %i", font_size);
				set_font("");  // Releases resources and disables the bitmap_font path.
			}
			else if (args.size() == 1)
			{
				enabled.toggle();
				Zeal::EqGame::print_chat("Floating combat text is %s", enabled.get() ? "Enabled" : "Disabled");
			}
			else {
				Zeal::EqGame::print_chat("Usage: `/fcd` toggles the enable on and off");
				Zeal::EqGame::print_chat("Usage: `/fcd <#>` selects the client font size (1 to 6)");
				Zeal::EqGame::print_chat("Usage: `/fcd font` prints the available fonts");
				Zeal::EqGame::print_chat("Usage: `/fcd font <fontname>` selects the zeal font <fontname>");
			}

			return true;
		});		
}

FloatingDamage::~FloatingDamage()
{
	clean_ui();
}