#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
#include "d3dx8\d3dx8.h"
#include <random>
#define NUM_VERTICES 4

struct Vertex {
	float x, y, z;  // Position coordinates
	float u, v;     // Texture coordinates
	D3DCOLOR color; // Optional color (0 for white)
};

#define CON_WHITE D3DCOLOR_ARGB(0x55, 0xf0, 0xf0, 0xf0)
#define CON_RED D3DCOLOR_ARGB(0x55, 0xf0, 0x0, 0x0)
#define CON_BLUE D3DCOLOR_ARGB(0x55, 0x0, 0x0, 0xf0)
#define CON_YELLOW D3DCOLOR_ARGB(0x55, 0xf0, 0xf0, 0x0)
#define CON_LIGHTBLUE D3DCOLOR_ARGB(0x55, 0x0, 0xf0, 0xf0)
#define CON_GREEN D3DCOLOR_ARGB(0x55, 0x0, 0xf0, 0x0)

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


void TargetRing::store_render_states()
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


void TargetRing::render_ring(Vec3 pos, float size, DWORD color)
{
	IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
	if (!device)
		return;
	float innerRadius = 0;
	if (ring_pct > 0.01f && ring_pct < 1.0f)
	{
		float inner = size - (size * ring_pct);
		innerRadius = inner > 0 ? inner : 0;  // Adjust these values for ring size
	}
	const float outerRadius = size;
	const int numSegments = 32;     // Adjust for smoothness of the ring


	D3DXMATRIX worldMatrix, originalWorldMatrix;
	Vertex* vertices = new Vertex[numSegments * 2 + 2];

	// Calculate angle increment for each segment
	float angleStep = 2.0f * M_PI / numSegments;

	// Loop to create vertices for inner and outer circles
	int vertexIndex = 0;
	for (int i = 0; i < numSegments; ++i) {
		float angle = i * angleStep;

		// Outer circle vertices
		vertices[vertexIndex].x = outerRadius * cosf(angle);
		vertices[vertexIndex].y = outerRadius * sinf(angle);
		vertices[vertexIndex].z = 0.05f;
		// Rotate texture coordinates by 90 degrees
		vertices[vertexIndex].u = 1.0f;
		vertices[vertexIndex].v = (float)i / (float)numSegments;
		vertices[vertexIndex].color = color;
		vertexIndex++;

		// Inner circle vertices
		vertices[vertexIndex].x = innerRadius * cosf(angle);
		vertices[vertexIndex].y = innerRadius * sinf(angle);
		vertices[vertexIndex].z = 0.05f;
		// Rotate texture coordinates by 90 degrees
		vertices[vertexIndex].u = 0.0f;
		vertices[vertexIndex].v = (float)i / (float)numSegments;
		vertices[vertexIndex].color = color;
		vertexIndex++;
	}



	// Duplicate the first two vertices to close the ring
	vertices[vertexIndex] = vertices[0];
	vertexIndex++;
	vertices[vertexIndex] = vertices[1];
	vertexIndex++;


	// Create vertex buffer
	IDirect3DVertexBuffer8* vertexBuffer = nullptr;
	if (FAILED(device->CreateVertexBuffer(sizeof(Vertex) * (numSegments * 2 + 2),
		D3DUSAGE_WRITEONLY,
		D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		D3DPOOL_MANAGED,
		&vertexBuffer))) {
		delete[] vertices;
		return;
	}


	// Lock the vertex buffer
	BYTE* data = nullptr;
	if (FAILED(vertexBuffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
		vertexBuffer->Release();
		delete[] vertices;
		return;
	}
	memcpy(data, (const void*)vertices, sizeof(Vertex) * (numSegments * 2 + 2));
	vertexBuffer->Unlock();

	store_render_states();

	// Enable alpha blending for the ring
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Enable depth writing
	device->SetRenderState(D3DRS_LIGHTING, FALSE);  // Disable lighting


	// Set texture stage states to use custom alpha
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTexture(0, NULL);  // Ensure no texture is bound



	// Save the original world matrix
	device->GetTransform(D3DTS_WORLD, &originalWorldMatrix);
	// Set the world transformation matrix
	D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);
	device->SetTransform(D3DTS_WORLD, &worldMatrix);
	device->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	device->SetStreamSource(0, vertexBuffer, sizeof(Vertex));
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, numSegments * 2);
	device->SetTransform(D3DTS_WORLD, &originalWorldMatrix);

	reset_render_states();
	// Release resources
	vertexBuffer->Release();
	delete[] vertices;
}


