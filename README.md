### Zeal pipes in c#
- https://github.com/OkieDan/ZealPipes

### Compatible UI's
- https://github.com/NilliP/NillipussUI_1080p
- https://github.com/NilliP/NillipussUI_1440p
- https://www.eqinterface.com/downloads/fileinfo.php?id=6959


### Features
- Camera motion improvements
- Additional binds
- Additional commands
- Additional ui support
- Various bug fixes
- Unique npc naming for better parsing

### Commands
___
- `/targetring`
  - **Arguments:** `float size`
  - **Example:** `/targetring 0.25`
  - **Description:** toggles targetring on/off if float is specified it sets the size.

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
    
- `/fov`
  - **Arguments:** `int`
  - **Example:** `/fov 65`
  - **Description:** changes your field of view with a value between 45 and 90.
    
- `/melody`
  - **Arguments:** `song gem #'s (maximum of 5)`
  - **Aliases:** `/mel`
  - **Example:** `/melody 1 4 2 3`
  - **Description:** plays songs in order until interrupted in any fashion.
    
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
  - **Description:** Changes the blue con color to usercolor #70 which is otherwise unused, you can edit in the options window.

- `/alarm`
  - **Arguments:** `oldui`
  - **Description:** Re-opens the alarm window, if oldui is specified it allows for an alarm on it.
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
### Building and Installation
<br>
32bit x86
<br>
file extension .asi
<br>
move zeal.asi into the root of your game folder
