#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"

#define NUM_VERTICES 4

struct Vertex {
	float x, y, z;  // Position coordinates
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
            conlevel =  Zeal::EqGame::get_user_color(70);
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
    IDirect3DDevice8* device = ZealService::get_instance()->dx->device;
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
    IDirect3DDevice8* device = ZealService::get_instance()->dx->device;
    for (auto& state : render_states)
    {
        if (state.type==DxStateType_Render)
            device->SetRenderState((D3DRENDERSTATETYPE)state.state, state.value);
        else if (state.type==DxStateType_Texture)
            device->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)state.state, state.value);
    }
}


void TargetRing::render_ring(Vec3 pos, float size, DWORD color)
{
    IDirect3DDevice8* device = ZealService::get_instance()->dx->device;
    if (!device)
        return;

    const float innerRadius = 0.0f;  // Adjust these values for ring size
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

        // Outer circle vertices first (ensure consistent winding order)
        vertices[vertexIndex].x = outerRadius * cosf(angle);
        vertices[vertexIndex].y = outerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.05f;  // Slightly above the XY plane
        vertices[vertexIndex].color = color;
        vertexIndex++;

        // Inner circle vertices
        vertices[vertexIndex].x = innerRadius * cosf(angle);
        vertices[vertexIndex].y = innerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.05f;  // Slightly above the XY plane
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
        D3DFVF_XYZ | D3DFVF_DIFFUSE,
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

    // Set texture stage states to avoid any unexpected texturing
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    device->SetTexture(0, NULL);  // Ensure no texture is bound


    // Save the original world matrix
    device->GetTransform(D3DTS_WORLD, &originalWorldMatrix);
    // Set the world transformation matrix


    D3DXMatrixTranslation(&worldMatrix,pos.x, pos.y, pos.z);
    device->SetTransform(D3DTS_WORLD, &worldMatrix);
    device->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    device->SetStreamSource(0, vertexBuffer, sizeof(Vertex));
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, numSegments * 2);
    device->SetTransform(D3DTS_WORLD, &originalWorldMatrix);

    reset_render_states();
    // Release resources
    vertexBuffer->Release();
    delete[] vertices;
}

void TargetRing::callback_render()
{
	Zeal::EqStructures::Entity* target =  Zeal::EqGame::get_target();
    if (!target || !target->ActorInfo)
        return;

    render_ring({ target->Position.x, target->Position.y,  target->ActorInfo->Z + 0.3f }, 5.0f, GetLevelCon(target));

} 

void TargetRing::set_enabled(bool _enabled)
{
	ZealService::get_instance()->ini->setValue<bool>("Zeal", "TargetRing", _enabled);
	enabled = _enabled;
}

//don't get too excited this isn't functioning
TargetRing::TargetRing(ZealService* zeal, IO_ini* ini)
{
	if (!ini->exists("Zeal", "TargetRing"))
		ini->setValue<bool>("Zeal", "TargetRing", false);
	enabled = ini->getValue<bool>("Zeal", "TargetRing");
	zeal->callbacks->add_generic([this]() { callback_render(); }, callback_type::RenderUI);
	zeal->commands_hook->add("/targetring", {}, "Toggles target ring",
		[this](std::vector<std::string>& args) {
			set_enabled(!enabled);
			Zeal::EqGame::print_chat("Target ring is %s", enabled ? "Enabled" : "Disabled");
			return true;
		});
}

TargetRing::~TargetRing()
{
}