#include "target_ring.h"
#include "EqPackets.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "directx.h"
#define NUM_VERTICES 4

struct Vertex {
	float x, y, z;  // Position coordinates
	D3DCOLOR color; // Optional color (0 for white)
};

void TargetRing::callback_render()
{
	Zeal::EqStructures::Entity* target =  Zeal::EqGame::get_target();
	IDirect3DDevice8* device = ZealService::get_instance()->dx->device;
	if (!target || !device || !target->ActorInfo)
		return;
    color = D3DCOLOR_ARGB(0x55, 0, 0xFF, 0x0);
    const float innerRadius = 0.0f;  // Adjust these values for ring size
    const float outerRadius = 5.0f;
    const int numSegments = 32;     // Adjust for smoothness of the ring


    D3DXMATRIX worldMatrix, originalWorldMatrix;
    Vertex* vertices = new Vertex[numSegments * 2];

    // Calculate angle increment for each segment
    float angleStep = 2.0f * M_PI / numSegments;

    // Loop to create vertices for inner and outer circles
    int vertexIndex = 0;
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleStep;

        // Outer circle vertices first (ensure consistent winding order)
        vertices[vertexIndex].x = outerRadius * cosf(angle);
        vertices[vertexIndex].y = outerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.1f;  // Slightly above the XY plane
        vertices[vertexIndex].color = color;
        vertexIndex++;

        // Inner circle vertices
        vertices[vertexIndex].x = innerRadius * cosf(angle);
        vertices[vertexIndex].y = innerRadius * sinf(angle);
        vertices[vertexIndex].z = 0.1f;  // Slightly above the XY plane
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
    if (FAILED(device->CreateVertexBuffer(sizeof(Vertex) * (numSegments * 2+2),
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
    memcpy(data, vertices, sizeof(Vertex) * (numSegments * 2+2));
    vertexBuffer->Unlock();

    DWORD origAlphaBlendEnable, origSrcBlend, origDestBlend, origCull;
    device->GetRenderState(D3DRS_ALPHABLENDENABLE, &origAlphaBlendEnable);
    device->GetRenderState(D3DRS_SRCBLEND, &origSrcBlend);
    device->GetRenderState(D3DRS_DESTBLEND, &origDestBlend);
    device->GetRenderState(D3DRS_CULLMODE, &origCull);
    // Enable alpha blending for the ring
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    // Save the original world matrix
    device->GetTransform(D3DTS_WORLD, &originalWorldMatrix);
    // Set the world transformation matrix
    
        
    D3DXMatrixTranslation(&worldMatrix, target->Position.x, target->Position.y, target->ActorInfo->Z + 0.3);
    device->SetTransform(D3DTS_WORLD, &worldMatrix);
   // device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    device->SetStreamSource(0, vertexBuffer, sizeof(Vertex));
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, numSegments * 2);
    device->SetTransform(D3DTS_WORLD, &originalWorldMatrix);

    device->SetRenderState(D3DRS_ALPHABLENDENABLE, origAlphaBlendEnable);
    device->SetRenderState(D3DRS_SRCBLEND, origSrcBlend);
    device->SetRenderState(D3DRS_DESTBLEND, origDestBlend);
    device->SetRenderState(D3DRS_CULLMODE, origCull);
    // Release resources
    vertexBuffer->Release();
    delete[] vertices;
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
		ini->setValue<bool>("Zeal", "TargetRing", true);
	enabled = ini->getValue<bool>("Zeal", "TargetRing");
	zeal->callbacks->add_generic([this]() { callback_render(); }, callback_type::EndScene);
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