IDirect3DTexture8* LoadTexture(IDirect3DDevice8* device, const std::string& filename) {
	IDirect3DTexture8* texture = nullptr;
	if (FAILED(D3DXCreateTextureFromFileA(device, filename.c_str(), &texture))) {
		//std::cerr << "Failed to load texture: " << filename << std::endl;
		return nullptr;
	}
	return texture;
}
void printMessageWithChance(const std::string& message, int denominator) {
	// Create a random device and a Mersenne Twister random number generator
	std::random_device rd;
	std::mt19937 gen(rd());
	// Define a uniform distribution from 1 to the denominator
	std::uniform_int_distribution<> dis(1, denominator);

	// Generate a random number
	int randomNumber = dis(gen);

	// Check if the random number is 1 (1/denominator chance)
	if (randomNumber == 1) {
		Zeal::EqGame::print_chat(message.c_str());
	}
}


void TargetRing::render_ring_with_texture(Vec3 pos, float size, DWORD color, IDirect3DTexture8* texture, float rotationAngle) {
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device)
        return;

	//// Load the texture (ensure it's loaded only once)
	//static IDirect3DTexture8* texture = nullptr;
	if (!texture) {
		printMessageWithChance("Texture was null.", 100);
		return;
		//texture = LoadTexture(device, "ZealTargetRing2.tga");
		//if (!texture) {
		//	//throw "Failed to load texture.";
		//	printMessageWithChance("Failed to load texture.", 100);
		//}
	}

    // Get the surface description
    D3DSURFACE_DESC desc;
    texture->GetLevelDesc(0, &desc);

    // Calculate inner and outer radii
    float innerRadius = 0;
    if (ring_pct > 0.01f && ring_pct < 1.0f) {
        float inner = size - (size * ring_pct);
        innerRadius = inner > 0 ? inner : 0;
    }
    const float outerRadius = size;
    const int numSegments = 128;  // Adjust for smoothness of the ring

    // Create vertices for the ring
    Vertex* vertices = new Vertex[numSegments * 2 + 2];
    float angleStep = 2.0f * static_cast<float>(M_PI) / numSegments;  // Fixed truncation warning
    int vertexIndex = 0;

    for (int i = 0; i <= numSegments; ++i) {  // Use <= to include the last segment
        float angle = i * angleStep;

        // Outer circle vertices
        vertices[vertexIndex].x = outerRadius * cosf(angle);
        vertices[vertexIndex].y = outerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.05f;
		// Rotate texture coordinates by 90 degrees
        vertices[vertexIndex].u = 1.0f;
        vertices[vertexIndex].v = (float)i / (float)numSegments;
        vertices[vertexIndex].color = color;
        vertexIndex++;

        // Inner circle vertices
        vertices[vertexIndex].x = innerRadius * cosf(angle);
        vertices[vertexIndex].y = innerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.05f;
		// Rotate texture coordinates by 90 degrees
        vertices[vertexIndex].u = 0.0f;
        vertices[vertexIndex].v = (float)i / (float)numSegments;
        vertices[vertexIndex].color = color;
        vertexIndex++;
    }
	//// Duplicate the first two vertices to close the ring
	//vertices[vertexIndex] = vertices[0];
	//vertexIndex++;
	//vertices[vertexIndex] = vertices[1];
	//vertexIndex++;

    // Create vertex buffer
    IDirect3DVertexBuffer8* vertexBuffer = nullptr;
    if (FAILED(device->CreateVertexBuffer(sizeof(Vertex) * (numSegments * 2 + 2),
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_TEX1,
        D3DPOOL_MANAGED,
        &vertexBuffer))) {
        delete[] vertices;
        return;
    }

    // Lock the vertex buffer
    BYTE* data = nullptr;
    if (FAILED(vertexBuffer->Lock(0, 0, &data, D3DLOCK_DISCARD))) {
        vertexBuffer->Release();
        delete[] vertices;
        return;
    }
    memcpy(data, (const void*)vertices, sizeof(Vertex) * (numSegments * 2 + 2));
    vertexBuffer->Unlock();

    // Store and set render states
    store_render_states();
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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
    device->SetTexture(0, texture);

    // Save the original world matrix and set the new one
    D3DXMATRIX worldMatrix, originalWorldMatrix, rotationMatrix;
    device->GetTransform(D3DTS_WORLD, &originalWorldMatrix);

    // Create rotation matrix
    D3DXMatrixRotationZ(&rotationMatrix, rotationAngle);

    // Create translation matrix
    D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

    // Combine rotation and translation matrices
    D3DXMatrixMultiply(&worldMatrix, &rotationMatrix, &worldMatrix);

    // Set the combined transformation matrix
    device->SetTransform(D3DTS_WORLD, &worldMatrix);

    // Render the ring
    device->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
    device->SetStreamSource(0, vertexBuffer, sizeof(Vertex));
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, numSegments * 2);

    // Restore the original world matrix and render states
    device->SetTransform(D3DTS_WORLD, &originalWorldMatrix);
    reset_render_states();

    // Release resources
    vertexBuffer->Release();
    delete[] vertices;
}

