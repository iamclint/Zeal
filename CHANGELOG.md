# Changelog

Summarizes notable changes to Zeal

## [0.6.4] - 2025/02/04

### New features:
Nameplates
* Add custom font supports to the nameplates
  - Controlled through nameplate options tab or /nameplate zealfont to toggle
  - Can use any zeal installed fonts (added more fonts with reasonable sizes)
  - Added drop shadow support, bottom align for centered text
* Enabled the target auto-attack blinking indicator. Synchronized
  it with the targetring and uses the targetring options slider for rate.
  - Added target blink always and only during auto-attack options
* Added option to show pet owner names as second line to nameplate ("Pet" for self-pet)
* Adds an option to display a health bar at the bottom of visible nameplates (custom font only)
  - Due to client data limitations, only applies to self, group, pet, target

Targetring:
* Add target_color option that uses target color instead of con color
* Add option to disable self target ring

Map:
* Added a small dull yellow position marker for self-pet if show group members is enabled
* Added a new options checkbox that enables text with your current location in the upper left corner
* Map supports loading zones not in the linked in database
  - Adds /map world command to lookup zone names

Floating combat damage:
* Added filters for suppressing damage from self, from pets, from other players, and from melee
* Switched to using the HPUpdate packets to report heal events which is mostly functional.
  - Note that self heals effectively bypass this so don't show up
* Added a "big hit" slider to make some damage outputs persist longer and stronger (values above threshold)
* Added FCD specific color settings in the zeal colors tab for controlling the FCD colors

Tab completion:
* Restored native client behavior to flush text beyond /tell and enable tab cycle list
* The recent tells list is always added to the cycle list at the end after any partial tab completion matches
* To simplify the logic, tab completion will not work in a tell window if there is more than one open.
  - The tab cycles between the tell windows. The main chat window still has tab completion.
* Added support for tab completion of /commands. Works like bash with first tab filling out
  common prefix and second tabs supporting cycling through match list.

New /protect command:
* Provides secondary protection against accidental item loss by blocking destroying or dropping
  items if a non-empty container, value is above a threshold, or the item is on a protection list.
* Also provides protection from selling items on the block list (but no value check for selling)
  - See readme for usage notes

General:
* Add /selfclickthru command and option ('u' to open doors works in 3rd person)
* Added a zeal general tab option to enable container locking
  (context menu popup will allow toggling "Lock")
