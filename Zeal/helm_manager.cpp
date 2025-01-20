#include "helm_manager.h"
#include "string_util.h"
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"

constexpr BYTE kMaterialSlotHead = 0;

constexpr WORD kMaterialNone = 0;
constexpr WORD kMaterialLeather = 1;
constexpr WORD kMaterialChain = 2;
constexpr WORD kMaterialPlate = 3;
constexpr WORD kMaterialVeliousHelm = 240;
constexpr WORD kMaterialVeliousHelmOgreRZ = 241;

// For race/gender combinations that have bugged Velious helms, we've patch in a head (4) material that we use whenever they would normally show the default (0) head.
// If the patch installed, we used material (4) whenever they are showing the default head, rather than material (0).
constexpr WORD kMaterialDefaultHeadOverride = 4;

// Whether or not we trust the server to send us the correct helmet after illusions and stuff.
// If we don't trust the server, the HelmManager will do the proper helm fixing/showing after an illusion is applied.
constexpr bool TRUST_SERVER_TO_FIX_HELMS_AFTER_ILLUSION = true;

// Enables debug logging
constexpr bool HELM_MANAGER_LOG_DEBUG = false;

// The internal hook name for CDisplay::SwapHead
static const std::string SwapHeadHook = "CDisplaySwapHead";

// Callback any time the head is changing
int __fastcall SwapHead_hk(int cDisplay, int unused_edx, Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, int local_only) {
    return ZealService::get_instance()->helm->HandleSwapHead(cDisplay, entity, new_material, old_material, color, local_only);
}

// Callback when we get illusioned
int __fastcall EntityChangeForm_hk(Zeal::EqStructures::Entity* entity, int unused_edx, Zeal::Packets::Illusion_Struct* illusion) {
    return ZealService::get_instance()->helm->HandleIllusion(entity, illusion);
}

// Callback when Server (local_only=true) or UI (local_only=false) is changing our armor appearance
void __fastcall WearChangeArmor_hk(int cDisplay, int unused_edx, Zeal::EqStructures::Entity* spawn, int wear_slot, WORD new_material, WORD old_material, DWORD colors, bool local_only) {
    ZealService::get_instance()->helm->HandleWearChangeArmor(cDisplay, spawn, wear_slot, new_material, old_material, colors, local_only);
}

// Helper Function
int SwapHeadOriginal(Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, int local_only) {
    return ZealService::get_instance()->hooks->hook_map[SwapHeadHook]->original(SwapHead_hk)(*(int*)Zeal::EqGame::Display, 0, entity, new_material, old_material, color, local_only);
}

// Helper function
int SwapHead(Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, int local_only) {
    return SwapHead_hk(*(int*)Zeal::EqGame::Display, 0, entity, new_material, old_material, color, local_only);
}

