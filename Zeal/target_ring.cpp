#define NOMINMAX
#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "d3dx8\d3dx8.h"
#include <random>
#include <algorithm>
#define NUM_VERTICES 4

struct TextureVertex {
	float x, y, z;  // Position coordinates
	float u, v;     // Texture coordinates
	D3DCOLOR color; // Optional color (0 for white)
};

struct Vertex {
	float x, y, z;  // Position coordinates
	//float u, v;     // Texture coordinates
	D3DCOLOR color; // Optional color (0 for white)
	Vertex(const TextureVertex& vert) {
		x = vert.x;
		y = vert.y;
		z = vert.z;
		color = vert.color;
	}
	Vertex() : x(0), y(0), z(0), color(0xFFFFFFFF) {};
};


#define CON_WHITE D3DCOLOR_ARGB(0x88, 0xf0, 0xf0, 0xf0)
#define CON_RED D3DCOLOR_ARGB(0x88, 0xf0, 0x0, 0x0)
#define CON_BLUE D3DCOLOR_ARGB(0x88, 0x0, 0x0, 0xf0)
#define CON_YELLOW D3DCOLOR_ARGB(0x88, 0xf0, 0xf0, 0x0)
#define CON_LIGHTBLUE D3DCOLOR_ARGB(0x88, 0x0, 0xf0, 0xf0)
#define CON_GREEN D3DCOLOR_ARGB(0x88, 0x0, 0xf0, 0x0)

DWORD GetLevelCon(Zeal::EqStructures::Entity* ent) {
	if (!ent || !Zeal::EqGame::get_self())
		return 0;
	int mylevel = Zeal::EqGame::get_self()->Level;
	short diff = ent->Level - mylevel;
	DWORD conlevel = 0;

	if (diff == 0)
		return CON_WHITE;
	else if (diff >= 1 && diff <= 2)
		return CON_YELLOW;
	else if (diff >= 3)
		return CON_RED;

	if (mylevel <= 7)
	{
		if (diff <= -4)
			conlevel = CON_GREEN;
		else
			conlevel = CON_BLUE;// Zeal::EqGame::get_user_color(70);
	}
	else if (mylevel <= 8)
	{
		if (diff <= -5)
			conlevel = CON_GREEN;
		else if (diff <= -4)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 12)
	{
		if (diff <= -6)
			conlevel = CON_GREEN;
		else if (diff <= -4)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 16)
	{
		if (diff <= -7)
			conlevel = CON_GREEN;
		else if (diff <= -5)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 20)
	{
		if (diff <= -8)
			conlevel = CON_GREEN;
		else if (diff <= -6)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 24)
	{
		if (diff <= -9)
			conlevel = CON_GREEN;
		else if (diff <= -7)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 28)
	{
		if (diff <= -10)
			conlevel = CON_GREEN;
		else if (diff <= -8)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 30)
	{
		if (diff <= -11)
			conlevel = CON_GREEN;
		else if (diff <= -9)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 32)
	{
		if (diff <= -12)
			conlevel = CON_GREEN;
		else if (diff <= -9)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 36)
	{
		if (diff <= -13)
			conlevel = CON_GREEN;
		else if (diff <= -10)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 40)
	{
		if (diff <= -14)
			conlevel = CON_GREEN;
		else if (diff <= -11)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 44)
	{
		if (diff <= -16)
			conlevel = CON_GREEN;
		else if (diff <= -12)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 48)
	{
		if (diff <= -17)
			conlevel = CON_GREEN;
		else if (diff <= -13)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 52)
	{
		if (diff <= -18)

			conlevel = CON_GREEN;
		else if (diff <= -14)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 54)
	{
		if (diff <= -19)

			conlevel = CON_GREEN;
		else if (diff <= -15)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 56)
	{
		if (diff <= -20)

			conlevel = CON_GREEN;
		else if (diff <= -15)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 60)
	{
		if (diff <= -21)
			conlevel = CON_GREEN;
		else if (diff <= -16)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 61)
	{
		if (diff <= -19)
			conlevel = CON_GREEN;
		else if (diff <= -14)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else if (mylevel <= 62)
	{
		if (diff <= -17)
			conlevel = CON_GREEN;
		else if (diff <= -12)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}
	else
	{
		if (diff <= -16)
			conlevel = CON_GREEN;
		else if (diff <= -11)
			conlevel = CON_LIGHTBLUE;
		else
			conlevel = CON_BLUE;
	}

	return conlevel;
}


RenderState::RenderState(IDirect3DDevice8* device, DWORD state, DxStateType_ type)
	: state(state), type(type)
{
	if (type == DxStateType_::DxStateType_Render)
	{
		device->GetRenderState((D3DRENDERSTATETYPE)state, &value);
	}
	else if (type == DxStateType_::DxStateType_Texture)
	{
		device->GetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)state, &value);
	}
}


