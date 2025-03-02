#include "helm_manager.h"
#include "string_util.h"
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"

constexpr BYTE kMaterialSlotHead = 0;
constexpr BYTE kMaterialSlotPrimary = 7;
constexpr BYTE kMaterialSlotSecondary = 8; // Shared with 'ranged'

constexpr WORD kMaterialNone = 0;
constexpr WORD kMaterialLeather = 1;
constexpr WORD kMaterialChain = 2;
constexpr WORD kMaterialPlate = 3;
constexpr WORD kMaterialVeliousHelm = 240;
constexpr WORD kMaterialVeliousHelmAlternate = 241; // A couple races support alternate helms

// We install 'Bald' variation of heads with ID 05: "{racetag}HE05_DMSPRITEDEF". We swap to this head only when wearing a Velious Helm, to ensure hair clipping is fixed.
constexpr WORD kMaterialBaldHead = 5;
constexpr char kHumanFemaleBaldHead[] = "HUFHE05_DMSPRITEDEF";
constexpr char kBarbarianFemaleBaldHead[] = "BAFHE05_DMSPRITEDEF";
constexpr char kEruditeFemaleBaldHead[] = "ERFHE05_DMSPRITEDEF";
constexpr char kEruditeMaleBaldHead[] = "ERMHE05_DMSPRITEDEF";
constexpr char kWoodElfFemaleBaldHead[] = "ELFHE05_DMSPRITEDEF";
constexpr char kDarkElfFemaleBaldHead[] = "DAFHE05_DMSPRITEDEF";

constexpr DWORD kColorNone = 0;
constexpr DWORD kColorDefault = 0x00FFFFFF;

// Enables debug logging
constexpr bool HELM_MANAGER_LOG_DEBUG = false;

// Called when an DAG-attached model is changing for a slot (weapons, shields, velious helmets)
int __fastcall SwapModel_hk(int* cdisplay, int unused, Zeal::EqStructures::Entity* entity, BYTE wear_slot, char* ITstr, bool from_server)
{
    return ZealService::get_instance()->helm->HandleSwapModel(cdisplay, entity, wear_slot, ITstr, from_server);
}

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

void SaveMaterialColor(Zeal::EqStructures::Entity* entity, int wear_slot, DWORD color)
{
    if (wear_slot >= kMaterialSlotHead && wear_slot <= kMaterialSlotSecondary)
        entity->EquipmentMaterialColor[wear_slot] = color;
}

// This is the main hook that fixes pretty much all of the weird helmet behaviors, as noted in the comments below.
int HelmManager::HandleSwapHead(int* cDisplay, Zeal::EqStructures::Entity* entity, int new_material, int old_material_or_head, DWORD color, bool from_server)
{
    bool use_bald_head = false; // On races with buggy Velious helms, we will try to use the bald head underneath the helm to fix clipping (see 3a).
    bool playable_race = Zeal::EqGame::IsPlayableRace(entity->Race); // Most logic only needs to apply on playable races.

    if (playable_race)
    {
        // (1) Fixes the old head from getting desync'd/stuck. We can manually detect the current head which ensures that SwapHead always works (requires the old head value to be accurate).
        old_material_or_head = GetHeadID(entity, old_material_or_head);

        // (2) Respect the ShowHelm toggle.
        if (entity == Zeal::EqGame::get_self() && !ShowHelmEnabled.get())
        {
            new_material = kMaterialNone;
            color = kColorNone;
        }

        // (3a) Fix broken Velious races by using a special head with no hair/hood graphics underneath their helmet. This stops their hair/hood clipping through the helm.
        use_bald_head = new_material >= kMaterialVeliousHelm && IsPatchedOldModel(entity->Race, entity->Gender);
        if (use_bald_head)
        {
            mem::write(0x4A1C65 + 1, (BYTE)kMaterialBaldHead); // Changes default head 0 -> 5
        }

        // (4) Fixes a bug that double-sends packets on materials below 240. Suppresses the extra packet (which also contains the wrong value).
        if (new_material < kMaterialVeliousHelm || block_outbound_wearchange > 0)
        {
            from_server = true; // This flag only controls whether the client generates a WearChange packet after swapping gear (true = local only, false = send packet)
        }

        // (5) Save the helmet color for tinting. SwapHead() calls SwapModel(), where we apply the tint. But the default SwapHead() saves the color too late, after calling SwapModel().
        SaveMaterialColor(entity, kMaterialSlotHead, color);
    }

    // Call SwapHead()
    int result = ZealService::get_instance()->hooks->hook_map["SwapHead"]->original(SwapHead_hk)(cDisplay, 0, entity, new_material, old_material_or_head, color, from_server);

    // (6) Fixes SwapHead() to save material values correctly when material >255. The original method only sets the lo-byte, but the storage supports uint16.
    entity->EquipmentMaterialType[kMaterialSlotHead] = new_material;

    // (3b) Unpatch the Change from (3a)
    if (use_bald_head)
    {
        mem::write(0x4A1C65 + 1, (BYTE)kMaterialNone); // Changes default head 5 -> 0
    }

    return result;
}