* Added an optional quiet parameter (/useitem # quiet) that
  will suppress warnings if the slot doesn't have a click effect
  (empty slot or no valid click). Still complains if invalid
  command (slot #) or blocked due to casting.
* Clicking on the skills window name or value columns will now
  toggle ascending or descending order
* Add option to export inventory and spellbook on /camp

### Bug fixes and infrastructure:
* Added simple heap checks at zeal construction (boot) that will generate a modal
  dialog box if any corruption detected
* Added showspelleffects state to the reasonsfile
* Added a /zeal get_command utility that will retrieve the command info struct
  including callback function address
* Refactored BitmapFont int a virtual base class to support both a 2D transformed vertex
  class and a 3D class that supports the z-buffer with occlusion and range
* Migrate some of the eqgame.dll features into Zeal
  - Buff timers, extended nameplates, brown skeletons, auto-stand, combat music
* RightClickToEquip now directly calls InvSlot functions rather than InvSlotWnd x/y clicks


## [0.6.3] - 2025/01/18 (ac962b2)

### New features:
General:
* Implements proper /showhelm functionality and will fix future issues with velious helmets
  * showhelm will now affect how your helmet looks to you and how others see you
* Refactored nameplate code to eliminate delay of the health / marker text when switching targets
  * Modified target marker from < > to >> <<
  * Added legacy inline guild option
* Added a * suffix to the group leader in the party window
* Zeal chatfilters moved to a contextmenu submenu
  * Added a /who filter to the chat filter list
* Added a /linkall command with arguments to directly insert item links into chat channels
* Added enhanced player name tab completion to /tell, /t, and /consent (tab or shift-tab to cycle)
  * Supports partial name search that pulls from tell history, raid, and zone
* Added a cycle nearest PC corpses keybind option
* Skills window now sorts by skill name or value when column header clicked
* Alt + Left-click item display windows now persist their locations independently

Maps:
* Updated auto z-level to a default height value of 10 to match QuarmTools
* Added a default to z autofade checkbox option in the map options tab
* Added a command line override for the auto height (/map level autoz <height>)
* Reduced the allowed minimum position and marker icon sizes by 4x

### Bug fixes and infrastructure:
* Fixed a chat issue where the first word could trigger a command (ie, output = /output)
* Fixed an issue with persisting textures and fonts for directories with more than 20 files
* Minor namedpipe cleanups to reduce overhead and for thread safety
* Increase logging for external map createdevice failures


## [0.6.2] - 2024/12/28 (ac95b4e)

### New features:
Map:
* Updated the map interactive mode so that the map window is similar to other game windows (mouse sizing, positioning, minimize, close, lock, context menu)
* Removed the position and size sliders in the options tab
  - Added a persistent enable for interactive mode in map settings
* Added a default zoom selection option in map settings
* Modified external map to use standard window resizing instead of options slider (so independent of internal size)

Spellsets:
* Fixed some corner issues where the spellbookwnd state could get locked up (like trying to memorize a higher level spell after a death)
* Loading continues if it has trouble with a spell (unless corrupted ID) and skips the spell instead of spending the time to memorize then failing
* Loading can be interrupted to switch to a different set
* Loads from top to bottom
* Avoids some corner cases of stance dancing when terminating
* Added basic spellset name length constraints (1 to 32 chars)

ItemDisplay:
* Split the alt + left-click from the right click temporary windows for consistent behavior
* Supports up to 5 persistent items or songs
  - This works for item links, inventory, and the casting gem bar and spellbook
* The right click long hold uses the default Item Display window
* Simplified and centralized the item description mods
* Added some initial spell info fields (like resists, target type)
* Fixed an issue with bank and merchant window z-layers when displaying items
* Combat effect (proc level) added
* Add class levels to spell scroll info item display

Floating combat:
* Added bitmap_font support to floating damage
* If font set, uses it, else falls back to client font size
* Added options combobox support for the fonts
* Updated bitmap_font and fonts
* Added larger arial font sizes 20, 24, 32

Melody:
* Changed from empirical delay to a server message interlock before advancing songs (should increase reliability)
* Make /stopsong more reliable by using an internal function that checks if the bard is singing a bard song

Right-click to equip:
* New option to enable right click to swap equipment from a bag to equip slot
  - From inside a bag only to avoid clicky collision

Misc:
* Add transparency ring slider for Target Ring
* Fix to gemicon default ui fallback path
* Possible fix to random color picker window bug by clearing ui_manager's button states
* Various other UI fixes that may improve stability after character select swaps
* Fixed /camp autosit to not sit if command will fail due to transaction windows
* Added /lootall command

### Bug fixes and infrastructure:
* Fixed a target ring ini issue for None transparency
* Fixed false haste on items with other (worn) effects


## [0.6.1] - 2024/12/08 (4aa4bb7)

### Bug fixes:
* Fixes map crash due to uninitialized font when no labels are active
* Fixes %t chat crash and makes the dopercentconvert calls more reliable


## [0.6.0] - 2024/11/30 (6de33da)

### New features:
* Separated the Load and Delete of spellsets so that Load is a single click
  while Delete is hidden behind a subcategory menu
* Map ring now supports indoor tracking distances

### Bug fixes and infrastructure:
* Fix StringSprite crash (skellies)
* Add a redundant Zeal load check to dllmain
* Adds trim_name call to target nameplate (deal with rare crash)
* Move texture release above the filename length check so "none" will work
* Gargoyle Nameplate fix


## [0.5.9] - 2024/11/24 (ffbfccd)  

### New features:
Map:
* Add map interactive mouse support (drag, pan zoom)
* Add /map show_zone mode to browse other zones
* Add /map ring to show tracking and distances
* Add map z-level fading and auto z-level mode
* Add succor to all zones using zone data
* Change keybind to toggle vs flash raid/group names

Nameplates:
* Major changes to support color customization and new features like target marker, health, etc
 
General:
* Chat filters & custom color updates for pets
* Added /corpsedrag nearest : auto-targets the closest corpse for /drag
* Added /corpsedrop all: drop all corpses
* Updated /lead command updated to report raid and group leader information
* Add instrument mod percent to item display
* Add slot #'s to /useitem
* Camera option to unlock for key turning
* Target Ring -- cone (depth) and indicator flash delay settings
* Floating Combat -- Spell and Spell Icon toggles
* Add /cls and /ss aliases add spell icons to floating combat add non-melee damage from other players to chat
* make tellwindow history color gray, add toggle for tell window history

### Bug fixes and infrastructure:
* Moved majority of zeal settings to separate zeal.ini file
* Construct ZealService on loader thread (boot stability)
* Few patches to improve boot stability (null SpawnInfo)
* Warnings about duplicate xmls
* ZealSetting improvements
* Use client's EQP_IDArray for ID to entity LUT
* Patch client to properly Type other PC corpses
* Restrict /useitem to match server side effect list
* Misc others


## [0.5.5] - 2024/11/02 (fa5ec26)

### New features:

General:
* Add /lead command to print group leader
* Options window updates
* add cut to edit windows, fix issue with large pastes
* Add item value to item display
* Disable character selection rotation by default
* Nameplate color updates (options, naming)

Map:
* Add multiple map markers with labels support
* Add map support for showing group and raid names with adjustable length
* Add a simple map grid option
* Upgraded to use custom bitmapfont (eliminates text label fps penalty)
* Add a 'marker_only' map labels mode

### Bug fixes and infrastructure:
* Added checks for if a group/raid exists before trying to send data via Pipe
* Fix Con Color initialization ui_options.cpp
* Fix infinite loop lockup caused by print_chat_wnd not calling the trampoline properly
* Add item link limit to pasting to solve another crash issue.
* Fix bluecon everywhere to use the color index 14
* Fix print chat buffer, attempt to make DoPercentConvert safer (some crashes reported when
 using assist macros may be related). 
* Add directx BitmapFont to accelerate text
* Prevent /useitem from trying to cast "(worn)" effects


## [0.5.0] - 2024/10/10 (24a2cc9)

### Notes:
Release notes
* EQUI.XML is no longer needed inside the zeal folder, please delete the entire uifiles/zeal before extracting the new zip
* If you do not see a zeal options window when you open your options window something is extracted wrong.
* Extract all of the files to your everquest folder from the zip in the paths they are predefined to go into.

### New features:
* Target ring updates
* Map: Add external window support
* Add group & raid positions to named pipe output
* Nameplate color updates

### Bug fixes and infrastructure:
* Fix entity remove (name is not reliable as a hash key)
* Separate tabs into own xml files, Add color selections for nameplate Add color saving/loading
* Separate color tab init into own function
* Add button callbacks for ui
* Add wnd notification hook for colorpicker callback system
* Add colorpicker wnd virtual calls activate and setcolor

