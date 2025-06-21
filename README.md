### Description
Zeal adds quality of life functionality to the legacy (2002) Everquest client
used by most TAKP EqEMU players. The Miles Sound System used by that client
supports plug-ins for arbitrary audio filtering, and Zeal injects itself into the
client like a standard dll by using the .asi extension in the EQ root directory.
It can then patch itself into the client's processing loop and callbacks to
add new functionality and smooth out issues in the old client.

Zeal custom code is entirely open source and the releases are built directly
from the repo using github actions, providing full transparency on the contents.

### Features
- Camera motion improvements (major improvements to third person view)
- Additional key binds (tab targeting, corpse cycling, strafe, pet, map,
  autoinventory, autofire, buy/sell stacks)
- Additional commands (melody, autofire, useitem, autoinventory, autobank,
  link all, loot all, raid survey, singleclick, show loot lockouts, etc)
- Integrated map (see In-game Map section below)
- Additional ui support (new gauges, bag control & locking, looting, spellsets, targetrings,
  nameplates, right click to equip, skill window sorting, ctrl for context menus/looting, etc)
- Autostand on move/cast, autosit on camp with export inventory/spellbook option
- Enhanced chat (% replacements, additional filters and colors, tell windows,
  tab completion, copy and paste)
- Optional enhanced spell info (spells, scrolls, items) on info displays
- Notification sounds (tells, group invites)
- Third party tool support (silent log messages, direct ZealPipes)
- Various client bug fixes and patches (crashe fixes, helm graphical glitches, etc)