void TargetRing::setup_render_states()
{
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (!device)
		return;
	render_states.clear();
	render_states.push_back({ device, (DWORD)D3DRS_ALPHABLENDENABLE, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_SRCBLEND, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_DESTBLEND, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_CULLMODE, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_ZENABLE, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_ZWRITEENABLE, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_LIGHTING, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DTSS_COLOROP, DxStateType_Texture });
	render_states.push_back({ device, (DWORD)D3DTSS_COLORARG1, DxStateType_Texture });
	render_states.push_back({ device, (DWORD)D3DTSS_ALPHAOP, DxStateType_Texture });
	render_states.push_back({ device, (DWORD)D3DTSS_ALPHAARG1, DxStateType_Texture });
	render_states.push_back({ device, (DWORD)D3DRS_BLENDOP, DxStateType_Render });
	render_states.push_back({ device, (DWORD)D3DRS_TEXTUREFACTOR, DxStateType_Render });
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Set texture stage states
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}
void TargetRing::reset_render_states()
{
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	for (auto& state : render_states)
	{
		if (state.type == DxStateType_Render)
			device->SetRenderState((D3DRENDERSTATETYPE)state.state, state.value);
		else if (state.type == DxStateType_Texture)
			device->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)state.state, state.value);
	}
}

void TargetRing::load_texture(const std::string& filename) {
	try
	{
		if (targetRingTexture)
			targetRingTexture->Release();
		targetRingTexture = nullptr;
		// Full texture path
		std::string texturePath = "./uifiles/zeal/targetrings/" + filename + ".tga";

		// Get the Direct3D device
		IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
		if (!device)
		{
			Zeal::EqGame::print_chat("Error: Failed to get Direct3D device.");
			return;
		}

		// Create texture from file
		HRESULT result = D3DXCreateTextureFromFileA(device, texturePath.c_str(), &targetRingTexture);
		if (FAILED(result))
		{
			targetRingTexture = nullptr;
			Zeal::EqGame::print_chat("Error: Failed to load texture file: " + texturePath);
			return;
		}
	}
	catch (const std::exception& ex)
	{
		Zeal::EqGame::print_chat("Exception: " + std::string(ex.what()));
	}
	catch (...)
	{
		Zeal::EqGame::print_chat("An unknown error occurred while setting the texture.");
	}
}

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

