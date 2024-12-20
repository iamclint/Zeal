#include "melody.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
#include <algorithm>
#include "string_util.h"

// Requirements for Melody on Quarm per Secrets in discord zeal-discussions ~ 2024/03/25
// - Bards only
// - 5 song limit
// - Retries allowed on missed notes
// - Character stuns must end melody

// Test cases:
// - Command line behavior and messages:
//   - Bard class only
//   - # of songs limit <= 5
//   - Only ints as parameters
//   - Zero parameter melody ends melody
//   - Start is prevented when not standing
//   - New /melody without a /stopsong transitions cleanly after current song
//   - /stopsong immediately stops (aborts) active song
// - Check basic song looping functionality (single song, multiple songs)
// - Retry logic for missed notes (correct rewind of song index, retry timeout)\
//   - Should advance song after 8 retries (try Selo's indoors)
//   - Should terminate melody after 15 failures without a success
// - Graceful handling of spells without single target
//   - Skipping of song with single line complaint
//   - Termination of melody after retry limit if all songs are failing
// - Terminated when sitting
// - Paused when zoning, trading, looting, or ducking and then resumed

// Issues list:
// - There's a timing window vulnerability if a UI gem is clicked right as a melody song
//   ends. The click should terminate melody but it doesn't always work and melody just
//   continues after that song is cast (can be confusing). When the current song is
//   failing (like Selo's indoors), the vulnerable timing window is dominant, making it
//   hard to click off the melody with the UI. The new retry_count logic mitigates this.

constexpr int RETRY_COUNT_REWIND_LIMIT = 8;  // Will rewind up to 8 times.
constexpr int RETRY_COUNT_END_LIMIT = 15;  // Will terminate if 15 retries w/out a 'success'.
constexpr ULONGLONG MELODY_SONG_INTERVAL = 150; // Interval between songs and after clickies. If too low, the song may not fire.
constexpr ULONGLONG USE_ITEM_QUEUE_TIMEOUT = 3500 + MELODY_SONG_INTERVAL; // Max duration a useitem will stay queued for before giving up (mostly to prevent ultra-stale clicks).

bool Melody::start(const std::vector<int>& new_songs)
{
    if (!Zeal::EqGame::is_in_game())
        return false;

    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    if (!char_info || char_info->StunnedState)
    {
        Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Can not start melody while stunned.");
        return false;
    }

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self || (self->StandingState != Stance::Stand)) {
        Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Can only start melody when standing.");
        return false;
    }

    //confirm all gem indices in new_songs are valid indices with memorized spells.
    std::vector<int> valid_songs;  // Valid, non-empty gem indices.
    for (const int& gem_index : new_songs)
    {
        if (gem_index < 0 || gem_index >= EQ_NUM_SPELL_GEMS)
        {
            Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Error: Invalid spell gem %i", gem_index + 1);
            return false;
        }

        if (char_info->MemorizedSpell[gem_index] == -1)
            Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Error: skipping empty spell gem %i", gem_index + 1);
        else
            valid_songs.push_back(gem_index);
    }

    if (valid_songs.empty()) {
        Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Error: no valid songs");
        return false;
    }

    songs = valid_songs;
    current_index = -1;
    retry_count = 0;
    casting_melody_spell_id = kInvalidSpellId;
    use_item_index = -1;
    if (songs.size())
        Zeal::EqGame::print_chat(USERCOLOR_SPELLS, "You begin playing a melody.");
    return true;
}

void Melody::end(bool do_print)
{
    if (songs.size())
    {
        current_index = -1;
        songs.clear();
        retry_count = 0;
        casting_melody_spell_id = kInvalidSpellId;
        use_item_index = -1;
        if (do_print)
            Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Your melody has ended.");
    }
}

bool Melody::use_item(int item_index)
{
    if (songs.empty() || item_index < 0 || item_index > 29)
        return false;

    // Set fields so use_item(item_index) will execute during tick().
    use_item_index = item_index;
    use_item_timeout = GetTickCount64() + USE_ITEM_QUEUE_TIMEOUT;
    return true;
}

void Melody::handle_stop_cast_callback(BYTE reason, WORD spell_id)
{
    // Terminate melody on stop except for missed note (part of reason == 3) rewind attempts.
    if (reason != 3 || !songs.size())
    {
        end(true);
        return;
    }

    // Support rewinding to the interrupted last song (primarily for missed notes).
    // Note that reason code == 3 is shared by missed notes as well as other failures (such as the spell
    // is not allowed in the zone), so we use a retry_count to limit the spammy loop that is
    // difficult to click off with UI spell gems (/stopsong, /melody still work fine). The modulo
    // check skips the rewind so it advances to the next song but then allows that song to retry.
    if (casting_melody_spell_id == spell_id && (++retry_count % RETRY_COUNT_REWIND_LIMIT)) {
        current_index--;
        if (current_index < 0) {  // Handle wraparound.
            current_index = songs.size() - 1;
        }
    }
    casting_melody_spell_id = kInvalidSpellId;
}

void __fastcall StopCast(int t, int u, BYTE reason, WORD spell_id)
{
    ZealService::get_instance()->melody->handle_stop_cast_callback(reason, spell_id);
    ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)(t, u, reason, spell_id);
}

void Melody::stop_current_cast()
{
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (char_info && self && self->ActorInfo && self->ActorInfo->CastingSpellId != kInvalidSpellId) {
        ZealService::get_instance()->hooks->hook_map["StopCast"]->original(StopCast)((int)char_info, 0, 0, self->ActorInfo->CastingSpellId);
    }
    casting_melody_spell_id = kInvalidSpellId;
}