### Installation
#### The easy way
1. [Download](https://github.com/iamclint/Zeal/releases/latest) the latest official (non-beta) Zeal
   - Use the link at the top or scroll down past the list of changes to the `Assets` section and download the `zeal_v#.#.#_*.zip` file
   - Ignore the `Source code` zip files
2. Quit Everquest and extract the zip file contents into your EQ game root directory
   - If you have previously installed zeal, deleting the old `uifiles/zeal` is recommended before copying over
   - For minimalists, the only required files are the `zeal.asi` and the `uifiles/zeal` folder
3. Recommended: Install a UI skin modified with the extra Zeal UI functionality
   such as mana gauges, experience per hour, loot all / link buttons, etc.
   - If the UI is not updated, there will be some XML error complaints that can be ignored
   - Check the list below, the Quarm discord `#ui-discussion` channel, or the Quarm guide
    link below for compatible UI's
4. Test Zeal installation in game by typing "/zeal version" and "/help zeal".
5. Configure Zeal using the new Zeal options window and assign new key binds.
   - The Zeal options window opens in parallel to the EQ options window (Alt-o)

#### If that doesn't work
Ensure that sound is enabled in the game (the volume level can be set to zero).
   - Check that `Sound=TRUE` in the `eqclient.ini` in your root Everquest directory

A comprehensive guide, including troubleshooting, can be found at
https://quarm.guide/install-guides under "Installing Zeal" or a
simplified checklist under "Checklist for Installing Zeal".

Note that the method Zeal uses to inject itself into the client can trigger the
heuristics of anti-virus scanners to label it as malware. See trouble-shooting
guide above if your `Zeal.asi` file keeps disappearing.

#### Description of zip file contents
1. `Zeal_README.md`: A copy of this readme file
2. `Zeal.asi`: Executable code that is loaded when EQ sound is enabled
3. `Zeal.pdb`: Symbol debug file for the `zeal.asi`. Developer use only.
4. `uifiles/zeal`: Folder with Zeal specific UI modifications (options, new features)
   - The files in `uifiles/zeal` override `uifiles/default` and `uifiles/<your_skin>`
5. `crashes/`: Folder with optional crash reporter that will send any captured crash zip logs for review 

#### Storage location of Zeal settings (in eqgame.exe folder)
1. `zeal.ini`: Contains most of the zeal settings (some common, some per character)
2. `eqclient.ini`: Contains extended key binds
3. `UI_<name>_pq.ini`: Configuration of Zeal client windows (map, extra item display, options)
4. `<name>_spellsets.ini`: Per character saved spell sets
5. `<name>_protected.ini`: Per character saved /protect item list

### Compatible UI's
- https://github.com/NilliP/NillipussUI_1080p
- https://github.com/NilliP/NillipussUI_1440p
- https://www.eqinterface.com/downloads/fileinfo.php?id=6959
- https://github.com/LordDemonos/Quarm.Guide/blob/master/assets/duxaUI.7z?raw=true

### Chat % Replacements
- %n or %mana for mana%
- %h or %hp for hp%
- %loc for your location
- %th or %targethp for your targets health %

### Commands (`/help zeal`)
___
- `/alarm`
  - **Arguments:** `oldui`
  - **Description:** Re-opens the alarm window, if oldui is specified it allows for an alarm on it.

- `/aspectratio`
  - **Aliases:** `/ar`
  - **Description:** Change your aspect ratio.

- `/assist`
  - **Arguments:** `on`, `off`
  - **Description:** Supports per character assist auto-attack on/off, also supports failure message.

- `/autobank`
  - **Aliases:** `/autoba`, `/ab`
  - **Description:** Drops whatever is on your cursor into your bank. [requires you to be at a banker] (not fully functional atm)

- `/autoinventory`
  - **Aliases:** `/autoinv`, `/ai`
  - **Description:** Drops whatever is on your cursor into your inventory.

- `/buffs`
  - **Description:** Outputs the players buff timers to the chat only if they are using OldUI.

- `/bluecon`
  - **Description:** Changes the blue con color to Zeal Color Button #15 which is in the Zeal Options window, Colors Tab.

- `/classicclasses`
  - **Aliases** `/cc`
  - **Example:** `/cc`
  - **Description:** toggles classic classes in who and other areas.

- `/clientmanatick`
  - **Aliases** `/cmt`
  - **Example:** `/cc`
  - **Description:** Toggles client mana tick (disabled by default in this client).

- `/cls`
  - **Description:** Adds cls alias for clearchat.

- `/corpsedrag`
  - **Aliases:** `/drag`
  - **Arguments:** none, `nearest` (auto-targets nearest corpse for dragging)
  - **Description:** Attempts to corpse drag your current target. Use `/corpsedrag nearest` to auto-target.

- `/corpsedrop`
  - **Aliases:** `/drop`
  - **Arguments:** none, `all` (drops all corpses)
  - **Description:** Stop dragging your currently targeted corpse. To drop all use `/corpsedrop all`.

- `/fcd` (floating combat damage)
  - **Arguments:** none, `client font size #`, `font`
  - **Example:** `/fcd` toggles on and off
  - **Example:** `/fcd 6` sets it to use client font size 6
  - **Example:** `/fcd font arial_24` sets it to use custom font arial_24
  - **Description:** shows floating combat damage.

- `/follow`
  - **Arguments:** none, `zeal on`, `zeal off`, `distance <value>`
  - **Example:** `/follow` toggles on and off (normal client command)
  - **Example:** `/follow zeal on` turns on patched Zeal auto-follow mode (same as options tab)
  - **Example:** `/follow distance 5` sets the Zeal mode follow distance to 5 (default 15)
  - **Description:** adds /follow arguments that support enabling Zeal mode with adjustable distance. The
    Zeal mode disables rapid toggling of run mode and slow turning to improve /follow reliability.

- `/fov`
  - **Arguments:** `int`
  - **Example:** `/fov 65`
  - **Description:** changes your field of view with a value between 45 and 90.

- `/help zeal`
  - **Description:** Shows the custom Zeal commands.

- `/hidecorpse`
  - **Arguments:** `looted` (hides after looting), `npc` (hides npcs only), `showlast` (unhides last hidden corpse)
  - **Aliases:** `/hideco`, `/hidec`, `/hc`
  - **Example:** `/hidecorpse looted`
  - **Description:** Adds extra arguments and aliases to built in /hidecorpses command.

- `/inspect target`
  - **Description:** adds target argument to /inspect, this just inspects your current target.

- `/lead`
  - **Arguments:** none, `open` (reports raid groups with open slots), `all` (lists all raid groups)
  - **Description:** prints out your current group leader (and raid leader if in raid).

- `/linkall`
  - **Arguments:** none (pastes into active chat) or `rs` (rsay), `gs` (gsay), `gu` (guildsay), `ooc`, `auc`, `say`
  - **Description:** prints item links if looting window is open. The argument options route directly to channel for macros.

- `/loc noprint`
  - **Description:** adds noprint argument to /loc, this just sends loc directly to your log.

- `/log`
  - **Arguments:** `on`, `off`, `output text message with percent converts`
  - **Description:** if first argument is not `on` or `off`, it copies the rest of line to the log directly.

- `/lootall`
  - **Description:** loots all items from a corpse if looting window is open.

- `/lootctrl`
  - **Arguments:** none (toggles), `on`, `off`
  - **Description:** controls the requirement to hold ctrl down to enable right click looting

- `/lootlast`
  - **Arguments:** `item_id_#`, `item_link`, or `0` to disable.
  - **Description:** specifies an item ID that will be left as the last item when using /lootall on your corpse 

- `/map`
  - **Arguments:** `on`, `off`, `size`, `alignment`, `marker`, `background`, `zoom`, `poi`, `labels`, `level`, `ring`, `grid`
  - **Example:** See In-game map section below
  - **Description:** controls map enable, size, labels, zoom, and markers

- `/melody`
  - **Arguments:** `song gem #'s (maximum of 5)`, `resume`
  - **Aliases:** `/mel`
  - **Example:** `/melody 1 4 2 3`
  - **Example:** `/melody resume` - Resumes an interrupted melody at the interrupted song index.
  - **Description:** plays songs in order until interrupted in any fashion.

- `/mystats`
  - **Arguments:** `none`, `info`, `<item_link>`
  - **Example:** `/mystats <item_link>` Prints out your current offensive stats if you were holding that item.
  - **Description:** prints out current stats values (mitigation, avoidance, offense, etc)

- `/nameplate`
  - **Arguments:** `colors`, `concolors`, `targetcolor`, `charselect`, `hideself`, `x`, `hideraidpets`, `showpetownername`, `targetmarker`, `targethealth`, `inlineguild`
  - **Description:** toggles nameplate modes for adjusting colors (tints) and text

- `/outputfile`
  - **Aliases:** `/output`, `/out`
  - **Arguments:** `inventory | spellbook` `[optional_name]`
  - **Example:** `/outputfile inventory my_inventory`
  - **Description:**
    - `inventory` outputs information about your equipment, inventory bag slots, held item, and bank slots to a file.
    - `spellbook` outputs a list of all spell ids current scribed in your spellbook.

- `/pandelay`
  - **Arguments:** `ms delay`, `none`
  - **Example:** `/pandelay 200`
  - **Description:** changes the amount of delay before left click panning will start to happen

- `/pipe`
  - **Arguments:** `string`
  - **Example:** `/pipe set a respawn timer for 30 seconds`
  - **Description:** outputs a string through the named pipe.

- `/pipedelay`
  - **Arguments:** `int`
  - **Example:** `/pipedelay 500`
  - **Description:** changes the delay between each loop of labels/gauges being sent out over the named pipe.

- `/protect`
  - **Arguments:** `on`, `off`, `value`, `item`, `<item_link>`, `list`, `cursor`, `worn`
  - **Example:** `/protect value 10` Protects against dropping or destroying items >= 10 pp
  - **Example:** `/protect list` Prints the list of currently protected items
  - **Example:** `/protect item 10931` Toggles protection from dropping, destroying, or selling the Crown of Rile (10931)
  - **Example:** `/protect <item_link>` Toggles protection from dropping, destroying, or selling the item_link item.
  - **Description:** Secondary protection against accidental loss of items. This should *not* be relied
          upon as the primary method of protection and you will not be reimbursed if it doesn't protect
          you from your own mistake.  Zeal intercepts the client calls to sell, destroy, or drop cursor
          contents and blocks the action if it is a non-empty container, the item_value is >= value (not
          checked for sell), or the item is on the protect list. It also protects against all trades to
          banker NPCs and trades of protected items or non-empty bags to NPCs (including pets). The
          enable, value, and protected list are stored per character with the list stored in the
          `./<character_name>_protected.ini` file.

- `/reloadskin`
  - **Description:** reloads your current skin using ini.

- `/resetexp`
  - **Arguments:** ``
  - **Example:** `/resetexp`
  - **Description:** resets the exp per hour calculations.

- `/rt`
  - **Description:** targets the last tell or active tell window player, also selects the player in your raid window

- `/run`
  - **Arguments:** none (toggles), `on` (run), `off` (walk)
  - **Description:** Controls run versus walk mode.

- `/selfclickthru`
  - **Arguments:** `on`, `off`
  - **Description:** Disables (on) click on self in third person and allows 'u' to activate doors.

- `/showhelm`
  - **Aliases:** `/helm`
  - **Arguments:** `on, off`
  - **Example:** `/showhelm on`
  - **Description:** Toggles your helmet.

- `/showlootlockouts`
  - **Aliases:** `/showlootlockout`, `/showlockouts`, `/sll`
  - **Description:** Shows you your current loot lockouts on Quarm.

- `/singleclick`
  - **Arguments:** none, `bag #` where 0 disables and 1-8 sets inventory bag #
  - **Description:** Toggles on and off the single click auto-transfer of stackable items to open
    give, trade, or crafting windows. It only activates when either ctrl or shift are held down.
    The `bag #` sets the target tradeskill inventory target if no world trade/tradeskill windows
    are open. Set # to zero to disable (default). The # is not a persistent setting (clears on camp).
  - **Example:** `/singleclick bag 2` will set inventory slot bag 2 (1-8) as the target.

- `/sit`
  - **Description:** The /sit command now accepts "on" as an argument. Using "/sit on" will always make you sit, even if you are currently sitting. This matches the game's native "/sit off" which always makes you stand even if you are currently standing. The "/sit" command will continue to toggle sit/stand state if no argument is provided or if the argument provided is not on or off. Additionally, "/sit down" now works as well and will always make you sit, even if already sitting.

- `/sortgroup`
  - **Arguments:** ``
  - **Example:** `/sq` `/sq 1 2`
  - **Description:** sorts your current group members in the ui  using /sq 1 2 will swap players 1 and 2 in your group on your ui.

- `/spellset`
  - **Arguments:** `save`, `load`, `delete`
  - **Example:** `/spellset save buffs`
  - **Example:** `/spellset load buffs`
  - **Example:** `/spellset delete buffs`
  - **Description:** allows you to save and load spellsets

- `/survey`
  - **Arguments:** `on`, `off`, `channel`, `new`, `response`, `results`, `share`
  - **Description:** Survey helper for polling raid groups. See /survey section below.

- `/target`
  - **Aliases:** `/cleartarget`
  - **Description:** acts as normal /target unless you provide no argument in which case it will clear your target.

- `/targetring`
  - **Arguments:** `size`, `indicator`
  - **Example:** `/targetring 0.25`
  - **Example:** `/targetring indicator` toggles auto attack indicator.
  - **Description:** toggles targetring on/off.

- `/tellwindows`
  - **Description:** Toggle tell windows. 

- `/tickreverse`
  - **Description:** Swaps the direction of the server tick gauge.

- `/timer`
  - **Arguments:** `int`
  - **Example:** `/timer 10`
  - **Description:** holds the last hotbutton pressed down for the duration (decisecond like /pause).

- `/timestamp`
  - **Aliases:** `/tms`
  - **Description:** Shows message timestamps.

- `/tooltipall`
  - **Description:** Toggle showing all open containers tooltips when holding alt.

- `/tooltiptimer`
  - **Aliases:** `/ttimer`
  - **Arguments:** `int`
  - **Example:** `/ttimer 500`
  - **Description:** change the delay in which a tooltip pops up on mouse hover.

- `/trade`
  - **Aliases:** `/opentrade`, `/ot`
  - **Description:** Opens a trade window with your current target.

- `/uilock`
  - **Arguments:** `on`, `off`
  - **Description:** Sets (on) or clears (off) the UI Lock value on primary game windows. Bag windows must be open to take effect.

- `/useitem`
  - **Arguments:** `slot_#` (+ optional `quiet` that suppresses warnings if no click effect)
  - **Description:** Activates a click effect on item in slot_#.
  - **Example:** `/useitem 16 quiet` activates click effect on BP and suppresses some warnings

- `/zeal`
  - **Arguments:** `version`
  - **Description:** Shows the version of zeal.

- `/zealcam`
  - **Aliases:** `/smoothing`
  - **Arguments:** `x y 3rdperson_x 3rdperson_y`
  - **Example:** `/zealcam 0.7 0.2 0.7 0.2` if 3rd person arguments are not supplied, the first x and y are applied to both
  - **Description:** Toggles Zeal's mouse look smoothing methods, the first 2 arguments are first person sensitivity, and the last 2 are for 3rd person

- `/zealinput`
  - **Description:** toggles the zeal input setup for any input in game, giving you a more modern input
     - ctrl+c, ctrl+v, left, right, shift left+right for highlighting, home, end, etc
     - enhanced tab completion for /tell, /t, and /consent

___
### Key Binds (Added to Options->Keyboard)
- Cycle through nearest NPCs
- Cycle through nearest PCs
- Strafe Right
- Strafe Left
- Auto Inventory
- Toggle open all containers
- Toggle last 2 targets
- Reply target
- Pet Attack
- Pet Guard
- Pet Follow
- Pet Back
- Pet Sit
- Pet Health
- Do /loot (targeted corpse)
- Slow turn left
- Slow turn right
- Auto Fire
- Buy / sell stack
- Close all tell windows
- Target nearest pc corpse
- Target nearest npc corpse
- Cycle through nearest pc corpses
- Toggle map on/off
- Toggle through map default zooms
- Toggle through map backgrounds
- Toggle through map label modes
- Toggle up or down through visible map levels
- Toggle map visibility of raid members and member names
- Toggle map visibility of grid lines
- Toggle map interactive mode (internal overlay)
- Toggle nameplate colors for players on/off
- Toggle nameplate con colors for npcs on/off
- Toggle nameplate for self on/off
- Toggle nameplate for self as X on/off
- Toggle nameplate for raid pets and your pet on/off
- Toggle nameplate for players pet owners name on/off
- Toggle nameplate choices that are shown at character selection screen on and off
- Toggle target nameplate color on and off
- Toggle target nameplate marker on and off
- Toggle target nameplate health on and off

---
### Advanced input (/zealinput) including tab completion
- Enables copy (ctrl+c), paste (ctrl+v)
- Enables left, right, shift + left and shift + right for highlighting, home, end etc
- Enables enhanced tab completion for /tell, /t, and /consent
  - `/tell<tab>`, `/tell <tab>`, `/tell name <tab>` cycles tell history list like default client
    - Tell history cycling mode will result from a reply keybind (`r`) 
  - `/tell start_of_name` triggers a search across tell history, raid, and zone
     to populate a new cycle list (tab or shift-tab) which is printed to chat
    - Any key besides tab or shift will clear the search cycle list

## Right click to equip item
- Enabled in Zeal general options
- Must be in your bags.
- Will equip to an empty slot when available.
- Equip Priority: Primary, Secondary, Range, Chest, Legs, Head, Arms, Hands, Feet,
  Shoulders, Back, Neck, Face, Waist, WristLeft, WristRight, EarLeft, EarRight, RingLeft,
  RingRight, Ammo
- Can hold Shift (2nd) / Ctrl (3rd) / Shift+Ctrl (4th) to equip the item to alternate slots
  if it can be equipped in several slots in the list.

## Polling of raid using /survey
### Usage by Zeal-enabled raidmembers:
#### Configuration:
- All raid members should execute `/survey on` to enable (persistent setting only needed once)
  - When on, Zeal monitors for the special survey message to enable auto channel joining & auto
    opening of the dialog box as well as accumulate poll results.
- The originator of questions needs to specify the response chat channel using `/survey channel <name>`
  - The `<name>` must start with a `survey` prefix (e.g., `/survey channel survey123`)
  - The channel name is also persistent and only needs to be set once
  - Use a unique suffix to avoid response collisions with other /survey parties
  
#### Polling:
`/survey new <question>` starts a new survey.
  - This generates a raidsay message `ZEAL_SURVEY | <channel> | <question>` that is detected by
    Zeal when in the raid
  - If /survey is enabled, Zeal will join the chat channel to send the response
  - Zeal then opens a dialog box for raid members to respond and sends the answer to the chat channel
    as `ZEAL_RESPONSE | <answer>`
    - Answers besides yes/no from the dialog can be sent with `/survey response <answer>`
  - Zeal monitors the survey chat channel and accumulates the responses (and snuffs all messages)

`/survey results` will print the results of the poll to local chat
  - Counts up each unique response with a list of first 25 names
  - Also reports a count and list of up to 25 raid members who didn't respond

`/survey share` prints the summary results (no names) to /raidsay

### Usage by non-Zeal raidmembers
In order to participate, they can manually watch for the ZEAL_SURVEY raid messages and then
manually join the chat channel and send their response by sending a message to the channel:
`/# ZEAL_RESPONSE | <answer>` where # is the chat channel. Zeal users can share the tallied results.
Note that the joined response chat channel will not be filtered by Zeal so it will get spammy.
They could leave after submitting their response to avoid it.

___
### UI
- **Gauge EqType's**
  - `23` EXP Per Hour
  - `24` Server tick timer
  - `25` Global cast recovery countdown timer
  - `26` to `33` Recast recovery countdown timers for spell0 - spell7
  - `34` Attack (melee/range) recovery timer

- **Label EqType's**
  - `80` Mana/Max Mana
  - `81` Exp Per Hour Percentage
  - `82` Owner of target (if pet)
  - `83` Count of empty inventory slots
  - `84` Count of all inventory slots
  - `85` Count of filled inventory slots
  - `124` Current Mana
  - `125` Max Mana
  - `134` Spell being casted
  - `135` Song Window Buff 1
  - `136` Song Window Buff 2
  - `137` Song Window Buff 3
  - `138` Song Window Buff 4
  - `139` Song Window Buff 5
  - `140` Song Window Buff 6

- **LootAllButton**
- **LinkAllButton** (w/option to select either `, ` or ` | ` delimiter)

- Option to select background zone at character select w/explore mode.

### Options UI 
- Separate Zeal options window that opens in parallel with the client window

___
### Zeal pipes
- Zeal supports creating a namedpipe for streaming game updates to third party applications
- C# example: https://github.com/OkieDan/ZealPipes

---
### Tick Timer

Adds a Gauge (EQType 24) that supports drawing a server tick timer natively in the EQUI.

The gauge drain/fill style can be swapped using `/tickreverse`.

The tick event is also logged to the Zeal Pipe, in addition to the gauge value:
- `{ "type": 0, "text": "Tick" }`

---
### Building
#### Github official release builds
1. Commit an updated, unique ZEAL_VERSION in Zeal/Zeal.h that will be used as the release tag.
2. Go to the "Actions" tab of the Github workspace
3. Select the "Create Manual Release" workflow on the left
4. Click the drop-down menu on the right top side titled "Run workflow"
5. Select the branch with the commit to be released
6. Add a summary description that will be prepended to the change log notes
7. Click the green "Run workflow" button
8. After the green checkmark appears, go back to the main workspace and verify the content of the new release tag.

#### Local builds
Build in `Release` `x86` (32bit) mode using Microsoft Visual Studio 2022 (free Community edition works)

---
### Creating Fonts (advanced users)
Zeal advanced users can create their own fonts to use with Zeal in addition to those that come with zeal install.
The custom created fonts can be used with the following Zeal features:
*  Floating Combat Damage, Maps, and Nameplates

#### Generation of new font files:
Zeal uses 'spritefont' files generated by MakeSprintFont.exe, a Microsoft windows command line tool.
  - https://github.com/microsoft/DirectXTK/wiki/MakeSpriteFont
  - Click on Downloads@Latest
  - Example command line:
   `./MakeSpriteFont "Arial" /FontStyle:Bold /FontSize:30 /TextureFormat:CompressedMono arial_bold_30.spritefont`
  - Copy the new fonts to the `uifiles/zeal/fonts` folder
  - It will now be selectable from the combobox in game next time the options window is opened
  - Note: You will need to manually back up these fonts when updating Zeal if erasing the uifiles folder

---
### Nameplate Options
#### Setup and configuration
Zeal includes options for players to adjust Player Nameplates and NPC Nameplates in game.
In addition, Skeletons now show a Nameplate. (Client Skeleton Nameplate Bug Fix)
Necromancers will now have an easier time finding their corpses.

The nameplate is controlled through three interfaces:
* Dedicated Zeal options window tab (requires `zeal\uifiles`, see Installation notes above)
* Key binds for nameplate options (configure in EQ Options->Keyboard->Target)
* The /nameplate commands described below

#### Enabling Disabling Nameplate Options
The Zeal Nameplate options tab is the primary interface, but the redundant commands below are available:
* `/nameplate colors` - Toggles Nameplate Colors for Players on and off
* `/nameplate concolors` - Toggles Nameplate Con Colors for NPCs on and off
* `/nameplate hideself` - Toggles Player Nameplate on and off
* `/nameplate x` - Toggles Player Nameplate as X on and off
* `/nameplate hideraidpets` - Toggles Raid member Pets Nameplate on and off
* `/nameplate showpetownername` - Toggles Players Pet Owner on Nameplate on and off
* `/nameplate charselect` - Toggles Nameplate Choices Shown at Character Selection Screen on and off
* `/nameplate targetcolor` - Toggles Target Nameplate Color on and off
* `/nameplate targetblink` - Toggles blinking of target nameplate on and off (rate controlled by targetring slider)
* `/nameplate attackonly` - Limits blinking of target nameplate to only when auto-attack is on
* `/nameplate targetmarker` - Toggles Target Nameplate Marker on and off
* `/nameplate targethealth` - Toggles Target Nameplate Health on and off
* `/nameplate targethealth` - Toggles Target Nameplate Health on and off
* `/nameplate inlineguild` - Toggles guild name appearing inline or on a separate line
* `/nameplate zealfont` - Toggles internal client or custom Zeal fonts
* `/nameplate dropshadow` - Toggles drop shadowing on Zeal fonts
* `/nameplate extendedshownames` - Toggles availability of /Shownames 5, /Shownames 6, and /Shownames 7

#### Changing the Color of Nameplates
Zeal allows players to change the colors of the Nameplates of Players and NPCs in game.
The Color Selector is available in the Zeal Colors Tab of the Zeal Options menu.
The following 19 Nameplate Colors can be changed to custom colors.
* AFK, LFG, LD, MyGuild, Raid, Group, PVP, Roleplay, OtherGuilds, DefaultAdventurer
* NPC Corpse, Player Corpse, GreenCon, LightBlueCon, BlueCon, WhiteCon, YellowCon, RedCon, Target Color

#### Default Colors of Nameplates when using Nameplate Colors system
* 1 - AFK - Orange                   
* 2 - LFG - Yellow                   
* 3 - LD - Red                       
* 4 - MyGuild - White Red            
* 5 - Raid - White Light Purple   
* 6 - Group - Light Green           
* 7 - PVP - Red              
* 8 - Roleplay - Purple       
* 9 - OtherGuilds - White Yellow    
* 10 - DefaultAdventurer - Default Blue
* 11 - Npc Corpse - Black
* 12 - Players Corpse - White Light Purple
* 13 - Green Con NPCs - CON_GREEN
* 14 - LightBlue Con NPCs - CON_LIGHTBLUE
* 15 - Blue Con NPCs - Default DarkBlue is lighter than CON_BLUE
* 16 - White Con NPCs - CON_WHITE
* 17 - Yellow Con NPCs - CON_YELLOW
* 18 - Red Con NPCs - CON_RED
* 19 - Target Color - Default Pink

#### Zeal Nameplate Font Choices and Drop Shadow option
1. The Zeal Nameplate tab now supports selecting custom "spritefont" formatted bitmap font files
 to choose which font your Zeal Nameplate has. Using this option turns off the normal Nameplate
 and replaces it with the Zeal Nameplate. There will be some variation in the location of the custom
 Zeal nameplate versus the client.
2. The Zeal Nameplate tab now offers a Drop Shadow option when using custom fonts for Nameplates.
  This option makes the font more visible by adding a black shadow behind the text.

A few sizes of arial font and arial_bold font are included with Zeal install and accessed through the
options tab combobox. See above for notes on how to generate new fonts.

---
### In-game Map
#### Map data source
The map data was sourced from Brewall's maps: https://www.eqmaps.info/eq-map-files/ with minimal
modifications (see README.md in zone_map_src). As a result there are some out of era points of interest.

#### Setup and configuration
Zeal 4.0 and later includes an integrated in-game map that contains the map data for
all zones through Planes of Power. The map is drawn into the game's DirectX viewport
as part of the rendering sequence and is by default not 'clickable' (see interactive mode below).

The map is controlled through four interfaces:
* Dedicated Zeal options window tab (requires `zeal\uifiles`, see Installation notes above)
* Mouse interactions (external window or internal window in interactive mode)
* Key binds for frequent map actions (configure in EQ Options->Keyboard->UI)
* The /map command

Most of the default map settings are stored in the zeal.ini file of the root Everquest directory.
The defaults are updated when adjusting settings in the Zeal options map tab. The size and
position of the internal map window is stored as part of the UI_character.ini files like normal
game windows. The key binds and /map commands create temporary changes unless the
`/map save_ini` command is used.

It is recommended to use the Options tab to adjust the basic map settings to the preferred
defaults (background, labels, names, marker sizes) and then use the keybinds for more
frequent map adjustments (on/off, toggle zoom, toggle backgrounds, toggle labels,
toggle visible levels).  The /map commands include extra options like poi search.

#### Enabling the map
* Zeal options checkbox
* Key bind: "Toggle Map" - Toggles map on and off (recommend 'm')
* Command examples:
  - `/map` - Toggles map on and off
  - `/map on` - Turns map on
  - `/map off` - Turns map off

#### Map size, position, and alignment
The external map window (see external map window below) can be moved and resized like a standard window.

The internal map operates in two modes.  In interactive mode, the map is framed by a
standard client window that allows it to be positioned and sized. When disabled, the
map draw viewport is fixed and transparent to the mouse. See Interactive Mode below
for more details.

The internal map size and position can also be controlled by the `/map size` command.
The content is drawn to fit within a rectangular viewport defined by a top left corner,
a height, and a width specified as a percentage of the game window dimensions. The
map viewport is relative to the game window and independent of the game /viewport,
so the map can be placed anywhere in the game window.

The zones have different aspect ratios, so some zones will scale to fill the height
and others the width.  The map alignment setting (top left, top center, top right)
controls where the map is drawn when it is height constrained.

* Zeal option enables for interactive mode or external window and a combobox for alignment
* Command examples:
  - `/map size 2 3 50 60` map window top=2% left=3% height=50% width=60% of game window dimensions
  - `/map alignment center` aligns the aspect ratio constrained map to the top center of the viewport

#### Map background
The map supports four different options for the map background for contrast enhancement:
clear (0), dark (1), light (2), or tan (3).  Additionally, it supports alpha transparency.

* Zeal options combo box for map background and slider for setting alpha as a percent
* Key bind: "Toggle Map Background" - toggles through the four settings
* Command examples:
  - `/map background 1` sets the background to dark with no change to alpha
  - `/map background 2 40` sets the background to light with 40% alpha

#### External map window
The map has simple support for opening an external window outside of the EQ client window.
This window can be moved and resized like a standard window. See interactive mode for other 
mouse inputs. The map content is controlled with the normal map key binds. Use the save_ini
command to store the current size and position as the default.

* Zeal options checkbox
* Command examples:
  - `/map external` - Toggles map between internal overlay and external window
  - `/map save_ini` - Stores the current external window size and position (and other settings)

If the map content looks pixelated, the monitor may be set to a DPI scaling greater than 100%.
Note that the EQ application itself does not properly handle this. To workaround, set the 
Windows override to tell the OS that the application will handle scaling:
  * Right click on eqgame.exe:
    - Properties -> Compatibility -> Change high DPI settings -> High DPI scaling override -> By application

#### Map zoom
The default 100% map scale makes the entire zone visible sized to the height or width constraint.
In zoom, the map draws all available data that fits within the rectangular viewport. The zoom
algorithm works to maximize the visible map closest to the player. Map edges will be pinned
to a viewport edge until the user moves at least half the viewport away, and then the map
background will scroll with the player centered in the viewport.

* Zeal options slider and default zoom select combobox
* Key bind: "Toggle Map Zoom" - toggles through 100%, 200%, 400%, 800% zoom
* Command examples:
  - `/map zoom 200` sets map scaling to 200% (2x)

#### Interactive mode
* Note: Use right click context menu in interactive mode to unlock the window to move and resize.

The map supports drag panning and mouse wheel zoom in interactive mode. Interactive mode is always 
enabled in external window mode, while a keybind toggle is used to toggle the internal overlay map
in and out of interactive mode. When not in interactive mode, the internal overlay map is transparent
to the mouse. When interactive mode is enabled, the map can be panned using a left mouse button drag
and zoomed using the scroll wheel. Once panning starts, auto-center is disabled until a right mouse
button click. The external window also supports a middle mouse button click to drop markers.

* Key bind: "Toggle Map Interactive Mode" - toggles internal overlay between transparent and interactive

#### Show zone mode and world data search
Zone maps other than the player's current location can be explored using the show_zone command. The
target zone is specified using the zone's short name (like /who all). Interactive mode, levels, grid,
labels, and poi search all work for the selected zone.

The world command allows browsing the available zone names in EQWorldData. See command examples below.

* Command examples
  - `/map show_zone gukbottom` shows the zone map for the Ruins of Old Guk
  - `/map show_zone` exits show_zone mode
  - `/map world dump` prints the zone ids, short names, and long names of all available zones
  - `/map world search karan` prints all available zones that contain 'karan' in their short or long names

#### Map grid
A simple background grid aligned at a selectable pitch is available. The x == 0 and y == 0 axes
are colored orange.

* Zeal options checkbox (enable) and slider (pitch)
* Command examples:
  - `/map grid` toggles grid on and off
  - `/map grid 500` sets the grid pitch to 500 loc units (lines at multiples of 500)

#### Map ring
A simple distance ring around the current position is available. The distance can be auto-set
based on the tracking skill for rangers, druids, and bards, so they can simply toggle the ring
on and off with `/map ring` (or explicitly on or off with `ring on` or `ring off`).

* Command examples:
  - `/map ring` if visible or a non-tracker, turns ring off
  - `/map ring` if not visible, sets the ring at max tracking distance per skill level
  - `/map ring on` sets the ring at max tracking distance per skill level
  - `/map ring off` turns ring off
  - `/map ring 500` sets the ring around the player at a distance of 500 (all classes)

#### Showing group and raid members
The map supports showing the live position of other group and raid members. The group
member markers are slighly shrunken player position markers and colored in this order
relative to their group listing: red, orange, green, blue, purple. The raid member
markers are simple fixed triangles with varying color.  Since there are a high number
of potential raid members, it is recommended to not use the persistent Show Raid checkbox
in the options tab and instead use the key bind to situationally toggle it on and off.

* Zeal options combobox to select group members mode (off, marker, numbers, names)
* Zeal checkbox to enable raid member visibility
* Zeal slider to adjust name length
* Key bind: "Toggle Map Show Raid" - Toggles visibility of raid members
* Key bind: "Flash Map Member Names" - Toggles (previously flashed) names of group and raid members
* Command examples:
  - `/map show_group` toggles the group member markers on and off
  - `/map show_group labels_off` disables group member labels
  - `/map show_group numbers` enables numeric (F2-F6) group member labels (uses nameplate colors)
  - `/map show_group names` enables shortened group member names (uses nameplate colors)
  - `/map show_group length 8` sets the shortened length of group and raid member names to 8
  - `/map show_raid` toggles the raid member markers on and off

#### Showing map levels
The map supports showing different levels based on the Brewall map color standards. Not all of
the zones are properly colored, but it does work well in some of the 3-D overlapping zones. It
also supports a simplified auto z-level mode that shows map data within a z-level range of the
player as fully opaque and further data at a faded alpha transparency level. The auto-mode
is selectable using the toggle map level keybinds (see below).

* Zeal slider to adjust the faded z-level alpha transparency value
* Key bind: "Toggle Map Level Up", "Toggle Map Level Down" - toggles up or down the visible level
  - The default index of 0 shows all levels and -1 = auto z-level.
* Command examples:
  - `/map level` shows the current zone's map data level info
  - `/map level 0` shows default of all levels
  - `/map level 2` shows the current zone's level 2 data
  - `/map level -1` enables auto z-level mode
  - `/map level autoz 10`  sets the auto z-level mode height limit to +/- 10 (setting to 0 restores default)

#### Position markers
The map supports adding position markers for easier identification of target coordinates. The
markers have a label centered above them, with the default set to the marker loc values. There
is no set limit to the number of markers. See how to clear them below.

* Zeal options slider to adjust the marker size
* Command examples:
  - `/map marker 1100 -500` adds a map marker at /loc of 1100, -500 labeled "(1100, -500)"
  - `/map 1100 -500` is a shortcut for the command above to set a marker at 1100, -500
  - `/map marker -300 200 camp` adds a map marker at /loc of -300, 200 labeled "camp"
  - `/map marker` clears all markers
  - `/map 0` is a shortcut for clearing all markers
  - `/map marker size 40` sets the marker size to "40%"

#### Map font
The map supports selecting a "spritefont" formatted bitmap font file. A few sizes of arial
font are included with Zeal and are located in uifiles/zeal/fonts. See the zeal/bitmap_font.cpp
file for notes on how to generate new fonts. The default font is embedded in the code and can
be selected with the font name `default`.

* Zeal options combobox to select from available fonts
* Command examples:
  - `/map font arial_10` changes the current map font to Arial size 10

#### Map points of interest (poi), labels, and search
The map supports listing the available points of interest and adding them as labels to the map.
Note that some maps have many points of interest, so the all setting for the labels can clutter
up the map even in zoom. The keybind to toggle through the label modes is recommended.

* Zeal options combobox for setting the labels mode (off, summary, all)
* Key bind: "Toggle Map Labels" - toggles through the labels modes
* Command examples:
  - `/map poi` lists the available poi's, including their indices
  - `/map poi 4` adds a marker on index [4] of the `/map poi` list
  - `/map poi butcherblock` performs a text search of the poi list for butcherblock, reports 
     any matches, and adds a marker on the first one
  - `/map butcherblock` shortcut for above (does not work for terms that match other commands)
  - `/map labels summary` enables the summary labels (other options are `off`, `all`, or `marker`)

#### Map data source
The map has simple support for external map data files. The map data_mode can be set to `internal`,
`both`, or `external`. In `both`, the internal maps are combined with any available data from an
external file for that zone. In `external`, the internal map data for the zone is ignored if
external file data exists for that zone. In all cases internal data is used if external data is
not present.

Note that some features, such as level recognition, are not currently supported with external data.

The external map files must be placed in a `map_files` directory in the root everquest directory
with zones named to match their short names (ie `map_files/commons.txt` contains the data for
West Commonlands). If that short name file is present, it will also look for an optional `_1.txt`
file (ie `map_files/commons_1.txt`) and parse it if present. Most Brewall map files with POIs can
be directly dropped in (although any `_2.txt` or higher will be ignored).

The external map support requires a format compatible with Brewall map data.
```
L x0, y0, z0, x1, y1, z1, red, green, blue
P x, y, z, red, green, blue, ignored, label_string
```

* Zeal options combobox to set data mode
* Command examples:
  - `/map data_mode both` adds external zone map file data if present to internal maps
  - `/map data_mode external` uses external zone map files if present to replace internal maps