float CalcBoundingRadius(Zeal::EqStructures::Entity* ent)
{
	int myrace = ent->Race;
	float mysize = ent->Height;
	float base_size = 5.0f;
	int mygender = ent->Gender;
	float myradius = 6.0f;
	switch (myrace)
	{
	case 1: // "Human"
	case 2: // "Barbarian"
	case 3: // "Erudite"
	case 4: // "Wood Elf"
	case 5: // "High Elf"
	case 6: // "Dark Elf"
	case 7: // "Half Elf"
	case 8: // "Dwarf"
	case 9: // "Troll",
	case 10: // "Ogre"
	case 11: // "Halfling"
	case 12: // "Gnome"
		// playable races have fixed mods
		mysize = 5.0f;
		myradius = 5.0f;
		break;
	case 13: // "Aviak"
		break;
	case 14: // "Were Wolf"
		break;
	case 15: // "Brownie",
		break;
	case 16: // "Centaur"
		break;
	case 17: // "Golem"
		break;
	case 18: // "Giant / Cyclops"
		break;
	case 19: // "Trakanon",
		myradius = 10.48f;
		break;
	case 20:
		break;
	case 21: // "Evil Eye"
		break;
	case 22: // "Beetle"
		break;
	case 23: // "Kerra"
		break;
	case 24: // "Fish"
		break;
	case 25: // "Fairy"
		break;
	case 26: // "Old Froglok"
		break;
	case 27: // "Old Froglok Ghoul"
		break;
	case 28: // "Fungusman"
		break;
	case 29: // "Gargoyle"
		break;
	case 30: // "Gasbag"
		break;
	case 31: // "Gelatinous Cube"
		break;
	case 33: // "Ghoul"
		break;
	case 34: // "Giant Bat"
		mysize = 5.0f;
		break;
	case 36: // "Giant Rat"
		break;
	case 37: // "Giant Snake"
		break;
	case 38: // "Giant Spider"
		break;
	case 39: // "Gnoll"
		break;
	case 40: // "Goblin"
		break;
	case 41: // "Gorilla"
		break;
	case 42: // "Wolf"
		break;
	case 43: // "Bear"
		break;
	case 44: // "Freeport Guards"
		break;
	case 45: // "Demi Lich"
		break;
	case 46: // "Imp"
		break;
	case 47: // "Griffin"
		break;
	case 48: // "Kobold"
		break;
	case 49: // "Lava Dragon"
		mysize = 32.5f;
		break;
	case 50: // "Lion"
		break;
	case 51: // "Lizard Man"
		break;
	case 52: // "Mimic"
		break;
	case 53: // "Minotaur"
		break;
	case 54: // "Orc"
		break;
	case 55: // "Human Beggar"
		break;
	case 56: // "Pixie"
		break;
	case 57: // "Dracnid"
		break;
	case 58: // "Solusek Ro"
		break;
	case 59: // "Bloodgills"
		break;
	case 60: // "Skeleton"
		break;
	case 61: // "Shark"
		break;
	case 62: // "Tunare"
		break;
	case 63: // "Tiger"
		break;
	case 64: // "Treant"
		break;
	case 65: // "Vampire"
		break;
	case 66: // "Rallos Zek"
		break;
	case 67: // "Highpass Citizen"
		break;
	case 68: // "Tentacle"
		break;
	case 69: // "Will 'O Wisp"
		break;
	case 70: // "Zombie"
		break;
	case 71: // "Qeynos Citizen"
		break;
	case 74: // "Piranha"	
		break;
	case 75: // "Elemental"
		break;
	case 76: // "Puma"
		break;
	case 77: // "Neriak Citizen"
		break;
	case 78: // "Erudite Citizen"
		break;
	case 79: // "Bixie"
		break;
	case 80: // "Reanimated Hand"
		break;
	case 81: // "Rivervale Citizen"
		break;
	case 82: // "Scarecrow"
		break;
	case 83: // "Skunk"
		break;
	case 85: // "Spectre"
		break;
	case 86: // "Sphinx"
		break;
	case 87: // "Armadillo"
		break;
	case 88: // "Clockwork Gnome"
		break;
	case 89: // "Drake"
		break;
	case 90: // "Halas Citizen"
		break;
	case 91: // "Alligator"
		break;
	case 92: // "Grobb Citizen"
		break;
	case 93: // "Oggok Citizen"
		break;
	case 94: // "Kaladim Citizen"
		break;
	case 95: // "Cazic Thule"
		break;
	case 96: // "Cockatrice"
		break;
	case 98: // "Elf Vampire"
		break;
	case 99: // "Denizen"
		break;
	case 100: // "Dervish"
		break;
	case 101: // "Efreeti"
		break;
	case 102: // "Old Froglok Tadpole"
		break;
	case 103: // "Kedge"
		break;
	case 104: // "Leech"
		break;
	case 105: // "Swordfish"
		break;
	case 106: // "Felguard"
		break;
	case 107: // "Mammoth"
		break;
	case 108: // "Eye of Zomm"
		break;
	case 109: // "Wasp"
		break;
	case 110: // "Mermaid"
		break;
	case 111: // "Harpie"
		break;
	case 112: // "Fayguard"
		break;
	case 113: // "Drixie"
		break;
	case 116: // "Sea Horse"
		break;
	case 117: // "Ghost Dwarf"
		break;
	case 118: // "Erudite Ghost"
		break;
	case 119: // "Sabertooth Cat"
		break;
	case 120: // "Wolf Elemental",
		break;
	case 121: // "Gorgon"
		break;
	case 122: // "Dragon Skeleton"
		break;
	case 123: // "Innoruuk"
		break;
	case 124: // "Unicorn"
		break;
	case 125: // "Pegasus"
		break;
	case 126: // "Djinn"
		break;
	case 127: // "Invisible Man"
		break;
	case 128: // "Iksar"
		// playable races have fixed mods
		mysize = 5.0f;
		myradius = 5.0f;
		break;
	case 129: // "Scorpion"
		break;
	case 130: // "Vah Shir"
		// playable races have fixed mods
		mysize = 5.0f;
		myradius = 5.0f;
		break;
	case 131: // "Sarnak"
		break;
	case 133: // "Lycanthrope"
		break;
	case 134: // "Mosquito"
		break;
	case 135: // "Rhino"
		break;
	case 136: // "Xalgoz"
		break;
	case 137: // "Kunark Goblin"
		break;
	case 138: // "Yeti"
		break;
	case 139: // "Iksar Citizen"
		break;
	case 140: // "Forest Giant"
		break;
	case 144: // "Burynai"
		break;
	case 145: // "Goo"
		break;
	case 146: // "Spectral Sarnak"
		break;
	case 147: // "Spectral Iksar"
		break;
	case 148: // "Kunark Fish"
		break;
	case 149: // "Iksar Scorpion"
		break;
	case 150: // "Erollisi"
		break;
	case 151: // "Tribunal"
		break;
	case 153: // "Bristlebane"
		break;
	case 154: // "Fay Drake"
		break;
	case 155: // "Sarnak Skeleton"
		break;
	case 156: // "Ratman"
		break;
	case 157: // "Wyvern"
		break;
	case 158: // "Wurm"
		mysize = 16.0f;
		break;
	case 159: // "Devourer"
		break;
	case 160: // "Iksar Golem"
		break;
	case 161: // "Iksar Skeleton"
		break;
	case 162: // "Man Eating Plant"
		break;
	case 163: // "Raptor"
		break;
	case 164: // "Sarnak Golem"
		break;
	case 165: // "Water Dragon"
		break;
	case 166: // "Iksar Hand"
		break;
	case 167: // "Succulent"
		break;
	case 168: // "Flying Monkey"
		break;
	case 169: // "Brontotherium"
		break;
	case 170: // "Snow Dervish"
		break;
	case 171: // "Dire Wolf"
		break;
	case 172: // "Manticore"
		break;
	case 173: // "Totem"
		break;
	case 174: // "Cold Spectre"
		break;
	case 175: // "Enchanted Armor"
		break;
	case 176: // "Snow Bunny"
		break;
	case 177: // "Walrus"
		break;
	case 178: // "Rock-gem Men"
		break;
	case 181: // "Yak Man"
		break;
	case 183: // "Coldain"
		break;
	case 184: // "Velious Dragons"
		myradius = 9.48f;
		break;
	case 185: // "Hag"
		break;
	case 187: // "Siren"
		break;
	case 188: // "Frost Giant"
		break;
	case 189: // "Storm Giant"
		break;
	case 190: // "Ottermen"
		break;
	case 191: // "Walrus Man"
		break;
	case 192: // "Clockwork Dragon"
		myradius = 10.48f;
		break;
	case 193: // "Abhorent"
		break;
	case 194: // "Sea Turtle"
		break;
	case 195: // "Black and White Dragons"
		myradius = 9.48f;
		break;
	case 196: // "Ghost Dragon"
		myradius = 9.48f;
		break;
	case 198: // "Prismatic Dragon"
		myradius = 9.48f;
		break;
	case 199: // "ShikNar"
		break;
	case 200: // "Rockhopper"
		break;
	case 201: // "Underbulk"
		break;
	case 202: // "Grimling"
		break;
	case 203: // "Vacuum Worm"
		break;
	case 205: // "Kahli Shah"
		break;
	case 206: // "Owlbear"
		break;
	case 207: // "Rhino Beetle"
		break;
	case 208: // "Vampyre"
		break;
	case 209: // "Earth Elemental"
		break;
	case 210: // "Air Elemental"
		break;
	case 211: // "Water Elemental"
		break;
	case 212: // "Fire Elemental"
		break;
	case 213: // "Wetfang Minnow"
		break;
	case 214: // "Thought Horror"
		break;
	case 215: // "Tegi"
		break;
	case 216: // "Horse"
		break;
	case 217: // "Shissar"
		break;
	case 218: // "Fungal Fiend"
		break;
	case 219: // "Vampire Volatalis"
		break;
	case 220: // "StoneGrabber"
		break;
	case 221: // "Scarlet Cheetah"
		break;
	case 222: // "Zelniak"
		break;
	case 223: // "Lightcrawler"
		break;
	case 224: // "Shade"
		break;
	case 225: // "Sunflower"
		break;
	case 226: // "Sun Revenant"
		break;
	case 227: // "Shrieker"
		break;
	case 228: // "Galorian"
		break;
	case 229: // "Netherbian"
		break;
	case 230: // "Akheva"
		break;
	case 231: // "Spire Spirit"
		break;
	case 232: // "Sonic Wolf"
		break;
	case 234: // "Vah Shir Skeleton"
		break;
	case 235: // "Mutant Humanoid"
		break;
	case 236: // "Seru"
		break;
	case 237: // "Recuso"
		break;
	case 238: // "Vah Shir King"
		break;
	case 239: // "Vah Shir Guard"
		break;
	case 241: // "Lujein",
	case 242: // "Naiad",
	case 243: // "Nymph",
	case 244: // "Ent",
	case 245: // "Fly Man",
	case 246: // "Tarew Marr"
		break;
	case 247: // "Sol Ro"
		break;
	case 248: // "Clockwork Golem"
		break;
	case 249: // "Clockwork Brain",
	case 250: // "Spectral Banshee",
	case 251: // "Guard of Justice",
	case 252: // 'PoM Castle',
	case 253: // "Disease Boss"
	case 254: // "Sol Ro Guard"
	case 255: // "New Bertox",
	case 256: // "New Tribunal",
	case 257: // "Terris Thule",
	case 258: // "Vegerog",
	case 259: // "Crocodile",
	case 260: // "Bat",
	case 261: // "Slarghilug",
	case 262: // "Tranquilion"
	case 263: // "Tin Soldier"
	case 264: // "Nightmare Wraith",
	case 265: // "Malarian",
	case 266: // "Knight of Pestilence",
	case 267: // "Lepertoloth",
	case 268: // "Bubonian Boss",
	case 269: // "Bubonian Underling",
	case 270: // "Pusling",
	case 271: // "Water Mephit",
	case 272: // "Stormrider",
	case 273: // "Junk Beast"
		break;
	case 274: // "Broken Clockwork"
		break;
	case 275: // "Giant Clockwork",
	case 276: // "Clockwork Beetle",
	case 277: // "Nightmare Goblin",
	case 278: // "Karana",
	case 279: // "Blood Raven",
	case 280: // "Nightmare Gargoyle",
	case 281: // "Mouths of Insanity",
	case 282: // "Skeletal Horse",
	case 283: // "Saryn",
	case 284: // "Fennin Ro",
	case 285: // "Tormentor",
	case 286: // "Necro Priest",
	case 287: // "Nightmare",
	case 288: // "New Rallos Zek",
	case 289: // "Vallon Zek",
	case 290: // "Tallon Zek",
	case 291: // "Air Mephit",
	case 292: // "Earth Mephit",
	case 293: // "Fire Mephit",
	case 294: // "Nightmare Mephit",
	case 295: // "Zebuxoruk",
	case 296: // "Mithaniel Marr",
	case 297: // "Undead Knight",
	case 298: // "The Rathe",
	case 299: // "Xegony",
	case 300: // "Fiend",
	case 301: // "Test Object",
	case 302: // "Crab",
	case 303: // "Phoenix",
	case 304: // "PoP Dragon",
	case 305: // "PoP Bear",
	case 306: // "Storm Taarid",
	case 307: // "Storm Satuur",
	case 308: // "Storm Kuraaln",
	case 309: // "Storm Volaas",
	case 310: // "Storm Mana",
	case 311: // "Storm Fire",
	case 312: // "Storm Celestial",
	case 313: // "War Wraith",
	case 314: // "Wrulon",
	case 315: // "Kraken",
	case 316: // "Poison Frog",
	case 317: // "Queztocoatal",
	case 318: // "Valorian",
	case 319: // "War Boar",
	case 320: // "PoP Efreeti",
	case 321: // "War Boar Unarmored",
	case 322: // "Black Knight",
	case 323: // "Animated Armor",
	case 324: // "Undead Footman",
	case 325: // "Rallos Zek Minion"
	case 326: // "Arachnid"
	case 327: // "Crystal Spider",
	case 328: // "Zeb Cage",
	case 329: // "BoT Portal",
	case 330: // "Froglok",
	case 331: // "Troll Buccaneer",
	case 332: // "Troll Freebooter",
	case 333: // "Troll Sea Rover",
	case 334: // "Spectre Pirate Boss",
	case 335: // "Pirate Boss",
	case 336: // "Pirate Dark Shaman",
	case 337: // "Pirate Officer",
	case 338: // "Gnome Pirate",
	case 339: // "Dark Elf Pirate",
	case 340: // "Ogre Pirate",
	case 341: // "Human Pirate",
	case 342: // "Erudite Pirate",
	case 343: // "Poison Dart Frog",
	case 344: // "Troll Zombie",
	case 345: // "Luggald Land",
	case 346: // "Luggald Armored",
	case 347: // "Luggald Robed",
	case 348: // "Froglok Mount",
	case 349: // "Froglok Skeleton",
	case 350: // "Undead Froglok",
	case 351: // "Chosen Warrior",
	case 352: // "Chosen Wizard",
	case 353: // "Veksar",
	case 354: // "Greater Veksar",
	case 355: // "Veksar Boss",
	case 356: // "Chokadai",
	case 357: // "Undead Chokadai",
	case 358: // "Undead Veksar",
	case 359: // "Vampire Lesser",
	case 360: // "Vampire Elite",	
		break;
	default:
		myradius = 6.0;
		break;

	}
	myradius *= mysize;
	myradius /= base_size;
	return myradius;
}