void TargetRing::render_ring(Vec3 pos, float size, DWORD color, IDirect3DTexture8* texture, float rotationAngle) {
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (!device)
		return;

	const float innerRadius = std::clamp(size - (size * inner_percent), 0.f, 100.f);
	const float outerRadius = size;
	const float angleStep = 2.0f * static_cast<float>(M_PI) / num_segments;  // Fixed truncation warning
	int vertexIndex = 0;


	TextureVertex* texture_vertices = new TextureVertex[num_segments * 2 + 2];
	Vertex* solid_vertices = new Vertex[num_segments * 2 + 2];
	Vec3 outerTerrainHeight, innerTerrainHeight;
	for (int i = 0; i <= num_segments; ++i) {
		float angle = (i * angleStep) + rotationAngle;
		Vec2 outerRadiusVertex = { outerRadius * cosf(angle), outerRadius * sinf(angle) };
		Vec2 innerRadiusVertex = { innerRadius * cosf(angle), innerRadius * sinf(angle) };

		//// Collide with world to get terrain height (this looks great on slopes but steep drop offs and walls are terrible)
		//Zeal::EqGame::collide_with_world({ pos.x + outerRadiusVertex.x, pos.y + outerRadiusVertex.y, pos.z + (size * 2) },
		//	{ pos.x + outerRadiusVertex.x, pos.y + outerRadiusVertex.y, pos.z - (size * 2) },
		//	outerTerrainHeight);
		//Zeal::EqGame::collide_with_world({ pos.x + innerRadiusVertex.x, pos.y + innerRadiusVertex.y, pos.z + (size * 2) },
		//	{ pos.x + innerRadiusVertex.x, pos.y + innerRadiusVertex.y, pos.z - (size * 2) },
		//	innerTerrainHeight);

		// Outer circle vertices
		texture_vertices[vertexIndex].x = outerRadiusVertex.x;
		texture_vertices[vertexIndex].y = outerRadiusVertex.y;
		texture_vertices[vertexIndex].z = 1.f;// outerTerrainHeight.z - pos.z;
		texture_vertices[vertexIndex].color = color;
		texture_vertices[vertexIndex].u = 1.0f;
		texture_vertices[vertexIndex].v = 1.0f - (float)i / (float)num_segments;
		solid_vertices[vertexIndex] = Vertex(texture_vertices[vertexIndex]);
		vertexIndex++;

		// Inner circle vertices
		texture_vertices[vertexIndex].x = innerRadiusVertex.x;
		texture_vertices[vertexIndex].y = innerRadiusVertex.y;
		texture_vertices[vertexIndex].z = 1.f;// innerTerrainHeight.z - pos.z;
		texture_vertices[vertexIndex].color = color;
		texture_vertices[vertexIndex].u = 0.0f;
		texture_vertices[vertexIndex].v = 1.0f - (float)i / (float)num_segments;
		solid_vertices[vertexIndex] = Vertex(texture_vertices[vertexIndex]);
		vertexIndex++;
	}
	DWORD vertex_count = num_segments * 2 + 2;
	// Create vertex buffers
	
	LPDIRECT3DVERTEXBUFFER8 solidVertexBuffer = CreateVertexBuffer(device, solid_vertices, vertex_count, D3DFVF_XYZ | D3DFVF_DIFFUSE);

	// Store and set render states
	setup_render_states();
	
	D3DXMATRIX worldMatrix, originalWorldMatrix, rotationMatrix;
	device->GetTransform(D3DTS_WORLD, &originalWorldMatrix);
	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);

	// Render the ring
	device->SetTexture(0, 0);
	device->SetStreamSource(0, solidVertexBuffer, sizeof(Vertex));
	device->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, vertex_count - 2);

	if (texture)
	{
		// Render the texture
		LPDIRECT3DVERTEXBUFFER8 texturedVertexBuffer = CreateVertexBuffer(device, texture_vertices, vertex_count, D3DFVF_XYZ | D3DFVF_TEX1);
		device->SetTexture(0, texture);
		device->SetStreamSource(0, texturedVertexBuffer, sizeof(TextureVertex));
		device->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, vertex_count - 2);
		texturedVertexBuffer->Release();
	}
	// Restore the original world matrix and render states
	device->SetTransform(D3DTS_WORLD, &originalWorldMatrix);
	reset_render_states();

	// Release resources
	solidVertexBuffer->Release();
	delete[] texture_vertices;
	delete[] solid_vertices;
}

