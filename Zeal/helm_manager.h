#pragma once
#include "hook_wrapper.h"
#include "EqStructures.h"
#include "EqPackets.h"
#include "ZealSettings.h"

class HelmManager
{
public:
	HelmManager(class ZealService* zeal);
	~HelmManager();

    static int GetHeadID(Zeal::EqStructures::Entity* entity, int default_value = 0);
    static WORD GetHelmMaterial(Zeal::EqStructures::EQITEMINFO* item);
    static WORD GetItemMaterial(Zeal::EqStructures::EQITEMINFO* item);
    static WORD ToCanonicalHelmMaterial(WORD material, WORD race);

    // Hook implementations
    void HandleWearChangeArmor(int* cDisplay, Zeal::EqStructures::Entity* spawn, int wear_slot, WORD new_material, WORD old_material, DWORD color_tint, bool from_server);
    int HandleSwapHead(int* cDisplay, Zeal::EqStructures::Entity* entity, int new_material, int old_material, DWORD color, bool from_server);
    int HandleSwapModel(int* cdisplay, Zeal::EqStructures::Entity* entity, BYTE wear_slot, char* ITstr, bool from_server);

    // Refreshes your helmet graphics to your current equipment and ShowHelm toggle.
    void RefreshHelm(bool local_only = false, int delay_ms = 64);    

    // Toggles ShowHelm status
    ZealSetting<bool> ShowHelmEnabled = { true, "Zeal", "ShowHelm", true, [this](bool v) { SyncShowHelm(false); RefreshHelm(false, 0); } };

private:

    bool Handle_In_OP_WearChange(Zeal::Packets::WearChange_Struct* data);
    bool Handle_Out_OP_WearChange(Zeal::Packets::WearChange_Struct * data);

    // Checks if this race/gender combination can be fixed
    bool IsPatchedOldModel(WORD race, BYTE gender);

    void RefreshHelmCallback(bool local_only);

    // Does any necessary helm adjustments/initializaiton on zone.
    void OnZone();

    // Sends the '#showhelm' command to the server with our current toggle.
    void SyncShowHelm(bool hide_response);

    // This call generally doesn't succeed until partway into the enter-world process.
    // So we do just-in-time detection when the first SwapHead() hook is called.
    bool DetectInstalledFixes();

    // Suppresses outbound helm WearChange packets from being sent when positive.
    // Sometimes enabled when we are internally manipulating things and don't want that to generate uncessary outbound packets.
    // Usage is to increment to suppress, and drecement to unsuppress (Works recursively).
    UINT block_outbound_wearchange = 0;

    // Used by RefreshHelmetCallback to prevent running multiple times if queued in succession.
    bool refresh_helm_pending = false;

    // We do some initialization on enter world once per-character
    std::string last_seen_character = "";

    // Auto-detected flags on startup that tell us if their Velious asset patch is installed
    bool DetectInstalledFixesComplete = false;
    bool UseHumanFemaleFix = false;
    bool UseBarbarianFemaleFix = false;
    bool UseEruditeMaleFix = false;
    bool UseEruditeFemaleFix = false;
    bool UseWoodElfFemaleFix = false;
    bool UseDarkElfFemaleFix = false;

};