// This is the main hook that handles 80% of fixing Velious Helms. For context, the issue with Velious helms is the client had buggy logic
// that was not able to correctly hide some races' hair from clipping through the helmets. Or when it did manage to hide the hair from clipping,
// it also hides the hair on all characters with the same face in the zone, even if they didn't have helms on, making their heads become see-through.
// 
// The key elements to the Velious helm fix are:
// - Uses the raw IT### number for Velious Helms, which bypasses the logic that flags hair meshes to become transparent.
// - We also have a custom s3d where the base head (material 0) is patched to not have hair at all. Instead we use head/material (4) when they wear nothing.
// - The base head (material 0) ends up only being used while wearing a Velious Helm, so that their hair doesn't clip through, because we deleted all the hair.
int HelmManager::HandleSwapHead(int cDisplay, Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, int local_only)
{
    ZealService* zeal = ZealService::get_instance();

    // (1) Respect the ShowHelm toggle:
    if (entity == Zeal::EqGame::get_self()) {
        if (HELM_MANAGER_LOG_DEBUG) {
            Zeal::EqGame::print_chat("[HelmManager] [SwapHead_hk] RECV SwapHead (%i -> %i) %s Race:%u Gender:%u", old_material, new_material, entity->Name, entity->Race, entity->Gender);
        }
        if (!zeal->helm->ShowHelmEnabled.get() && Zeal::EqGame::IsPlayableRace(entity->Race)) {
            new_material = kMaterialNone;
        }
    }

    // (2) Fix broken Velious races by using their patched head model numbers:
    if (zeal->helm->IsHelmBuggedOldModel(entity->Race, entity->Gender)) {
        // For bugged races we convert the canonical material to the special ones:
        // - Convert the generic Velious Helm Material (240), to their racial IT### model number.
        // - Convert the default Head Material (0) to a custom Head Material (4). We use (4) as the default head for these bugged races.
        new_material = HelmManager::ToRacialHelmMaterial(new_material, entity->Race, entity->Gender);

        // The 'old_material' param MUST match the current head state, or the head will become locked to its current form.
        // EQ passes in the Item->Material/IDFile value (e.g. '0'), which is inconsistent with the head state when we're using head overrides, so we have to correct for this,
        if (entity->EquipmentMaterialType[kMaterialSlotHead] == kMaterialDefaultHeadOverride) {
            old_material = kMaterialDefaultHeadOverride;
        }
    }

    // (3) Fix a packet bug sending wrong values:
    // - When equipping a non-Velious Helm when local_only=false, this causes a double-send with a wrong value in the first packet. Suppressing this causes it to single-send the correct value only.
    // - But we don't want to suppress Velious Helms, as those do no exhibit the double-send bug, and it would instead cause nothing to be sent.
    if (new_material < kMaterialVeliousHelm || block_outbound_wearchange > 0) {
        local_only = true;
    }

    if (HELM_MANAGER_LOG_DEBUG && entity == Zeal::EqGame::get_self()) {
        Zeal::EqGame::print_chat("[HelmManager] [SwapHead_hk] APPLY SwapHead (%i -> %i) %s Race:%u Gender:%u Silent:%s", old_material, new_material, entity->Name, entity->Race, entity->Gender, local_only ? "true" : "false");
    }
    int result = zeal->hooks->hook_map[SwapHeadHook]->original(SwapHead_hk)(cDisplay, 0, entity, new_material, old_material, color, local_only);

    // (4) Fix CDisplay::SwapHead() overflow:
    // The original call only writes the lo-byte of the 'new_material' value to entity->EquipmentMaterialType[0] in some code paths.
    // Because of our new logic, we can overflow this when custom IT### numbers are introduced (in the 500-600).
    // Simply overwriting this value after the call works fine.
    entity->EquipmentMaterialType[kMaterialSlotHead] = new_material;

    return result;
}