void TargetRing::callback_render() {
	if (!enabled)
		return;
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	if (!target || !target->ActorInfo || !target->ActorInfo->ViewActor_)
		return;
	float radius = 10.f;// Zeal::EqGame::CalcCombatRange(Zeal::EqGame::get_self(), target);

	ULONGLONG currentTime = GetTickCount64(); // Get the current time in milliseconds

	// ### Target Ring Color ###
	DWORD originalColor = GetLevelCon(target);
	// Max Red, Green, and Blue by default
	DWORD Color = originalColor;
		/*Color = D3DCOLOR_ARGB(0xFF,
		(originalColor & 0x00FF0000) ? 0xFF : 0x00,
		(originalColor & 0x0000FF00) ? 0xFF : 0x00,
		(originalColor & 0x000000FF) ? 0xFF : 0x00);*/
	
	static ULONGLONG lastColorChanged = 0; // Store the last time the color was changed

	// ### Auto Attack Indicator (fade/unfade target's color while autoattack turned on)###
	if ((bool)(*(BYTE*)0x7f6ffe) && attack_indicator) // auto attack is enabled
	{
		if (currentTime - lastColorChanged >= 300) // Reset the timer every 300ms
			lastColorChanged = currentTime;

		float elapsedTime = (currentTime - lastColorChanged) / 300.0f; // Get the time elapsed in the current cycle as a fraction
		float fadeFactor;
		if (elapsedTime < 0.5f)
			fadeFactor = elapsedTime * 2; // Fade in during the first half of the cycle
		else
			fadeFactor = (1.0f - elapsedTime) * 2; // Fade out during the second half of the cycle

		// Extract the ARGB components from the original color
		BYTE originalA = (originalColor >> 24) & 0xFF;
		BYTE originalR = (originalColor >> 16) & 0xFF;
		BYTE originalG = (originalColor >> 8) & 0xFF;
		BYTE originalB = originalColor & 0xFF;

		// Calculate the faded color components
		BYTE fadedA = originalA; // Keep the original alpha value
		BYTE fadedR = (BYTE)((1.0f - fadeFactor) * 0x00 + fadeFactor * originalR);
		BYTE fadedG = (BYTE)((1.0f - fadeFactor) * 0x00 + fadeFactor * originalG);
		BYTE fadedB = (BYTE)((1.0f - fadeFactor) * 0x00 + fadeFactor * originalB);

		// Set the color with the faded components
		Color = D3DCOLOR_ARGB(fadedA, fadedR, fadedG, fadedB);
	}

	if (targetRingTexture) {

		// ### Rotate Target Ring ###
		static float rotationAngle = 0.0f;
		// Calculate the increment for a full rotation every 6 seconds
		static const float rotationIncrement = (2.0f * M_PI) / (6.0f * 1000.0f); // radians per millisecond
		// Update the rotation angle based on the elapsed time
		static ULONGLONG lastRotationTime = GetTickCount64();
		ULONGLONG currentRotationTime = GetTickCount64();
		ULONGLONG elapsedRotationTime = currentRotationTime - lastRotationTime;
		rotationAngle += (rotationIncrement * elapsedRotationTime) * rotation_speed;
		lastRotationTime = currentRotationTime;
		// Reset the rotation angle after a full rotation
		if (rotationAngle >= 2.0f * M_PI) {
			rotationAngle -= 2.0f * M_PI;
		}
		float direction = static_cast<float>(target->Heading * M_PI / 256); 
		// ### Render Target Ring ###
		render_ring({ target->Position.x, target->Position.y,  target->ActorInfo->Z }, outer_size, Color, targetRingTexture, rotate_match_heading ? direction : rotationAngle);
	}
	else
	{
		render_ring({ target->Position.x, target->Position.y,  target->ActorInfo->Z }, outer_size, Color, nullptr, rotate_match_heading ? target->Heading : 0);
	}
}