// Fixes tinting on Velious helmets. Supports Weapon tints too.
int HelmManager::HandleSwapModel(int* cdisplay, Zeal::EqStructures::Entity* entity, BYTE wear_slot, char* ITstr, bool from_server)
{
    int material = ITstr && strlen(ITstr) > 2 ? atoi(&ITstr[2]) : 0;
    bool is_weapon_slot = wear_slot == kMaterialSlotPrimary || wear_slot == kMaterialSlotSecondary;
    bool is_tint_slot = is_weapon_slot || (wear_slot == kMaterialSlotHead && Zeal::EqGame::IsPlayableRace(entity->Race));

    if (!from_server && is_weapon_slot && entity == Zeal::EqGame::get_self() && Zeal::EqGame::get_char_info())
    {
        // This is reached when an item was swapped by the user equipping a new item through the UI.
        // We aren't given the color of the item in this scenario, so we have to look it up by matching the IT# to the equipped item in primary/secondary/range slot.
        // In all other Scenarios, we already know the color because we got an OP_WearChange event, so we can skip this logic.
        Zeal::EqStructures::EQINVENTORY& inv = Zeal::EqGame::get_char_info()->Inventory;
        if (material == kMaterialNone)
            SaveMaterialColor(entity, wear_slot, kColorNone);
        else if (wear_slot == kMaterialSlotPrimary)
            SaveMaterialColor(entity, wear_slot, inv.Primary ? inv.Primary->Common.Color : kColorNone);
        else if (HelmManager::GetItemMaterial(inv.Secondary) == material)
            SaveMaterialColor(entity, wear_slot, inv.Secondary ? inv.Secondary->Common.Color : kColorNone);
        else if (HelmManager::GetItemMaterial(inv.Ranged) == material)
            SaveMaterialColor(entity, wear_slot, inv.Ranged ? inv.Ranged->Common.Color : kColorNone);
    }

    // Swaps the model
    int result = ZealService::get_instance()->hooks->hook_map["SwapModel"]->original(SwapModel_hk)(cdisplay, 0, entity, wear_slot, ITstr, from_server);

    // After models are swapped, apply tint to the Helm and Weapon slots.
    if (material > kMaterialNone && is_tint_slot)
    {
        DWORD color = entity->EquipmentMaterialColor[wear_slot];
        DWORD tint = color == kColorNone ? kColorDefault : color;
        Zeal::EqStructures::EQDAGINFO* dag = Zeal::EqGame::get_dag(entity, wear_slot);
        if (dag)
            Zeal::EqGame::set_dag_sprite_tint(dag, tint);
        if (wear_slot == kMaterialSlotSecondary && entity->ActorInfo && entity->ActorInfo->DagShieldPoint) // Also tint shields
            Zeal::EqGame::set_dag_sprite_tint(entity->ActorInfo->DagShieldPoint, tint);
    }

    return result;
}