// Illusions are the other place where we need to clean up the material numbers and fix our helm during an illusion.
int HelmManager::HandleIllusion(Zeal::EqStructures::Entity* entity, Zeal::Packets::Illusion_Struct* is) {

    if (HELM_MANAGER_LOG_DEBUG) {
        Zeal::EqGame::print_chat("[HelmManager] [Illusion] RECV race=%i gender=%u head=%u (hi=%u)", is->race, is->gender, is->helmtexture, is->unknown007);
    }

    auto* self = Zeal::EqGame::get_self();
    last_wc = { 0 }; // Reset any cached helm state since we're swapping.

    bool refresh_helm = false;
    WORD set_helm_material_before_illusion = 0xFFFF;
    WORD set_helm_material_after_illusion = 0xFFFF;
    WORD color = 0;
    WORD old_material = entity->EquipmentMaterialType[kMaterialSlotHead]; // '4' or some Velious ID are the ones we need to clean up
    WORD old_material_generic = ToCanonicalHelmID(old_material, entity->Race); // 0 or 240

    // (1) Cleanup any custom material values prior to illusion, so we don't leak them to the new illusion.
    if (IsHelmBuggedOldModel(entity->Race, entity->Gender)) {
        if (old_material == kMaterialDefaultHeadOverride || old_material_generic >= kMaterialVeliousHelm) {
            set_helm_material_before_illusion = kMaterialNone;
        }
    }

    // (2) Let's do the logic to figure out our helmet after the illusion
    if (Zeal::EqGame::IsPlayableRace(is->race)) {
        if (entity == self && is->helmtexture == 0 && !TRUST_SERVER_TO_FIX_HELMS_AFTER_ILLUSION)
        {
            // If we don't trust the server, just always refresh our helm after illusion.
            refresh_helm = true;
        }
        else if (is->helmtexture != 0xFF)
        {
            // If the server gives us a non-0xFF helm texture to use, just trust that and apply it.
            set_helm_material_after_illusion = is->helmtexture;
        }
        else if (entity == self)
        {
            // Server gave us 0xFF helm texture ("use current" instruction).
            // Since the target is us, we can refresh our helm by refreshing it using our equipment data.
            refresh_helm = true;
        }
        else
        {
            // Server gave us 0xFF helm texture for some other player.
            // Carry over their current texture/color.
            set_helm_material_after_illusion = old_material_generic;
            color = entity->EquipmentMaterialColor[kMaterialSlotHead];
        }
    }

    block_outbound_wearchange++;

    // (3) Cleanup head state before illusion
    if (set_helm_material_before_illusion != 0xFFFF) {
        if (HELM_MANAGER_LOG_DEBUG) {
            Zeal::EqGame::print_chat("[HelmManager] [Illusion] PRE-ILLUSION SwapHead (%i -> %i) %s Race:%u Gender:%u", old_material, set_helm_material_before_illusion, entity->Name, entity->Race, entity->Gender);
        }
        // Calling directly to the original function here because we want to bypass our Velious-fix logic, to actually set their material to potentially 0 (not 4).        
        SwapHeadOriginal(entity, set_helm_material_before_illusion, old_material, 0, true);
        entity->EquipmentMaterialType[kMaterialSlotHead] = set_helm_material_before_illusion;
    }

    // (4) Process the illusion change
    int result = ZealService::get_instance()->hooks->hook_map["EntityChangeForm"]->original(EntityChangeForm_hk)(entity, 0, is);

    // (5) Setup the head state after the illusion
    if (refresh_helm)
    {
        refresh_helm_pending = true;
        RefreshHelmCallback(true);
    }
    else if (set_helm_material_after_illusion != 0xFFFF)
    {
        if (HELM_MANAGER_LOG_DEBUG) {
            Zeal::EqGame::print_chat("[HelmManager] [Illusion] POST-ILLUSION SwapHead (%i -> %i) %s Race:%u Gender:%u", kMaterialNone, set_helm_material_after_illusion, entity->Name, entity->Race, entity->Gender);
        }
        // Call our Velious-hook after the illusion to setup their new head
        SwapHead(entity, set_helm_material_after_illusion, kMaterialNone, color, true);
    }

    block_outbound_wearchange--;
    return result;
}

// Callback when Server (local_only=true) or UI (local_only=false) is changing our armor appearance
void HelmManager::HandleWearChangeArmor(int cDisplay, Zeal::EqStructures::Entity* spawn, int wear_slot, WORD new_material, WORD old_material, DWORD colors, int local_only) {
    int block_wearchange = 0;
    if (wear_slot == kMaterialSlotHead && local_only && spawn == Zeal::EqGame::get_self()) {
        // Check for any showhelm violation.
        if (new_material != kMaterialNone && !ShowHelmEnabled.get()) {
            if (HELM_MANAGER_LOG_DEBUG) {
                Zeal::EqGame::print_chat("[HelmManager] [WearChange] Got inbound helm while our helm is hidden, hiding our helm.");
            }
            RefreshHelm(false, 0);
        }
        // Inbound WearChanges from the server shouldn't generate a response. However, the client doesn't respect 'local_only' flag on some helmets.
        block_wearchange = 1;
    }
    block_outbound_wearchange += block_wearchange;
    ZealService::get_instance()->hooks->hook_map["WearChangeArmor"]->original(WearChangeArmor_hk)(cDisplay, 0, spawn, wear_slot, new_material, old_material, colors, local_only);
    block_outbound_wearchange -= block_wearchange;
}

// Callback for server WearChange packets. It immediately calls HandleWearChangeArmor after for non-weapon slots.
bool HelmManager::Handle_In_OP_WearChange(Zeal::Packets::WearChange_Struct* wc) {
    if (wc && wc->wear_slot_id == kMaterialSlotHead) {
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
        if (self && wc->spawn_id == self->SpawnId) {
            last_wc = *wc;
        }
        if (HELM_MANAGER_LOG_DEBUG) {
            Zeal::EqStructures::Entity* spawn = (self && self->SpawnId == wc->spawn_id) ? self : Zeal::EqGame::get_entity_by_id(wc->spawn_id);
            Zeal::EqGame::print_chat("[HelmManager] [OP_WearChange] RECV WearChange from %s Material=%u Color=0x%08x", spawn ? spawn->Name : "Unknown", wc->material, wc->color.Color);
        }
    }
    return false; // Continue processing the OP_WearChange.
}

