#include "helm_manager.h"
#include "string_util.h"
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"

constexpr BYTE kMaterialSlotHead = 0;
constexpr WORD kMaterialNone = 0;
constexpr WORD kMaterialVeliousHelm = 240;
constexpr DWORD kColorNone = 0;

// Enables debug logging
constexpr bool HELM_MANAGER_LOG_DEBUG = false;

// Called when changing helms.
int __fastcall SwapHead_hk(int* cDisplay, int unused_edx, Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, bool from_server)
{
    return ZealService::get_instance()->helm->HandleSwapHead(cDisplay, entity, new_material, old_material, color, from_server);
}

// Called when changing armor textures (head chest arms legs feet hands).
// On character select screen, also called for weapons slots.
void __fastcall WearChangeArmor_hk(int* cDisplay, int unused_edx, Zeal::EqStructures::Entity* spawn, int wear_slot, WORD new_material, WORD old_material, DWORD colors, bool from_server)
{
    ZealService::get_instance()->helm->HandleWearChangeArmor(cDisplay, spawn, wear_slot, new_material, old_material, colors, from_server);
}

// This is the main hook that fixes pretty much all of the weird helmet behaviors, as noted in the comments below.
int HelmManager::HandleSwapHead(int* cDisplay, Zeal::EqStructures::Entity* entity, int new_material, int old_material_or_head, DWORD color, bool from_server)
{
    // Respect the ShowHelm toggle.
    if (entity == Zeal::EqGame::get_self() && !ShowHelmEnabled.get() && entity->Texture == 0xFF)
    {
        new_material = kMaterialNone;
        color = kColorNone;
    }
    return ZealService::get_instance()->hooks->hook_map["SwapHead"]->original(SwapHead_hk)(cDisplay, 0, entity, new_material, old_material_or_head, color, from_server);
}

// Callback when Server (local_only=true) or UI (local_only=false) is changing our armor texture
void HelmManager::HandleWearChangeArmor(int* cDisplay, Zeal::EqStructures::Entity* entity, int wear_slot, WORD new_material, WORD old_material, DWORD colors, bool from_server)
{
    // If server gave us a helm graphic but ShowHelm is off, refresh the helm
    if (wear_slot == kMaterialSlotHead 
        && entity == Zeal::EqGame::get_self()
        && !ShowHelmEnabled.get()
        && new_material != kMaterialNone
        && from_server
        && entity->Texture == 0xFF)
    {
        RefreshHelm(false, 0); // Refresh to remove our helm
    }
    ZealService::get_instance()->hooks->hook_map["WearChangeArmor"]->original(WearChangeArmor_hk)(cDisplay, 0, entity, wear_slot, new_material, old_material, colors, from_server);
}

// Process outbound helm WearChanges to make sure the right material value is sent out
bool HelmManager::Handle_Out_OP_WearChange(Zeal::Packets::WearChange_Struct* wc)
{
    if (wc && wc->wear_slot_id == kMaterialSlotHead)
    {
        if (block_outbound_wearchange > 0)
            return true;

        // Respect ShowHelm toggle
        if (!ShowHelmEnabled.get() && Zeal::EqGame::get_self() && Zeal::EqGame::get_self()->Texture == 0xFF)
        {
            wc->material = kMaterialNone;
            wc->color.Color = kColorNone;
        }
    }
    return false; // Continue processing this OP_WearChange, sending the message.
}

// Gets the material/model ID for a helmet
WORD HelmManager::GetHelmMaterial(Zeal::EqStructures::EQITEMINFO* item)
{
    if (!item || item->Type != 0)
        return 0;

    if (item->Common.Material)
        return item->Common.Material;

    if (item->IDFile[0] == 'I' && item->IDFile[1] == 'T' && item->IDFile[2] != '\0')
    {
        int material = atoi(&item->IDFile[2]);
        if (material >= kMaterialVeliousHelm) {
            return material;
        }
    }
    return 0;
}

void HelmManager::RefreshHelm(bool local_only, int delay_ms)
{
    if (!refresh_helm_pending)
    {
        refresh_helm_pending = true;
        ZealService::get_instance()->callbacks->AddDelayed([this, local_only]() { RefreshHelmCallback(local_only); }, delay_ms);
    }
}

void HelmManager::RefreshHelmCallback(bool local_only)
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    if (!self || !char_info || !refresh_helm_pending)
        return;

    refresh_helm_pending = false;

    if (self->Texture != 0xFF)
        return;

    WORD old_material = self->EquipmentMaterialType[kMaterialSlotHead];
    WORD material = kMaterialNone;
    DWORD color = kColorNone;

    if (ShowHelmEnabled.get() && char_info->Inventory.Head)
    {
        material = GetHelmMaterial(char_info->Inventory.Head);
        color = char_info->Inventory.Head->Common.Color;
    }

    if (HELM_MANAGER_LOG_DEBUG)
        Zeal::EqGame::print_chat("[HelmManager] [RefreshHelm] Current helm is %u color %08x", material, color);

    block_outbound_wearchange++;
    HandleSwapHead(Zeal::EqGame::get_display(), self, material, old_material, color, true);
    block_outbound_wearchange--;

    if (!local_only)
    {
        Zeal::Packets::WearChange_Struct wc;
        memset(&wc, 0, sizeof(Zeal::Packets::WearChange_Struct));
        wc.spawn_id = self->SpawnId;
        wc.wear_slot_id = kMaterialSlotHead;
        wc.material = material;
        wc.color.Color = color;
        Zeal::EqGame::send_message(Zeal::Packets::WearChange, (int*)&wc, sizeof(Zeal::Packets::WearChange_Struct), 0);
    }
}

void HelmManager::OnZone()
{
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    if (!char_info || !Zeal::EqGame::is_in_game())
    {
        ZealService::get_instance()->callbacks->AddDelayed([this]() { OnZone(); }, 1000);
        return;
    }

    std::string name = char_info->Name;
    if (name != last_seen_character)
    {
        last_seen_character = name;
        SyncShowHelm(true);
    }
}

void HelmManager::SyncShowHelm(bool hide_response)
{
    Zeal::EqGame::do_say(true, "#showhelm %s%s", ShowHelmEnabled.get() ? "on" : "off", hide_response ? " silent" : "");
}

HelmManager::HelmManager(ZealService* zeal)
{
    zeal->hooks->Add("SwapHead", 0x004a1735, SwapHead_hk, hook_type_detour);
    zeal->hooks->Add("WearChangeArmor", 0x004A2A7A, WearChangeArmor_hk, hook_type_detour);

    zeal->callbacks->AddGeneric([this]() { OnZone(); }, callback_type::Zone);

    zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
        if (opcode == Zeal::Packets::WearChange && len >= sizeof(Zeal::Packets::WearChange_Struct))
            return Handle_Out_OP_WearChange((Zeal::Packets::WearChange_Struct*)buffer);
        return false; // continue processing
        }, callback_type::SendMessage_);
}

HelmManager::~HelmManager() {
}