// Callback when Server (local_only=true) or UI (local_only=false) is changing our armor texture
void HelmManager::HandleWearChangeArmor(int* cDisplay, Zeal::EqStructures::Entity* entity, int wear_slot, WORD new_material, WORD old_material, DWORD colors, bool from_server)
{
    int block_wearchange = 0;
    if (from_server && entity == Zeal::EqGame::get_self() && Zeal::EqGame::IsPlayableRace(entity->Race))
    {
        // Inbound WearChanges from the server shouldn't generate a response. However, the client doesn't respect 'from_server' flag on some helmets and replies anyway.
        block_wearchange = 1;

        // Update tint cache (This line is needed for character select scren tints)
        SaveMaterialColor(entity, wear_slot, colors);

        // Prevent any showhelm violation.
        if (wear_slot == kMaterialSlotHead && new_material != kMaterialNone && !ShowHelmEnabled.get())
        {
            if (HELM_MANAGER_LOG_DEBUG)
                Zeal::EqGame::print_chat("[HelmManager] [WearChange] Got inbound helm while our helm is hidden. Refreshing our helm to hide.");
            RefreshHelm(false, 0);
        }
    }
    block_outbound_wearchange += block_wearchange;
    ZealService::get_instance()->hooks->hook_map["WearChangeArmor"]->original(WearChangeArmor_hk)(cDisplay, 0, entity, wear_slot, new_material, old_material, colors, from_server);
    block_outbound_wearchange -= block_wearchange;
}

// Callback for server WearChange packets. It immediately calls HandleWearChangeArmor for non-weapon slots.
bool HelmManager::Handle_In_OP_WearChange(Zeal::Packets::WearChange_Struct* wc)
{
    if (!wc)
        return false;

    Zeal::EqStructures::Entity* entity = Zeal::EqGame::get_entity_by_id(wc->spawn_id);
    if (!entity)
        return false;

    // Weapon color is not passed from OP_WearChange to any called method, so we have to save it from here.
    if (wc->wear_slot_id == kMaterialSlotPrimary || wc->wear_slot_id == kMaterialSlotSecondary)
        SaveMaterialColor(entity, wc->wear_slot_id, wc->color.Color);

    if (HELM_MANAGER_LOG_DEBUG)
        Zeal::EqGame::print_chat("[HelmManager] [OP_WearChange] RECV WearChange from %s Slot=%u Material=%u Color=0x%08x", entity->Name, wc->wear_slot_id, wc->material, wc->color.Color);

    return false; // Continue processing the OP_WearChange. Either calls WearChange_Armor or WearChange_Model hooks.
}

// Process outbound helm WearChanges to make sure the right material value is sent out
bool HelmManager::Handle_Out_OP_WearChange(Zeal::Packets::WearChange_Struct* wc)
{
    if (!wc)
        return false;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self)
        return false;

    if (wc->wear_slot_id == kMaterialSlotHead)
    {
        if (block_outbound_wearchange > 0)
            return true; // Stop processing this OP_WearChange, preventing the message from being sent.

        // Respect ShowHelm toggle
        if (Zeal::EqGame::IsPlayableRace(self->Race))
        {
            if (ShowHelmEnabled.get())
            {
                wc->material = ToCanonicalHelmMaterial(wc->material, self->Race); // Uses the proper outbound helm ID (may be unecessary now)
                wc->color.Color = self->EquipmentMaterialColor[kMaterialSlotHead];
            }
            else
            {
                wc->material = kMaterialNone;
                wc->color.Color = kColorNone;
            }
        }
    }
    else if (wc->wear_slot_id == kMaterialSlotPrimary || wc->wear_slot_id == kMaterialSlotSecondary)
    {
        // Fixes outbound weapons to include the current tint
        wc->color.Color = self->EquipmentMaterialColor[wc->wear_slot_id];
    }

    if (HELM_MANAGER_LOG_DEBUG)
        Zeal::EqGame::print_chat("[HelmManager] [OP_WearChange] SEND WearChange Slot=%u Material=%u Color=0x%08x", wc->wear_slot_id, wc->material, wc->color.Color);

    return false; // Continue processing this OP_WearChange, sending the message.
}