// Process outbound helm WearChanges to make sure the right material value is sent out
bool HelmManager::Handle_Out_OP_WearChange(Zeal::Packets::WearChange_Struct* wc) {
    if (wc && wc->wear_slot_id == kMaterialSlotHead) {
        if (block_outbound_wearchange > 0) {
            return true; // Stop processing this OP_WearChange, preventing the message from being sent.
        }
        auto* self = Zeal::EqGame::get_self();
        if (self) {
            // Ensure the ShowHelm toggle is respected.
            WORD material = ShowHelmEnabled.get() ? wc->material : kMaterialNone;
            // Always use the canonical material for server communication.
            wc->material = ToCanonicalHelmID(material, self->Race);
            // Remember the last thing we've sent out, can avoid duplicating if RefreshHelm() is pending.
            last_wc = *wc;
            if (HELM_MANAGER_LOG_DEBUG) {
                Zeal::EqGame::print_chat("[HelmManager] [OP_WearChange] SEND WearChange Material=%u Color=0x%08x", wc->material, wc->color.Color);
            }
        }
    }
    return false; // Continue processing this OP_WearChange, sending the message.
}

// Helper function. Converts our overrides back to their canonical values.
// We only send the canonical values to the server and other players.
// - [4] -> [0]         Swaps our custom head '4' back to the standard '0'
// - [5xx/6xx] -> [240] Swaps our racial Velious head model IT### back to the generic '240' value used by all Velious helms.
// - etc..
WORD HelmManager::ToCanonicalHelmID(WORD material, WORD race) {
    if (Zeal::EqGame::IsPlayableRace(race)) {
        switch (material) {

        // Converts our custom Zeal/Quarm Head ID back to the normal default head
        case kMaterialDefaultHeadOverride: // (4)
            return kMaterialNone; // (0)

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
            if (race == 130) { // vah
                return kMaterialPlate;
            }
            return kMaterialVeliousHelm;
        case 641: // OGR (F) Alternate Helm (Barbarian/RZ look)
        case 646: // OGR (M) Alternate Helm (Barbarian/RZ look)
            if (race == 10) {
                return kMaterialVeliousHelmOgreRZ;
            }
            return kMaterialVeliousHelm;
        }
    }
    return material;
}

// Only call this when IsHelmBuggedOldModel() is true.
// - Converts canonical helmet materials to their internal/racial overrides that we use for these fixes.
// - We only need to do this for the bugged race/gender combinations.
// - Normally the client is fine using the canonical helm ID (e.g. 240) for Velious helms.
//   But the bugged races we must set them via their racial IT### number to bypass the helm from triggering the hair transparency toggles.
//   If we don't do it, then hair becomes transparent for people without the helm on.
WORD HelmManager::ToRacialHelmMaterial(WORD material, WORD race, BYTE gender) {

    if (material == kMaterialNone) {
        // For bugged races, we swap their default head (0) to the patched override (4).
        switch (race) {
        case 1: // HUM
            return gender ? kMaterialDefaultHeadOverride : material;
        case 2: // BAR
            return gender ? kMaterialDefaultHeadOverride : material;
        case 3: // ERU
            return kMaterialDefaultHeadOverride;
        case 4: // ELF
            return gender ? kMaterialDefaultHeadOverride : material;
        case 6: // DEF
            return gender ? kMaterialDefaultHeadOverride : material;
        }
    }

    // Material 241 exists but only gives Ogres an alternate looking helmet.
    // Treat the rest like regular Velious helms for now.
    if (material == kMaterialVeliousHelmOgreRZ) {
        if (race == 10) {
            return kMaterialVeliousHelmOgreRZ;
        } else if (Zeal::EqGame::IsPlayableRace(race)) {
            material = kMaterialVeliousHelm;
        }
    }

    // For bugged races, convert Velious Helm to their racial IT### number
    if (material == kMaterialVeliousHelm) {
        switch (race) {
        case 1: // HUM
            return gender ? 620 : material;
        case 2: // BAR
            return gender ? 530 : material;
        case 3: // ERU
            return gender ? 570 : 575;
        case 4: // ELF
            return gender ? 561 : material;
        case 6: // DEF
            return gender ? 540 : material;
        }
    }

    return material;
}