float CalcZOffset(Zeal::EqStructures::Entity* ent)
{
	float size = std::clamp(ent->Height, 0.f, 20.f); //rule has bestzsizemax at 20 on server code
	switch (ent->Race)
	{
	case 49: //dragon
	case 158: //wurm
	case 196: //ghost dragon
		return 20.f;
	case 63: //tiger
		return (size / 5.0f * 3.125f * 0.44999999f);
	case 42: //wolf
	case 120: //wolf elemntal
		if (ent->Gender == 2)
			return (size / 5.0f * 3.125f * 0.44999999f);
	}
	return (size / 5.0f * 3.125f);
}

float CalcCombatRange(Zeal::EqStructures::Entity* entity1, Zeal::EqStructures::Entity* entity2)
{
	float size_mod = (CalcBoundingRadius(entity1) + CalcBoundingRadius(entity2)) * 0.75f;
	float z_diff = std::abs(CalcZOffset(entity1) - CalcZOffset(entity2));
	if (entity2->MovementSpeed > 0)
		size_mod += 2.0f;
	size_mod += z_diff;
	size_mod = std::clamp(size_mod, 14.f, 75.f);
	return size_mod;
}

void TargetRing::callback_render() {	
	if (!enabled)
		return;
	Zeal::EqStructures::Entity* target = Zeal::EqGame::get_target();
	if (!target || !target->ActorInfo || !target->ActorInfo->ViewActor_)
		return;
	float radius = CalcCombatRange(Zeal::EqGame::get_self(), target);

	if (!textures.size())
	{
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_green.tga"));
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_ltblue.tga"));
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_dkblue.tga"));
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_white.tga"));
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_yellow.tga"));
		textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTarget_red.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
		//textures.push_back(LoadTexture(ZealService::get_instance()->dx->GetDevice(), "ZealTargetRing3.tga"));
	}


	DWORD originalColor = GetLevelCon(target);
	DWORD Color = originalColor;

	static ULONGLONG lastTime = 0; // Store the last time the color was changed
	ULONGLONG currentTime = GetTickCount64(); // Get the current time in milliseconds

	if ((bool)(*(BYTE*)0x7f6ffe) && attack_indicator) // auto attack is enabled
	{
		if (currentTime - lastTime >= 300) // Reset the timer every 300ms
		{
			lastTime = currentTime;
		}

		float elapsedTime = (currentTime - lastTime) / 300.0f; // Get the time elapsed in the current cycle as a fraction
		float fadeFactor;

		if (elapsedTime < 0.5f)
		{
			fadeFactor = elapsedTime * 2; // Fade in during the first half of the cycle
		}
		else
		{
			fadeFactor = (1.0f - elapsedTime) * 2; // Fade out during the second half of the cycle
		}

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

	// Call the new method
	//try
	//{

	//}
	//catch (const std::string err)
	//{
	//	Zeal::EqGame::print_chat(err.c_str());
	//	// Commented out the old method call

	IDirect3DTexture8* texture = nullptr;
	int color_index = 0;
	switch (Color)
	{
	case CON_GREEN:
		color_index = 0;
		break;
	case CON_LIGHTBLUE:
		color_index = 1;
		break;
	case CON_BLUE:
		color_index = 2;
		break;
	case CON_WHITE:
		color_index = 3;
		break;
	case CON_YELLOW:
		color_index = 4;
		break;
	case CON_RED:
		color_index = 5;
		break;
	default:
		color_index = 0;
		break;
	}
	texture = textures[color_index];
	//if (!texture) {
	//	//throw "Failed to load texture.";
	//	printMessageWithChance("Failed to load texture.", 100);
	//	render_ring({ target->Position.x, target->Position.y,  target->ActorInfo->Z + 0.3f }, radius, Color);
	//}
	//else {


	// Static variable to keep track of the rotation angle
	static float rotationAngle = 0.0f;

	// Calculate the increment for a full rotation every 6 seconds
	static const float rotationIncrement = (2.0f * M_PI) / (6.0f * 1000.0f); // radians per millisecond

	// Update the rotation angle based on the elapsed time
	static ULONGLONG lastRotationTime = GetTickCount64();
	ULONGLONG currentRotationTime = GetTickCount64();
	ULONGLONG elapsedRotationTime = currentRotationTime - lastRotationTime;
	rotationAngle += rotationIncrement * elapsedRotationTime;
	lastRotationTime = currentRotationTime;

	// Reset the rotation angle after a full rotation
	if (rotationAngle >= 2.0f * M_PI) {
		rotationAngle -= 2.0f * M_PI;
	}

	// Call the render method with the rotation angle
	render_ring_with_texture({ target->Position.x, target->Position.y,  target->ActorInfo->Z + 0.3f }, radius, Color, texture, rotationAngle);
	//}

/*
ZealTarget_green.tga
ZealTarget_ltblue.tga
ZealTarget_dkblue.tga
ZealTarget_white.tga
ZealTarget_yellow.tga
ZealTarget_red.tga

#define CON_WHITE D3DCOLOR_ARGB(0x55, 0xf0, 0xf0, 0xf0)
#define CON_RED D3DCOLOR_ARGB(0x55, 0xf0, 0x0, 0x0)
#define CON_BLUE D3DCOLOR_ARGB(0x55, 0x0, 0x0, 0xf0)
#define CON_YELLOW D3DCOLOR_ARGB(0x55, 0xf0, 0xf0, 0x0)
#define CON_LIGHTBLUE D3DCOLOR_ARGB(0x55, 0x0, 0xf0, 0xf0)
#define CON_GREEN D3DCOLOR_ARGB(0x55, 0x0, 0xf0, 0x0)

	*/

		//}
}

void TargetRing::set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "TargetRing", _enabled);
	enabled = _enabled;
	Zeal::EqGame::print_chat("Target ring is %s", enabled ? "Enabled" : "Disabled");
}
void TargetRing::set_pct(float pct)
{
	ZealService::get_instance()->ini->setValue<float>("Zeal", "RingPCT", pct);
	ring_pct = pct;
	if (!enabled)
		set_enabled(true);
}