// Detect the Head ID from our current in-game model (0=default, 1=leather, etc...)
int HelmManager::GetHeadID(Zeal::EqStructures::Entity* entity, int default_value)
{
    if (!entity || !entity->ActorInfo || !entity->ActorInfo->ModelInfo)
        return default_value;

    char tag[32];
    int num_skins = Zeal::EqGame::Graphics::s3dGetNumSkinsAttachedToHierarchicalSprite(entity->ActorInfo->ModelInfo);
    for (int i = 0; i < num_skins; i++)
    {
        auto* sprite = Zeal::EqGame::Graphics::s3dGetSkinAttachedToHierarchicalSprite(i, entity->ActorInfo->ModelInfo);
        if (sprite && sprite->Type == Zeal::EqStructures::DMSprite::kType)
        {
            auto* sprite_definition = Zeal::EqGame::Graphics::s3dGetDMSpriteDefinition(sprite);
            if (sprite_definition)
            {
                Zeal::EqGame::Graphics::t3dGetObjectTag(sprite_definition, tag);
                if (strlen(tag) > 7 && tag[3] == 'H' && tag[4] == 'E' && isdigit(tag[5]) && isdigit(tag[6]) && tag[7] == '_') // e.g. "ELFHE01_DMSPRITEDEF"
                {
                    tag[7] = '\0';
                    int head = atoi(&tag[5]);
                    return head;
                }
            }
        }
    }
    return default_value;
}

// Helper function. Converts Velious Helms to their common values.
// We only send the canonical values to the server and other players.
// - [5xx/6xx] -> [240] Swaps our racial Velious head model IT### back to the generic '240' value used by all Velious helms.
WORD HelmManager::ToCanonicalHelmMaterial(WORD material, WORD race)
{
    if (Zeal::EqGame::IsPlayableRace(race))
    {
        switch (material)
        {
        // Vah Shir have their own material IDs when they equip leather/chain/plate helms, but no custom helm.
        case 661: // VAH (F) Leather Helm
        case 666: // VAH (M) Leather Helm
            return kMaterialLeather; // (1)
        case 662: // VAH (F) Chain Helm
        case 667: // VAH (M) Chain Helm
            return kMaterialChain; // (2)
        case 663: // VAH (F) Plate Helm
        case 668: // VAH (M) Plate Helm
            return kMaterialPlate; // (3)

        // Converts all the race-specific Velious Helm IT### numbers to the common 240 value
        case 665: // vah
        case 660: // vah
        case 627: // hum
        case 620: // hum
        case 537: // bar
        case 530: // bar
        case 570: // eru
        case 575: // eru
        case 565: // elf
        case 561: // elf
        case 605: // hie
        case 600: // hie
        case 545: // def
        case 540: // def
        case 595: // hef
        case 590: // hef
        case 557: // dwf
        case 550: // dwf
        case 655: // trl
        case 650: // trl
        case 645: // ogr
        case 640: // ogr
        case 615: // hlf
        case 610: // hlf
        case 585: // gnm
        case 580: // gnm
        case 635: // iks
        case 630: // iks
            if (race == 130) // vah
                return kMaterialPlate;
            return kMaterialVeliousHelm;
        case 641: // OGR (F) Alternate Helm (Barbarian/RZ look)
        case 646: // OGR (M) Alternate Helm (Barbarian/RZ look)
            if (race == 10) // ogre
                return kMaterialVeliousHelmAlternate;
            return kMaterialVeliousHelm;
        }
    }
    return material;
}