// Detects whether the given race/gender model needs the special material logic applied.
// Checks whether you have the Luclin models enabled or not.
bool HelmManager::IsHelmBuggedOldModel(WORD race, BYTE gender) {

    if (!DetectInstalledFixesComplete) {
        DetectInstalledFixes(); // Have to lazy-loid this here because this is reached before OnZone() is called during initial login.
    }

    // Bugged male races
    if (gender == 0) {
        // Erudite
        return race == 3 ? UseEruditeMaleFix : false;
    }

    // Bugged female races
    if (gender == 1) {
        switch (race) {
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

// Gets the material/model ID from an item.
WORD HelmManager::GetHelmMaterial(Zeal::EqStructures::EQITEMINFO* item) {
    if (!item || item->Type != 0) {
        return 0;
    }
    if (item->Common.Material) {
        return item->Common.Material;
    }
    if (item->IDFile[0] == 'I' && item->IDFile[1] == 'T' && item->IDFile[2] != '\0') {
        int material = atoi(&item->IDFile[2]);
        if (material >= kMaterialVeliousHelm) {
            return material;
        }
    }
    return 0;
}

void HelmManager::RefreshHelm(bool local_only, int delay_ms) {
    if (!refresh_helm_pending) {
        refresh_helm_pending = true;
        ZealService::get_instance()->callbacks->AddDelayed([this, local_only]() { RefreshHelmCallback(local_only); }, delay_ms);
    }
}

void HelmManager::RefreshHelmCallback(bool local_only) {
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    if (self && char_info && refresh_helm_pending) {
        refresh_helm_pending = false;
        if (Zeal::EqGame::IsPlayableRace(self->Race)) {
            WORD old_material = self->EquipmentMaterialType[kMaterialSlotHead];
            WORD material = 0;
            DWORD color = 0;
            if (ShowHelmEnabled.get() && char_info->Inventory.Head) {
                material = GetHelmMaterial(char_info->Inventory.Head);
                color = char_info->Inventory.Head->Common.Color;
                if (HELM_MANAGER_LOG_DEBUG) {
                    Zeal::EqGame::print_chat("[HelmManager] [UpdateHelm] Current helm is %u color %u", material, color);
                }
            }

            // CDisplay isn't sending the right wearchange via this call. We'll do it ourselves after.
            block_outbound_wearchange++;
            SwapHead(self, material, old_material, color, true);
            block_outbound_wearchange--;

            if (!local_only) {
                if (HELM_MANAGER_LOG_DEBUG) {
                    Zeal::EqGame::print_chat("[HelmManager] [UpdateHelm] Manually sending WearChange %u color %u", material, color);
                }
                material = ToCanonicalHelmID(material, self->Race);
                if (self->SpawnId != last_wc.spawn_id || material != last_wc.material || color != last_wc.color.Color)
                {
                    Zeal::Packets::WearChange_Struct wc;
                    memset(&wc, 0, sizeof(Zeal::Packets::WearChange_Struct));
                    wc.spawn_id = self->SpawnId;
                    wc.wear_slot_id = kMaterialSlotHead;
                    wc.material = material;
                    wc.color.Color = color;
                    Zeal::EqGame::send_message(Zeal::Packets::WearChange, (int*)&wc, sizeof(Zeal::Packets::WearChange_Struct), 0);
                }
                else if (HELM_MANAGER_LOG_DEBUG)
                {
                    Zeal::EqGame::print_chat("[HelmManager] [UpdateHelm] Skipping sending WearCchange, we already are in sync with server.");
                }
            }
        }
    }
}

// With the helm manager system, we can fully handle the ShowHelm behavior on the client-side.
// Just make sure the server-side is sending us helms.
void HelmManager::OnZone() {

    last_wc = { 0 };

    auto* char_info = Zeal::EqGame::get_char_info();
    if (!char_info || !Zeal::EqGame::is_in_game()) {
        // Wait and try again
        ZealService::get_instance()->callbacks->AddDelayed([this]() { OnZone(); }, 1000);
        return;
    }

    std::string name = char_info->Name;
    if (name != last_seen_character) {
        last_seen_character = name;
        SyncShowHelm(true);
    }
}

void HelmManager::SyncShowHelm(bool server_silent) {
    Zeal::EqGame::do_say(true, "#showhelm %s%s", ShowHelmEnabled.get() ? "on" : "off", server_silent ? " silent" : "");
}

void HelmManager::DetectInstalledFixes()
{
    int* this_display = *(int**)Zeal::EqGame::Display;
    if (this_display) {
        int dictionary = this_display[1];
        if (dictionary) {
            FARPROC fn = GetProcAddress(GetModuleHandleA("eqgfx_dx8.dll"), "t3dGetPointerFromDictionary");
            if (fn) {
                using FunctionType = uintptr_t(__cdecl*)(int _dictionary, const char* _key);
                FunctionType t3dGetPointerFromDictionary = reinterpret_cast<FunctionType>(fn);
                uintptr_t initialized = t3dGetPointerFromDictionary(dictionary, "ELFHE00_DMSPRITEDEF"); // Use a known-good value to ensure stuff is loaded
                if (initialized) {
                    IO_ini ini(".\\eqclient.ini");
                    UseHumanFemaleFix = !ini.getValue<bool>("Defaults", "UseLuclinHumanFemale") && t3dGetPointerFromDictionary(dictionary, "HUFHE04_DMSPRITEDEF");
                    UseBarbarianFemaleFix = !ini.getValue<bool>("Defaults", "UseLuclinBarbarianFemale") && t3dGetPointerFromDictionary(dictionary, "BAFHE04_DMSPRITEDEF");
                    UseEruditeFemaleFix = !ini.getValue<bool>("Defaults", "UseLuclinEruditeFemale") && t3dGetPointerFromDictionary(dictionary, "ERFHE04_DMSPRITEDEF");
                    UseEruditeMaleFix = !ini.getValue<bool>("Defaults", "UseLuclinEruditeMale") && t3dGetPointerFromDictionary(dictionary, "ERMHE04_DMSPRITEDEF");
                    UseWoodElfFemaleFix = !ini.getValue<bool>("Defaults", "UseLuclinWoodElfFemale") && t3dGetPointerFromDictionary(dictionary, "ELFHE04_DMSPRITEDEF");
                    UseDarkElfFemaleFix = !ini.getValue<bool>("Defaults", "UseLuclinDarkElfFemale") && t3dGetPointerFromDictionary(dictionary, "DAFHE04_DMSPRITEDEF");
                    if (HELM_MANAGER_LOG_DEBUG) {
                        Zeal::EqGame::print_chat("[HelmManager] [DetectInstalledFixes] Complete");
                    }
                    DetectInstalledFixesComplete = true;
                    return;
                }
            }
        }
    }
    return;
}

HelmManager::HelmManager(ZealService* zeal) {

    zeal->hooks->Add(SwapHeadHook, 0x004a1735, SwapHead_hk, hook_type_detour);
    zeal->hooks->Add("EntityChangeForm", 0x005074FA, EntityChangeForm_hk, hook_type_detour);
    zeal->hooks->Add("WearChangeArmor", 0x004A2A7A, WearChangeArmor_hk, hook_type_detour);

    zeal->callbacks->AddGeneric([this]() { OnZone(); }, callback_type::Zone);

    zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
        if (opcode == Zeal::Packets::WearChange && len >= sizeof(Zeal::Packets::WearChange_Struct)) {
            return Handle_Out_OP_WearChange((Zeal::Packets::WearChange_Struct*)buffer);
        }
        return false; // continue processing
        }, callback_type::SendMessage_);

    zeal->callbacks->AddPacket([this](UINT opcode, char* buffer, UINT len) {
        if (opcode == Zeal::Packets::WearChange && len >= sizeof(Zeal::Packets::WearChange_Struct)) {
            return Handle_In_OP_WearChange((Zeal::Packets::WearChange_Struct*)buffer);
        }
        return false; // continue processing
    }, callback_type::WorldMessage);

    if (HELM_MANAGER_LOG_DEBUG) {
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