void Melody::tick()
{
    if (!songs.size())
        return;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    Zeal::EqStructures::EQCHARINFO* char_info = Zeal::EqGame::get_char_info();

    // Handle various reasons to terminate Zeal automatically.
    if (!Zeal::EqGame::is_in_game() || !self || !char_info ||
        (self->StandingState == Stance::Sit) || (char_info->StunnedState) ||
        (retry_count > RETRY_COUNT_END_LIMIT))
    {
        end(true);
        return;
    }

    //use timestamps to add minimum delay after casting ends and detect excessive retries.
    static ULONGLONG casting_visible_timestamp = GetTickCount64();
    static ULONGLONG start_of_cast_timestamp = casting_visible_timestamp;

    ULONGLONG current_timestamp = GetTickCount64();

    if (!Zeal::EqGame::Windows->Casting || Zeal::EqGame::Windows->Casting->IsVisible)
    {
        casting_visible_timestamp = current_timestamp;
        //reset retry_count if the song cast window has been visible for > 1 second.
        if ((casting_visible_timestamp - start_of_cast_timestamp) > 1000)
            retry_count = 0;
        return;
    }

    // Successfully finished casting current song/spell
    // Reseting this field prevents the song from repeating after this point
    casting_melody_spell_id = kInvalidSpellId;

    // Wait for MELODY_SONG_INTERVAL ms between casting the next song
    if ((current_timestamp - casting_visible_timestamp) < MELODY_SONG_INTERVAL)
        return;

    // Handles situations like trade windows, looting (Stance::Bind), and ducking.
    if (!Zeal::EqGame::get_eq() || !Zeal::EqGame::get_eq()->IsOkToTransact() ||
        self->StandingState != Stance::Stand)
        return;

    stop_current_cast();  //abort bard song if active.

    // Execute a pending use_item() call here
    if (use_item_index >= 0)
    {
        bool success = (use_item_timeout >= current_timestamp) && Zeal::EqGame::use_item(use_item_index);
        use_item_index = -1;
        if (success) {
            casting_visible_timestamp = current_timestamp; // Pushes back the start of next song by MELODY_SONG_INTERVAL ms.
            return;
        }
    }

    // Cast the next song in the melody
    current_index++;
    if (current_index >= songs.size() || current_index < 0)
        current_index = 0;

    int current_gem = songs[current_index];  // songs is 'guaranteed' to have a valid gem index from start().
    WORD current_gem_spell_id = char_info->MemorizedSpell[current_gem];
    if (current_gem_spell_id == kInvalidSpellId)
        return;  //simply skip empty gem slots (unexpected to occur)

    //handle a common issue of no target gracefully (notify once and skip to next song w/out retry failures).
    if (Zeal::EqGame::get_spell_mgr() &&
        (Zeal::EqGame::get_spell_mgr()->Spells[current_gem_spell_id]->TargetType == Zeal::EqEnums::SpellTargetType::Target ||
            Zeal::EqGame::get_spell_mgr()->Spells[current_gem_spell_id]->TargetType == Zeal::EqEnums::SpellTargetType::TargetedAE) &&
        !Zeal::EqGame::get_target())
    {
        Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "You must first select a target for spell %i", current_gem + 1);
        retry_count++;  // Re-use the retry logic to limit runaway spam if entire song list is target-based.
        return;
    }

    casting_melody_spell_id = current_gem_spell_id;
    char_info->cast(current_gem, current_gem_spell_id, 0, -1);
    start_of_cast_timestamp = current_timestamp;
}

Melody::Melody(ZealService* zeal, IO_ini* ini)
{
    zeal->callbacks->AddGeneric([this]() { tick();  });
    zeal->callbacks->AddGeneric([this]() { end(); }, callback_type::CharacterSelect);
    zeal->hooks->Add("StopCast", 0x4cb510, StopCast, hook_type_detour); //Hook in to end melody as well.
    zeal->commands_hook->Add("/melody", {"/mel"}, "Bard only, auto cycles 5 songs of your choice.",
        [this](std::vector<std::string>& args) {

            end(true);  //any active melodies are always terminated

            if (!Zeal::EqGame::get_char_info() || Zeal::EqGame::get_char_info()->Class != Zeal::EqEnums::ClassTypes::Bard)
            {
                Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Only bards can keep a melody.");
                return true;
            }

            if (args.size() > 6)
            {
                Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "A melody can only consist of up to 5 songs.");
                return true;
            }

            std::vector<int> new_songs;
            for (int i = 1; i < args.size(); i++) //start at argument 1 because 0 is the command itself
            {
                int current_gem = -1;
                if (Zeal::String::tryParse(args[i], &current_gem))
                    new_songs.push_back(current_gem - 1);  //base 0
                else
                {
                    Zeal::EqGame::print_chat(USERCOLOR_SPELL_FAILURE, "Melody parsing error: Usage example: /melody 1 2 3 4");
                    return true;
                }
            }
            start(new_songs);
            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });

    // Hooking '/stopsong' to address a client bug: '/stopsong' during a clicky-casted causes client/server desync in casting state:
    // - Client cast bar disappears, but the spell is not interrupted on the server side.
    //   - Client is wiping the casting state and bailing out without sending a message at 0x004cb5bc for bards.
    // - To fix, we will just ignore '/stopsong' if the bard isn't singing bard song
    zeal->commands_hook->Add("/stopsong", {}, "Stops the current bard song from casting",
        [this](std::vector<std::string>& args) {
            if (Zeal::EqGame::EqGameInternal::IsPlayerABardAndSingingASong())
                return false;  // Let regular /stopsong logic run to interrupt it

            return true; // casting a non-gem'd spell (likely a clicky). Prevent '/stopsong' from running.
        });
}

Melody::~Melody()
{
}