// Detects whether the given race/gender model needs the special material logic applied.
// Checks whether you have the Luclin models enabled or not.
bool HelmManager::IsPatchedOldModel(WORD race, BYTE gender)
{
    if (!DetectInstalledFixesComplete)
    {
        DetectInstalledFixes(); // Have to lazy-load this here because this is reached before OnZone() is called during initial login.
    }

    // Bugged male races
    if (gender == 0)
    {
        // Erudite
        return race == 3 ? UseEruditeMaleFix : false;
    }

    // Bugged female races
    if (gender == 1)
    {
        switch (race)
        {
        case 1: // Human
            return UseHumanFemaleFix;
        case 2: // Barbarian
            return UseBarbarianFemaleFix;
        case 3: // Erudite
            return UseEruditeFemaleFix;
        case 4: // Wood Elf
            return UseWoodElfFemaleFix;
        case 6: // Dark Elf
            return UseDarkElfFemaleFix;
        }
    }

    return false;
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

// Gets the material/model ID from an item.
WORD HelmManager::GetItemMaterial(Zeal::EqStructures::EQITEMINFO* item)
{
    if (!item || item->Type != 0)
        return 0;

    if (item->Common.Material)
        return item->Common.Material;

    if (item->IDFile[0] == 'I' && item->IDFile[1] == 'T' && item->IDFile[2] != '\0')
        return atoi(&item->IDFile[2]);

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

    if (!Zeal::EqGame::IsPlayableRace(self->Race))
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

    // SwapHead isn't sending the right wearchange via this call. We'll do it ourselves after.
    block_outbound_wearchange++;
    HandleSwapHead(Zeal::EqGame::get_display(), self, material, old_material, color, true);
    block_outbound_wearchange--;

    if (!local_only)
    {
        material = ToCanonicalHelmMaterial(material, self->Race);
        if (HELM_MANAGER_LOG_DEBUG)
            Zeal::EqGame::print_chat("[HelmManager] [RefreshHelm] Manually sending WearChange %u color %08x", material, color);
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
    DetectInstalledFixes();

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

bool HelmManager::DetectInstalledFixes()
{
    if (DetectInstalledFixesComplete)
        return true;

    if (!Zeal::EqGame::Graphics::IsWorldInitialized())
        return false;

    auto* sprite_definition = Zeal::EqGame::Graphics::t3dGetPointerFromDictionary("ELFHE00_DMSPRITEDEF"); // Check a known-good value to ensure assets are ready
    if (!sprite_definition)
        return false;

    IO_ini ini(IO_ini::kClientFilename);
    bool AllLuclinPcModelsOff = ini.getValue<bool>("Defaults", "AllLuclinPcModelsOff");
    bool UseLuclinHumanFemale = ini.getValue<bool>("Defaults", "UseLuclinHumanFemale");
    bool UseLuclinBarbarianFemale = ini.getValue<bool>("Defaults", "UseLuclinBarbarianFemale");
    bool UseLuclinEruditeFemale = ini.getValue<bool>("Defaults", "UseLuclinEruditeFemale");
    bool UseLuclinEruditeMale = ini.getValue<bool>("Defaults", "UseLuclinEruditeMale");
    bool UseLuclinWoodElfFemale = ini.getValue<bool>("Defaults", "UseLuclinWoodElfFemale");
    bool UseLuclinDarkElfFemale = ini.getValue<bool>("Defaults", "UseLuclinDarkElfFemale");
    UseHumanFemaleFix     = (AllLuclinPcModelsOff || !UseLuclinHumanFemale)     && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kHumanFemaleBaldHead);
    UseBarbarianFemaleFix = (AllLuclinPcModelsOff || !UseLuclinBarbarianFemale) && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kBarbarianFemaleBaldHead);
    UseEruditeFemaleFix   = (AllLuclinPcModelsOff || !UseLuclinEruditeFemale)   && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kEruditeFemaleBaldHead);
    UseEruditeMaleFix     = (AllLuclinPcModelsOff || !UseLuclinEruditeMale)     && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kEruditeMaleBaldHead);
    UseWoodElfFemaleFix   = (AllLuclinPcModelsOff || !UseLuclinWoodElfFemale)   && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kWoodElfFemaleBaldHead);
    UseDarkElfFemaleFix   = (AllLuclinPcModelsOff || !UseLuclinDarkElfFemale)   && Zeal::EqGame::Graphics::t3dGetPointerFromDictionary(kDarkElfFemaleBaldHead);

    if (HELM_MANAGER_LOG_DEBUG)
        Zeal::EqGame::print_chat("[HelmManager] [DetectInstalledFixes] Complete");

    DetectInstalledFixesComplete = true;
    return true;
}

