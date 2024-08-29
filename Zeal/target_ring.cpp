#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"
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
        if (state.type==DxStateType_Render)
            device->SetRenderState((D3DRENDERSTATETYPE)state.state, state.value);
        else if (state.type==DxStateType_Texture)
            device->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)state.state, state.value);
    }
}


void TargetRing::render_ring(Vec3 pos, float size, DWORD color)
{
    IDirect3DDevice8* device = ZealService::get_instance()->dx->GetDevice();
    if (!device)
        return;
    float innerRadius = 0;
    if (ring_pct > 0.01f && ring_pct<1.0f)
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
    if (!enabled)
        return;
	Zeal::EqStructures::Entity* target =  Zeal::EqGame::get_target();
    if (!target || !target->ActorInfo || !target->ActorInfo->ViewActor_)
        return;
    float radius = target->ActorInfo->ViewActor_->BoundingRadius;
    if (radius > 30)
        radius = 30;
    radius *= target->ActorInfo->ViewActor_->ScaleFactor;
    if (radius > 60)
        radius = 60;

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
    render_ring({ target->Position.x, target->Position.y,  target->ActorInfo->Z + 0.3f }, radius, Color);

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
                if (args[2] == "indicator")
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