bool TargetRing::get_enabled()
{
	return enabled;
}
bool TargetRing::get_indicator()
{
	return attack_indicator;
}
bool TargetRing::get_rotation_match()
{
	return rotate_match_heading;
}
float TargetRing::get_pct()
{
	return inner_percent;
}
float TargetRing::get_rotation_speed()
{
	return rotation_speed;
}
float TargetRing::get_size()
{
	return outer_size;
}
int TargetRing::get_segments()
{
	return num_segments;
}
std::string TargetRing::get_texture()
{
	return texture_name;
}

void TargetRing::set_enabled(bool _enabled)
{
	enabled = _enabled;
	save_ini();
	Zeal::EqGame::print_chat("Target ring is %s", enabled ? "Enabled" : "Disabled");
}
void TargetRing::set_pct(float pct)
{
	inner_percent = pct;
	save_ini();
}

void TargetRing::set_indicator(bool enabled)
{
	attack_indicator = enabled;
	save_ini();
}
void TargetRing::set_rotation_speed(float speed)
{
	rotation_speed = speed;
	save_ini();
}
void TargetRing::set_rotation_match(bool enable)
{
	rotate_match_heading = enable;
	save_ini();
}
void TargetRing::set_segments(int segments)
{
	num_segments = segments;
	save_ini();
}
void TargetRing::set_texture(std::string name)
{
	texture_name = name;
	load_texture(name);
	save_ini();
}
void TargetRing::set_size(float size)
{
	outer_size = size;
	save_ini();
}


void TargetRing::save_ini()
{
	std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
	IO_ini ini(ini_name);
	ini.setValue<bool>("TargetRing", "Enabled", enabled);
	ini.setValue<bool>("TargetRing", "AttackIndicator", attack_indicator);
	ini.setValue<bool>("TargetRing", "MatchHeading", rotate_match_heading);
	ini.setValue<float>("TargetRing", "InnerSize", inner_percent);
	ini.setValue<float>("TargetRing", "RotateSpeed", rotation_speed);
	ini.setValue<float>("TargetRing", "Size", outer_size);
	ini.setValue<int>("TargetRing", "Segments", num_segments);
	ini.setValue<std::string>("TargetRing", "Texture", texture_name);
}

void TargetRing::load_ini()
{
	std::string ini_name = ".\\UI_" + std::string(Zeal::EqGame::get_self()->Name) + "_pq.proj.ini";
	IO_ini ini(ini_name);
	if (!ini.exists("TargetRing", "Enabled"))
		ini.setValue<bool>("TargetRing", "Enabled", false);
	if (!ini.exists("TargetRing", "InnerSize"))
		ini.setValue<float>("TargetRing", "InnerSize", .5);
	if (!ini.exists("TargetRing", "AttackIndicator"))
		ini.setValue<bool>("TargetRing", "AttackIndicator", false);
	if (!ini.exists("TargetRing", "MatchHeading"))
		ini.setValue<bool>("TargetRing", "MatchHeading", false);
	if (!ini.exists("TargetRing", "RotateSpeed"))
		ini.setValue<float>("TargetRing", "RotateSpeed", 1.0f);
	if (!ini.exists("TargetRing", "Size"))
		ini.setValue<float>("TargetRing", "Size", 10.f);
	if (!ini.exists("TargetRing", "Segments"))
		ini.setValue<int>("TargetRing", "Segments", 128);
	if (!ini.exists("TargetRing", "Texture"))
		ini.setValue<std::string>("TargetRing", "Texture", "");
	outer_size = ini.getValue<float>("TargetRing", "Size");
	enabled = ini.getValue<bool>("TargetRing", "Enabled");
	attack_indicator = ini.getValue<bool>("TargetRing", "AttackIndicator");
	inner_percent = ini.getValue<float>("TargetRing", "InnerSize");
	rotation_speed = ini.getValue<float>("TargetRing", "RotateSpeed");
	rotate_match_heading = ini.getValue<bool>("TargetRing", "MatchHeading");
	num_segments = ini.getValue<int>("TargetRing", "Segments");
	texture_name = ini.getValue<std::string>("TargetRing", "Texture");
}

