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
- Additional key binds (tab targeting, strafe, pet)
- Additional commands (melody, useitem, autoinventory)
- Additional ui support (new gauges, bag control, looting, spellsets, targetrings, nameplates)
- Third party tool support (silent log messages, direct ZealPipes)
- Integrated map (see In-game Map section below)
- Various bug fixes
- Unique npc naming for better parsing

### Installation
#### The easy way
1. [Download](https://github.com/iamclint/Zeal/releases) the latest Zeal
   - Scroll down past the list of changes to the `Assets` section and download the `zeal_v#.#.#_*.zip` file
   - Ignore the `Source code` files
2. Quit Everquest and extract the zip file contents into your EQ game root directory
   - Key required files are the `zeal.asi` and the `uifiles/zeal` folder
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

### Commands
___
- `/sortgroup`
  - **Arguments:** ``
  - **Example:** `/sq` `/sq 1 2`
  - **Description:** sorts your current group members in the ui  using /sq 1 2 will swap players 1 and 2 in your group on your ui.

- - `/cc`
  - **Arguments:** ``
  - **Example:** `/cc`
  - **Description:** toggles classic classes in who and other areas.

- `/targetring`
  - **Arguments:** `size`, `indicator`
  - **Example:** `/targetring 0.25`
  - **Example:** `/targetring indicator` toggles auto attack indicator.
  - **Description:** toggles targetring on/off.

- `/resetexp`
  - **Arguments:** ``
  - **Example:** `/resetexp`
  - **Description:** resets the exp per hour calculations.
  - 
- `/timer`
  - **Arguments:** `int`
  - **Example:** `/timer 10`
  - **Description:** holds the last hotbutton pressed down for the duration (decisecond like /pause).
  
- `/pipedelay`
  - **Arguments:** `int`
  - **Example:** `/pipedelay 500`
  - **Description:** changes the delay between each loop of labels/gauges being sent out over the named pipe.

- `/pipe`
  - **Arguments:** `string`
  - **Example:** `/pipe set a respawn timer for 30 seconds`
  - **Description:** outputs a string through the named pipe.
  
- `/ttimer`
  - **Arguments:** `int`
  - **Example:** `/ttimer 500`
  - **Description:** change the delay in which a tooltip pops up on mouse hover.
  
- `/inspect target`
  - **Description:** adds target argument to /inspect, this just inspects your current target.

- `/loc noprint`
  - **Description:** adds noprint argument to /loc, this just sends loc directly to your log.

- `/reloadskin`
  - **Description:** reloads your current skin using ini.
    
- `/fcd` (floating combat damage)
  - **Arguments:** none, `client font size #`, `font`
  - **Example:** `/fcd` toggles on and off
  - **Example:** `/fcd 6` sets it to use client font size 6
  - **Example:** `/fcd font arial_24` sets it to use custom font arial_24
  - **Description:** shows floating combat damage.
  
- `/fov`
  - **Arguments:** `int`
  - **Example:** `/fov 65`
  - **Description:** changes your field of view with a value between 45 and 90.

- `/lead`
  - **Arguments:** none, `open` (reports raid groups with open slots), `all` (lists all raid groups)
  - **Description:** prints out your current group leader (and raid leader if in raid).

- `/melody`
  - **Arguments:** `song gem #'s (maximum of 5)`
  - **Aliases:** `/mel`
  - **Example:** `/melody 1 4 2 3`
  - **Description:** plays songs in order until interrupted in any fashion.

- `/map`
  - **Arguments:** `on`, `off`, `size`, `alignment`, `marker`, `background`, `zoom`, `poi`, `labels`, `level`
  - **Example:** See In-game map section below
  - **Description:** controls map enable, size, labels, zoom, and markers
    
- `/pandelay`
  - **Arguments:** `ms delay`, `none`
  - **Example:** `/pandelay 200`
  - **Description:** changes the amount of delay before left click panning will start to happen
    
- `/hidecorpse`
  - **Arguments:** `looted`, `none`
  - **Aliases:** `/hideco`, `/hidec`, `/hc`
  - **Example:** `/hidecorpse looted`
  - **Description:** `looted` Hides a corpse after you have looted it., `none` reveals all hidden corpses

- `/spellset`
  - **Arguments:** `save`, `load`, `delete`
  - **Example:** `/spellset save buffs`
  - **Example:** `/spellset load buffs`
  - **Example:** `/spellset delete buffs`
  - **Description:** allows you to save and load spellsets

- `/showhelm`
  - **Aliases:** `/helm`
  - **Arguments:** `on, off`
  - **Example:** `/showhelm on`
  - **Description:** Toggles your helmet.

- `/showlootlockouts`
  - **Aliases:** `/showlootlockout`, `/showlockout`, `/sll`
  - **Description:** Shows you your current loot lockouts on Quarm.

- `/zealcam`
  - **Aliases:** `/smoothing`
  - **Arguments:** `x y 3rdperson_x 3rdperson_y`
  - **Example:** `/zealcam 0.7 0.2 0.7 0.2` if 3rd person arguments are not supplied, the first x and y are applied to both
  - **Description:** Toggles Zeal's mouse look smoothing methods, the first 2 arguments are first person sensitivity, and the last 2 are for 3rd person

- `/autoinventory`
  - **Aliases:** `/autoinv`, `/ai`
  - **Description:** Drops whatever is on your cursor into your inventory.

- `/autobank`
  - **Aliases:** `/autoba`, `/ab`
  - **Description:** Drops whatever is on your cursor into your bank. [requires you to be at a banker] (not fully functional atm)

- `/corpsedrag`
  - **Aliases:** `/drag`
  - **Arguments:** none, `nearest` (auto-targets nearest corpse for dragging)
  - **Description:** Attempts to corpse drag your current target. Use `/corpsedrag nearest` to auto-target.

- `/corpsedrop`
  - **Aliases:** `/drop`
  - **Arguments:** none, `all` (drops all corpses)
  - **Description:** Stop dragging your currently targeted corpse. To drop all use `/corpsedrop all`.

- `/target`
  - **Aliases:** `/cleartarget`
  - **Description:** acts as normal /target unless you provide no argument in which case it will clear your target.

- `/sit`
  - **Description:** The /sit command now accepts "on" as an argument. Using "/sit on" will always make you sit, even if you are currently sitting. This matches the game's native "/sit off" which always makes you stand even if you are currently standing. The "/sit" command will continue to toggle sit/stand state if no argument is provided or if the argument provided is not on or off. Additionally, "/sit down" now works as well and will always make you sit, even if already sitting.

- `/camp`
  - **Description:** Auto sits before camping.

- `/zeal`
  - **Arguments:** `version`
  - **Description:** Shows the version of zeal.

- `/zealinput`
  - **Description:** toggles the zeal input setup for any input in game, giving you a more modern input (ctrl+c, ctrl+v, left, right, shift left+right for highlighting, home, end ect).

- `/help zeal`
  - **Description:** Shows the custom Zeal commands.

- `/timestamp`
  - **Aliases:** `/tms`
  - **Description:** Shows message timestamps.

- `/outputfile`
  - **Aliases:** `/output`, `/out`
  - **Arguments:** `inventory | spellbook` `[optional_name]`
  - **Example:** `/outputfile inventory my_inventory`
  - **Description:**
    - `inventory` outputs information about your equipment, inventory bag slots, held item, and bank slots to a file.
    - `spellbook` outputs a list of all spell ids current scribed in your spellbook.

- `/buffs`
  - **Description:** Outputs the players buff timers to the chat only if they are using OldUI.

- `/bluecon`
  - **Description:** Changes the blue con color to Zeal Color Button #15 which is in the Zeal Options window, Colors Tab.

- `/alarm`
  - **Arguments:** `oldui`
  - **Description:** Re-opens the alarm window, if oldui is specified it allows for an alarm on it.

- `/nameplatecolors`
  - **Description:** toggles nameplate colors for players

- `/nameplateconcolors`
  - **Description:** toggles nameplate con colors for npcs

- `/nameplatehideself`
  - **Description:** toggles nameplate for self (on/off)

- `/nameplatex`
  - **Description:** toggles nameplate for self as X (on/off)

- `/nameplatehideraidpets`
  - **Description:** toggles nameplate for raid pets and your pet (on/off)
 
- `/nameplatecharselect`
  - **Description:** toggles nameplate choices shown at character selection screen on and off (on/off)

- `/nameplatetargetcolor`
  - **Description:** toggles target nameplate color on and off (on/off)
 
- `/nameplatetargetmarker`
  - **Description:** toggles target nameplate marker on and off (on/off)
 
- `/nameplatetargethealth`
  - **Description:** toggles target nameplate health on and off (on/off)
___
### Binds
- Cycle through nearest NPCs
- Cycle through nearest PCs
- Strafe Right
- Strafe Left
- Auto Inventory
- Toggle last 2 targets
- Reply target
- Pet Attack
- Pet Guard
- Pet Follow
- Pet Back
- Slow turn left
- Slow turn right
- Target nearest pc corpse
- Target nearest npc corpse
- Toggle map on/off
- Toggle through map default zooms
- Toggle through map backgrounds
- Toggle through map label modes
- Toggle up or down through visible map levels
- Toggle map visibility of raid members and member names
- Toggle map visibility of grid lines
- Toggle map interactive mode (internal overlay)
- Press to display group and raid member labels
- Toggle nameplate colors for players on/off
- Toggle nameplate con colors for npcs on/off
- Toggle nameplate for self on/off
- Toggle nameplate for self as X on/off
- Toggle nameplate for raid pets and your pet on/off
- Toggle nameplate choices that are shown at character selection screen on and off
- Toggle target nameplate color on and off
- Toggle target nameplate marker on and off
- Toggle target nameplate health on and off
___
### UI
- **Gauge EqType's**
  - `23` EXP Per Hour

- **Label EqType's**
  - `80` Mana/Max Mana
  - `81` Exp Per Hour Percentage
  - `124` Current Mana
  - `125` Max Mana
  - `134` Spell being casted

- **LootAllButton**
- **LinkAllButton**


### Options UI 
- **ScreenID Checkboxes**
  - Zeal_ShowHelm
  - Zeal_HideCorpse
  - Zeal_Cam
  - Zeal_BlueCon
  - Zeal_Timestamp
  - Zeal_Input
  - Zeal_Escape_
    
- **ScreenID Sliders**
  - Zeal_PanDelaySlider
  - Zeal_FirstPersonSlider_X
  - Zeal_FirstPersonSlider_Y
  - Zeal_ThirdPersonSlider_X
  - Zeal_ThirdPersonSlider_Y
    
- **ScreenID Labels**
  - Zeal_ThirdPersonLabel_X
  - Zeal_ThirdPersonLabel_Y
  - Zeal_FirstPersonLabel_X
  - Zeal_FirstPersonLabel_Y
  - Zeal_PanDelayLabel
___

### Zeal pipes in c#
- https://github.com/OkieDan/ZealPipes

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

### Nameplate Options
#### Setup and configuration
Zeal 5.0 and later includes options for players to adjust Player Nameplates and NPC Nameplates in game.
In addition, Skeletons now show a Nameplate. (Client Skeleton Nameplate Bug Fix)
Necromancers will now have an easier time finding their corpses.

The nameplate is controlled through three interfaces:
* Dedicated Zeal options window tab (requires `zeal\uifiles`, see Installation notes above)
* Key binds for nameplate options (configure in EQ Options->Keyboard->Target)
* The five /nameplate commands listed below

#### Enabling Disabling Nameplate Options
* The `/nameplatecolors` command - Toggles Nameplate Colors for Players on and off
* The `/nameplateconcolors` command - Toggles Nameplate Con Colors for NPCs on and off
* The `/nameplatehideself` command - Toggles Player Nameplate on and off
* The `/nameplatex` command - Toggles Player Nameplate as X on and off
* The `/nameplatehideraidpets` command - Toggles NPC Raid Pets Nameplate on and off
* The `/nameplatecharselect` command - Toggles Nameplate Choices Shown at Character Selection Screen on and off
* The `/nameplatetargetcolor` command - Toggles Target Nameplate Color on and off
* The `/nameplatetargetmarker` command - Toggles Target Nameplate Marker on and off
* The `/nameplatetargethealth` command - Toggles Target Nameplate Health on and off

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
The map supports drag panning and mouse wheel zoom in interactive mode. Interactive mode is always 
enabled in external window mode, while a keybind toggle is used to toggle the internal overlay map
in and out of interactive mode. When not in interactive mode, the internal overlay map is transparent
to the mouse. When interactive mode is enabled, the map can be panned using a left mouse button drag
and zoomed using the scroll wheel. Once panning starts, auto-center is disabled until a right mouse
button click. The external window also supports a middle mouse button click to drop markers.

* Key bind: "Toggle Map Interactive Mode" - toggles internal overlay between transparent and interactive

#### Show zone mode
Zone maps other than the player's current location can be explored using the show_zone command. The
target zone is specified using the zone's short name (like /who all). Interactive mode, levels, grid,
labels, and poi search all work for the selected zone.

* Command examples
  - `/map show_zone gukbottom` shows the zone map for the Ruins of Old Guk
  - `/map show_zone` exits show_zone mode

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
on and off with `/map ring`.

* Command examples:
  - `/map ring` if visible or a non-tracker, turns ring off
  - `/map ring` if not visible, sets the ring at max tracking distance per skill level
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
West Commonlands).  An optional, `_1.txt` file (ie `map_files/commons_1.txt`) will also be
parsed if present, so Brewall map files with POIs can be directly dropped in.

The external map support requires a format compatible with Brewall map data.
```
L x0, y0, z0, x1, y1, z1, red, green, blue
P x, y, z, red, green, blue, ignored, label_string
```

* Zeal options combobox to set data mode
* Command examples:
  - `/map data_mode both` adds external zone map file data if present to internal maps
  - `/map data_mode external` uses external zone map files if present to replace internal maps