HelmManager::HelmManager(ZealService* zeal)
{
    zeal->hooks->Add("SwapHead", 0x004a1735, SwapHead_hk, hook_type_detour);
    zeal->hooks->Add("SwapModel", 0x4A9EB3, SwapModel_hk, hook_type_detour);
    zeal->hooks->Add("WearChangeArmor", 0x004A2A7A, WearChangeArmor_hk, hook_type_detour);

    zeal->callbacks->AddGeneric([this]() { OnZone(); }, callback_type::Zone);

    zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
        if (opcode == Zeal::Packets::WearChange && len >= sizeof(Zeal::Packets::WearChange_Struct))
            return Handle_Out_OP_WearChange((Zeal::Packets::WearChange_Struct*)buffer);
        return false; // continue processing
        }, callback_type::SendMessage_);

    zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
        if (opcode == Zeal::Packets::WearChange && len >= sizeof(Zeal::Packets::WearChange_Struct))
            return Handle_In_OP_WearChange((Zeal::Packets::WearChange_Struct*)buffer);
        return false; // continue processing
        }, callback_type::WorldMessage);

    // 0x4A1512 - GetVeliousHelmMaterialIT(entity, material, *show_hair)
    // - (1) Disables hair becoming invisible on the shared default head. Prevents "show_hair = false" happening with Velious helms.
    mem::fill_with_nop(0x4A159B, 0x4A159D); // '*show_hair = 0' -> No-OP
    mem::fill_with_nop(0x4A16D2, 0x4A16D4); // '*show_hair = 0' -> No-OP
    // - (2) Enables Velious Helms showing on Character Select. Prevents returning early if char_info is null.
    mem::fill_with_nop(0x4A152B, 0x4A1533); // 'if (!char_info) return 3'; -> No-OP
    mem::fill_with_nop(0x4A153E, 0x4A154B); // 'if ((char_info->Unknown0D3C & 0x1E) == 0) return 3;' -> No-OP

    // ChangeDag()
    // Unlocks proper tinting for IT# model helms/weapons below IT# number 1000:
    // - IT# models under ID 1000 used shared memory their tint storage, so setting the tint on one model affected all models in the zone.
    mem::write(0x4B094E + 3, (DWORD)1); // 'if IT_num >= 1000' changed to 'if IT_num >= 1'

    if (HELM_MANAGER_LOG_DEBUG)
    {
        zeal->commands_hook->Add("/helmconfig", {}, "Prints/Toggles HelmManager configuration (for testing).", [this](std::vector<std::string>& args) {
            if (args.size() > 1) {
                int p;
                if (Zeal::String::tryParse(args[1], &p, true)) {
                    switch (p) {
                    case 1:
                        ShowHelmEnabled.toggle();
                        break;
                    case 2:
                        UseHumanFemaleFix = !UseHumanFemaleFix;
                        break;
                    case 3:
                        UseBarbarianFemaleFix = !UseBarbarianFemaleFix;
                        break;
                    case 4:
                        UseEruditeFemaleFix = !UseEruditeFemaleFix;
                        break;
                    case 5:
                        UseEruditeMaleFix = !UseEruditeMaleFix;
                        break;
                    case 6:
                        UseWoodElfFemaleFix = !UseWoodElfFemaleFix;
                        break;
                    case 7:
                        UseDarkElfFemaleFix = !UseDarkElfFemaleFix;
                        break;
                    }
                }
            }
            Zeal::EqGame::print_chat("[1] ShowHelm: %s", ShowHelmEnabled.get() ? "on" : "off");
            Zeal::EqGame::print_chat("[2] Fix Human (F): %s", UseHumanFemaleFix ? "on" : "off");
            Zeal::EqGame::print_chat("[3] Fix Barbarian (F): %s", UseBarbarianFemaleFix ? "on" : "off");
            Zeal::EqGame::print_chat("[4] Fix Erudite (F): %s", UseEruditeFemaleFix ? "on" : "off");
            Zeal::EqGame::print_chat("[5] Fix Erudite (M): %s", UseEruditeMaleFix ? "on" : "off");
            Zeal::EqGame::print_chat("[6] Fix WoodElf (F): %s", UseWoodElfFemaleFix ? "on" : "off");
            Zeal::EqGame::print_chat("[7] Fix DarkElf (F): %s", UseDarkElfFemaleFix ? "on" : "off");
            return true;
            });
    }
}

HelmManager::~HelmManager() {
}