std::vector<std::string> GetTGAFiles(const std::string& directoryPath) {
	std::vector<std::string> tgaFiles;

	// Iterate over the directory
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		// Check if it's a file and has a .tga extension
		if (entry.is_regular_file() && entry.path().extension() == ".tga") {
			// Get the filename without extension and add to the vector
			tgaFiles.push_back(entry.path().stem().string());
		}
	}

	return tgaFiles;
}

void TargetRing::options_opened()
{
	std::vector<std::string> tgas = GetTGAFiles("uifiles/zeal/targetrings");
	tgas.insert(tgas.begin(), "None");
	for (auto& t : tgas)
	{
		Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)ZealService::get_instance()->ui->options->wnd->GetChildItem("Zeal_TargetRingTexture_Combobox");
		if (!cmb)
		{
			Zeal::EqGame::print_chat("Couldn't find target ring texture combobox");
			return;
		}
		cmb->DeleteAll();
		//Zeal::EqUI::ListWnd* lst = cmb->CmbListWnd;
		//if (!lst)
		//{
		//	Zeal::EqGame::print_chat("Couldn't find the list wnd");
		//	return;
		//}
		int sel_index = -1;
		for (int i = 0; i < tgas.size(); i++)
		{
			if (Zeal::String::compare_insensitive(tgas[i], texture_name))
			{
				sel_index = i;
				break;
			}
		}

		ZealService::get_instance()->ui->AddListItems(cmb, tgas);
		cmb->SetChoice(sel_index);
		return;

	}
}

void TargetRing::callback_initui()
{
	Zeal::EqGame::print_debug("Target Ring init");
	load_texture(texture_name);
}



//don't get too excited this isn't functioning
TargetRing::TargetRing(ZealService* zeal, IO_ini* ini)
{
	zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
	zeal->callbacks->AddGeneric([this]() { load_ini(); callback_initui(); }, callback_type::InitUI);
	//zeal->callbacks->AddGeneric([this]() { callback_initui(); }, callback_type::CharacterSelect);
	zeal->callbacks->AddGeneric([this]() { callback_initui(); }, callback_type::CleanUI);

	zeal->commands_hook->Add("/loadtextures", {}, "",
		[this](std::vector<std::string>& args) {

			std::vector<std::string> tgas = GetTGAFiles("uifiles/zeal/targetrings");
			tgas.insert(tgas.begin(), "None");
			for (auto& t : tgas)
			{
				Zeal::EqUI::ComboWnd* cmb = (Zeal::EqUI::ComboWnd*)Zeal::EqGame::Windows->Options->GetChildItem("Zeal_TargetRingTexture_Combobox");
				if (!cmb)
				{
					Zeal::EqGame::print_chat("Couldn't find target ring texture combobox");
					return true;
				}
				cmb->DeleteAll();
				Zeal::EqUI::ListWnd* lst = cmb->CmbListWnd;
				if (!lst)
				{
					Zeal::EqGame::print_chat("Couldn't find the list wnd");
					return true;
				}
				lst->Location.Bottom = lst->Location.Top + (23 * (tgas.size()-1));
				ZealService::get_instance()->ui->AddListItems(lst, tgas);
				return true;
				
			}
			return true;
		});
	zeal->commands_hook->Add("/targetring", {}, "Toggles target ring",
		[this](std::vector<std::string>& args) {

			if (args.size() == 2)
			{
				if (args[1] == "indicator")
				{
					set_indicator(!attack_indicator);
					return true;
				}
				float pct = 0;
				if (!Zeal::String::tryParse(args[1], &pct))
					return true;
				set_pct(pct);
				return true;
			}
			set_enabled(!enabled);
			return true;
		});
}

TargetRing::~TargetRing()
{
}