void TargetRing::set_indicator(bool enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "AttackIndicator", enabled);
	attack_indicator = enabled;
	if (!enabled)
		set_enabled(true);
}
void TargetRing::load_ini(IO_ini* ini)
{
	if (!ini->exists("Zeal", "TargetRing"))
		ini->setValue<bool>("Zeal", "TargetRing", false);
	if (!ini->exists("Zeal", "RingPCT"))
		ini->setValue<float>("Zeal", "RingPCT", 0);
	if (!ini->exists("Zeal", "AttackIndicator"))
		ini->setValue<bool>("Zeal", "AttackIndicator", false);

	enabled = ini->getValue<bool>("Zeal", "TargetRing");
	attack_indicator = ini->getValue<bool>("Zeal", "AttackIndicator");
	ring_pct = ini->getValue<float>("Zeal", "RingPCT");
}

//don't get too excited this isn't functioning
TargetRing::TargetRing(ZealService* zeal, IO_ini* ini)
{

	load_ini(ini);
	zeal->callbacks->AddGeneric([this]() { callback_render(); }, callback_type::RenderUI);
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

	//textures[0] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_green.tga");
	//textures[1] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_ltblue.tga");
	//textures[2] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_dkblue.tga");
	//textures[3] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_white.tga");
	//textures[4] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_yellow.tga");
	//textures[5] = LoadTexture(zeal->dx->GetDevice(), "ZealTarget_red.tga");


	//texture = LoadTexture(device, "ZealTargetRing2.tga");

}

TargetRing::~TargetRing()
{
	//for (int i = 0; i < 6; ++i) {
	//	if (textures[i]) {
	//		textures[i]->Release();
	//		textures[i] = nullptr;
	//	}
	//}
}